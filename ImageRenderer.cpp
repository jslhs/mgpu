#include "ImageRenderer.h"

ImageRenderer::ImageRenderer(HWND hwnd)
	: _hwnd(hwnd)
{
	init();
}

ImageRenderer::~ImageRenderer()
{
}

void ImageRenderer::init()
{
	DXGI_SWAP_CHAIN_DESC scd = {};
	RECT rc = {};
	GetClientRect(_hwnd, &rc);
	auto w = rc.right - rc.left;
	auto h = rc.bottom - rc.top;
	scd.BufferCount = 1;
	scd.BufferDesc.Width = w;
	scd.BufferDesc.Height = h;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferDesc.RefreshRate.Numerator = 60;
	scd.BufferDesc.RefreshRate.Denominator = 1;
	scd.Windowed = true;
	scd.OutputWindow = _hwnd;
	scd.SampleDesc.Count = 1;
	scd.SampleDesc.Quality = 0;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	D3D_DRIVER_TYPE driveTypes[] = 
	{ 
		D3D_DRIVER_TYPE_HARDWARE, 
		D3D_DRIVER_TYPE_WARP, 
		D3D_DRIVER_TYPE_REFERENCE 
	};

	UINT flags[] = 
	{ 
		D3D11_CREATE_DEVICE_DEBUG, 
		0 
	};

	D3D_FEATURE_LEVEL levels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0
	};

	for (auto &type : driveTypes)
	{
		for (auto &flag : flags)
		{
			_hr = D3D11CreateDeviceAndSwapChain(nullptr
				, type
				, nullptr
				, flag
				, levels
				, ARRAYSIZE(levels)
				, D3D11_SDK_VERSION
				, &scd
				, &_swap_chain
				, &_dev
				, &_level
				, &_ctx);
			if (_hr) break;
		}
		if (_hr) break;
	}

	vertex *vp = nullptr;
	D3D11_INPUT_ELEMENT_DESC ils[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, (UINT)(&vp->pos), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, (UINT)(&vp->tex), D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	shader_factory sf(_dev);

	std::string ss = "\
		Texture2D _color_map : register(t0);\
		SamplerState _sampler : register(s0);\
		\
		cbuffer changed_on_resize : register(b0) \
		{\
			matrix projm;\
		};\
		\
		struct vs_input\
		{\
			float4 pos : POSITION;\
			float2 tex0 : TEXCOORD0;\
		};\
		\
		struct vs_output\
		{\
			float4 pos : SV_POSITION;\
			float2 tex0 : TEXCOORD0;\
		};\
		\
		vs_output vs_main(vs_input v)\
		{\
			vs_output vo = (vs_output)0;\
			vo.pos = mul(v.pos, projm);\
			vo.tex0 = v.tex0;\
			return vo;\
		}\
		\
		float4 ps_main(vs_output frag) : SV_TARGET\
		{\
			return _color_map.Sample(_sampler, frag.tex0);\
		}\
		";

	sf.set_source(ss);
	//sf.set_source(std::ifstream("image.fx"));
	sf.set_flags1(D3DCOMPILE_DEBUG | D3DCOMPILE_ENABLE_STRICTNESS);
	_hr = sf.create_pixel_shader(&_ps, "ps_5_0", "ps_main");
	_hr = sf.create_vertex_shader(&_vs, "vs_5_0", "vs_main");
	_hr = sf.create_input_layout(ils, ARRAYSIZE(ils), &_layout);

	D3D11_SAMPLER_DESC sd = {};
	sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sd.MaxLOD = D3D11_FLOAT32_MAX;
	_hr = _dev->CreateSamplerState(&sd, &_sampler);

	using namespace DirectX;
	D3D11_BUFFER_DESC cb_desc = {};
	cb_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb_desc.ByteWidth = sizeof(XMMATRIX);
	cb_desc.Usage = D3D11_USAGE_DEFAULT;

	_hr = _dev->CreateBuffer(&cb_desc, nullptr, &_cb_proj_m);

	
	D3D11_RENDER_TARGET_BLEND_DESC rtbd = {};

	rtbd.BlendEnable			 = TRUE;
	rtbd.SrcBlend				 = D3D11_BLEND_SRC_ALPHA;
	rtbd.DestBlend				 = D3D11_BLEND_INV_SRC_ALPHA;
	rtbd.BlendOp				 = D3D11_BLEND_OP_ADD;
	rtbd.SrcBlendAlpha			 = D3D11_BLEND_ONE;
	rtbd.DestBlendAlpha			 = D3D11_BLEND_ZERO;
	rtbd.BlendOpAlpha			 = D3D11_BLEND_OP_ADD;
	rtbd.RenderTargetWriteMask	 = D3D11_COLOR_WRITE_ENABLE_ALL;

	D3D11_BLEND_DESC blend_desc = {};
	blend_desc.AlphaToCoverageEnable = FALSE;
	blend_desc.RenderTarget[0] = rtbd;

	_hr = _dev->CreateBlendState(&blend_desc, &_blend);

	resizeBuffer();
}

void ImageRenderer::resizeBuffer()
{
	RECT rc = {};
	GetClientRect(_hwnd, &rc);
	float w = (float)(rc.right - rc.left);
	float h = (float)(rc.bottom - rc.top);

	if(w <= 0 || h <= 0) return;

	_ctx->ClearState();
	_back_buf_target = nullptr;
	_back_buf_tex = nullptr;

	_hr = _swap_chain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
	_hr = _swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID *)&_back_buf_tex);
	_hr = _dev->CreateRenderTargetView(_back_buf_tex, nullptr, &_back_buf_target);
	_ctx->OMSetRenderTargets(1, &_back_buf_target, nullptr);

	D3D11_VIEWPORT viewport = {};
	viewport.Width = w;
	viewport.Height = h;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	_ctx->RSSetViewports(1, &viewport);

	using namespace DirectX;
	//_proj_m = XMMatrixTranspose(XMMatrixOrthographicLH((float)w, (float)h, 0.1f, 1.0f));
	//_proj_m = XMMatrixTranspose(XMMatrixOrthographicOffCenterLH(-w, w, -h, h, 0.1f, 1.0f));
	_proj_m = XMMatrixTranspose(XMMatrixOrthographicOffCenterLH(0.0f, w, h, 0.0f, 0.1f, 1.0f));
	_ctx->UpdateSubresource(_cb_proj_m, 0, nullptr, &_proj_m, 0, 0);
}

void ImageRenderer::Render(const Scene &scene)
{
	float color[4] = {};
	_ctx->ClearRenderTargetView(_back_buf_target, color);

	for (auto &s : scene._sprites)
	{
		float x = s.pos.x;
		float y = s.pos.y;
		float w = s.size.x;
		float h = s.size.y;
		vertex v[]=
		{
			/*
			vertex(vec3(x + w, y + h, 1.0f), vec2(1.0f, 0.0f)),
			vertex(vec3(x + w, y - h, 1.0f), vec2(1.0f, 1.0f)),
			vertex(vec3(x - w, y - h, 1.0f), vec2(0.0f, 1.0f)),
			vertex(vec3(x - w, y - h, 1.0f), vec2(0.0f, 1.0f)),
			vertex(vec3(x - w, y + h, 1.0f), vec2(0.0f, 0.0f)),
			vertex(vec3(x + w, y + h, 1.0f), vec2(1.0f, 0.0f)),
			*/
			vertex(vec3(x + w, y	, 1.0f), vec2(1.0f, 0.0f)),
			vertex(vec3(x + w, y + h, 1.0f), vec2(1.0f, 1.0f)),
			vertex(vec3(x	 , y + h, 1.0f), vec2(0.0f, 1.0f)),
			vertex(vec3(x	 , y + h, 1.0f), vec2(0.0f, 1.0f)),
			vertex(vec3(x	 , y	, 1.0f), vec2(0.0f, 0.0f)),
			vertex(vec3(x + w, y	, 1.0f), vec2(1.0f, 0.0f)),
		};

		D3D11_BUFFER_DESC vbd = {};
		vbd.Usage = D3D11_USAGE_DEFAULT;
		vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbd.ByteWidth = sizeof(v);

		D3D11_SUBRESOURCE_DATA res = {};
		res.pSysMem = v;
		com_ptr<ID3D11Buffer> vb;
		_hr = _dev->CreateBuffer(&vbd, &res, &vb);

		D3D11_TEXTURE2D_DESC desc = {};
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.ArraySize = 1;
		desc.Width = s.width;
		desc.Height = s.height;
		desc.SampleDesc.Count = 1;
		desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MipLevels = 1;

		com_ptr<ID3D11Texture2D> tex;
		com_ptr<ID3D11ShaderResourceView> tex_view;
		res.pSysMem = s.img;
		res.SysMemPitch = s.width * 4;
		_hr = _dev->CreateTexture2D(&desc, &res, &tex);
		_hr = _dev->CreateShaderResourceView(tex, nullptr, &tex_view);

		UINT stride = sizeof(vertex);
		UINT offset = 0;
		_ctx->IASetInputLayout(_layout);
		_ctx->IASetVertexBuffers(0, 1, &vb, &stride, &offset);
		_ctx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		_ctx->VSSetConstantBuffers(0, 1, &_cb_proj_m);
		_ctx->VSSetShader(_vs, nullptr, 0);
		_ctx->PSSetShader(_ps, nullptr, 0);
		_ctx->PSSetSamplers(0, 1, &_sampler);
		_ctx->PSSetShaderResources(0, 1, &tex_view);
		_ctx->OMSetBlendState(_blend, nullptr, 0xffffffff);

		_ctx->Draw(ARRAYSIZE(v), 0);
	}
	
	_hr = _swap_chain->Present(0, 0);
}

void ImageRenderer::ResizeBuffer()
{
	resizeBuffer();
}

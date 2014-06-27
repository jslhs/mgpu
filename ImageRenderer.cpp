#include "ImageRenderer.h"

ImageRenderer::ImageRenderer(HWND hwnd)
	: _hwnd(hwnd)
{
	init();
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
	D3D_DRIVER_TYPE driveTypes[] = { D3D_DRIVER_TYPE_HARDWARE, D3D_DRIVER_TYPE_WARP, D3D_DRIVER_TYPE_REFERENCE };
	UINT flags[] = { D3D11_CREATE_DEVICE_DEBUG };
	for (auto &type : driveTypes)
	{
		for (auto &flag : flags)
		{
			D3D_FEATURE_LEVEL levels[] =
			{
				D3D_FEATURE_LEVEL_11_1,
				D3D_FEATURE_LEVEL_11_0
			};

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

	float hw = 0.5f;
	float hh = 0.5f;

	_verts.push_back(vertex{ vec3(hw, hh, 1.0f), vec2(1.0f, 0.0f) });
	_verts.push_back(vertex{ vec3(hw, -hh, 1.0f), vec2(1.0f, 1.0f) });
	_verts.push_back(vertex{ vec3(-hw, -hh, 1.0f), vec2(0.0f, 1.0f) });

	_verts.push_back(vertex{ vec3(-hw, -hh, 1.0f), vec2(0.0f, 1.0f) });
	_verts.push_back(vertex{ vec3(-hw, hh, 1.0f), vec2(0.0f, 0.0f) });
	_verts.push_back(vertex{ vec3(hw, hh, 1.0f), vec2(1.0f, 0.0f) });

	D3D11_BUFFER_DESC vbd{};
	vbd.Usage = D3D11_USAGE_DEFAULT;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.ByteWidth = sizeof(vertex) * _verts.size();

	D3D11_SUBRESOURCE_DATA res{};
	res.pSysMem = &_verts[0];

	_hr = _dev->CreateBuffer(&vbd, &res, &_vb);
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
			vo.pos = v.pos;\
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

	resizeBuffer();
}

void ImageRenderer::resizeBuffer()
{
	_ctx->ClearState();
	_back_buf_target = nullptr;
	_back_buf_tex = nullptr;

	_hr = _swap_chain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
	_hr = _swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID *)&_back_buf_tex);
	_hr = _dev->CreateRenderTargetView(_back_buf_tex, nullptr, &_back_buf_target);
	_ctx->OMSetRenderTargets(1, &_back_buf_target, nullptr);

	D3D11_VIEWPORT viewport{};
	RECT rc = {};
	GetClientRect(_hwnd, &rc);
	auto w = rc.right - rc.left;
	auto h = rc.bottom - rc.top;
	viewport.Width = static_cast<float>(w);
	viewport.Height = static_cast<float>(h);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	_ctx->RSSetViewports(1, &viewport);
}

void ImageRenderer::createTexture(unsigned int width, unsigned int height)
{
	D3D11_TEXTURE2D_DESC desc = {};
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.ArraySize = 1;
	desc.Width = width;
	desc.Height = height;
	desc.SampleDesc.Count = 1;
	desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MipLevels = 1;
	_hr = _dev->CreateTexture2D(&desc, nullptr, &_tex);
	
	_hr = _dev->CreateShaderResourceView(_tex, nullptr, &_tex_view);
}

void ImageRenderer::Update(void *buf, unsigned int width, unsigned int height)
{
	//if (!_tex) createTexture(width, height);

	//D3D11_MAPPED_SUBRESOURCE res = {};
	//_hr = _ctx->Map(_tex, 0, D3D11_MAP_WRITE_DISCARD, 0, &res);
	//_ctx->Unmap(_tex, 0);

	D3D11_TEXTURE2D_DESC desc = {};
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.ArraySize = 1;
	desc.Width = width;
	desc.Height = height;
	desc.SampleDesc.Count = 1;
	desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MipLevels = 1;
	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = buf;
	data.SysMemPitch = width * 4;
	_hr = _dev->CreateTexture2D(&desc, &data, &_tex);

	_hr = _dev->CreateShaderResourceView(_tex, nullptr, &_tex_view);
}

void ImageRenderer::Render()
{
	float color[3] = {};
	_ctx->ClearRenderTargetView(_back_buf_target, color);

	UINT stride = sizeof(vertex);
	UINT offset = 0;
	_ctx->IASetInputLayout(_layout);
	_ctx->IASetVertexBuffers(0, 1, &_vb, &stride, &offset);
	_ctx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	_ctx->VSSetShader(_vs, nullptr, 0);
	_ctx->PSSetShader(_ps, nullptr, 0);
	_ctx->PSSetSamplers(0, 1, &_sampler);
	_ctx->PSSetShaderResources(0, 1, &_tex_view);

	_ctx->Draw(_verts.size(), 0);
	_hr = _swap_chain->Present(0, 0);
}

void ImageRenderer::ResizeBuffer()
{
	resizeBuffer();
}

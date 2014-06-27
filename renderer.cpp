#include "renderer.h"

renderer::renderer(window &w)
	: _w(w)
	, _scale(1.0f, 1.0f, 1.0f)
{
	_trans.x = 0;
	_trans.y = 0;
	_trans.z = 0;
	init_dev();
	_w.did_resize() += [&](window&, size_event_args &e){
		std::cout << "width = " << e.width() << ", height = " << e.height() << std::setw(40) << "\r";
		if (e.type() != SIZE_MINIMIZED && e.type() != SIZE_MAXHIDE)
		{
			float width = e.width();
			float height = e.height();
			using namespace DirectX;
			//_proj_matrix = XMMatrixPerspectiveFovLH(XM_PI * 0.3, width / height, 0.01f, 100.0f);
			//_proj_matrix = XMMatrixTranspose(_proj_matrix);
			_proj_matrix = XMMatrixTranspose(XMMatrixOrthographicLH(width / height, 1.0f, 0.1f, 1.0f));
			init_view();
		}
	};

	load();
}

renderer::~renderer()
{
	//_dev->CreateVertexShader()
}

void renderer::update(float dt)
{

}

void renderer::render()
{
	float color[3] = {};
	_ctx->ClearRenderTargetView(_back_buf_target, color);

	UINT stride = sizeof(vertex);
	UINT offset = 0;
	_ctx->IASetInputLayout(layout);
	_ctx->IASetVertexBuffers(0, 1, &vb, &stride, &offset);
	_ctx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	_ctx->VSSetShader(vs, nullptr, 0);
	_ctx->PSSetShader(ps, nullptr, 0);
	_ctx->PSSetSamplers(0, 1, &_sampler);
	_ctx->PSSetShaderResources(0, 1, &_color_tex);

	using namespace DirectX;
	auto m = XMMatrixScaling(_scale.x, _scale.y, _scale.z) * XMMatrixRotationRollPitchYaw(_rot.x, _rot.y, _rot.z) * XMMatrixTranslation(_trans.x, _trans.y, _trans.z);
	_world_matrix = XMMatrixTranspose(m);

	_ctx->UpdateSubresource(_cb_world_matrix, 0, 0, &_world_matrix, 0, 0);
	_ctx->UpdateSubresource(_cb_view_matrix, 0, 0, &_view_matrix, 0, 0);
	_ctx->UpdateSubresource(_cb_proj_matrix, 0, 0, &_proj_matrix, 0, 0);

	_ctx->VSSetConstantBuffers(0, 1, &_cb_world_matrix);
	_ctx->VSSetConstantBuffers(1, 1, &_cb_view_matrix);
	_ctx->VSSetConstantBuffers(2, 1, &_cb_proj_matrix);

	_ctx->Draw(_verts.size(), 0);
	_hr = _swap_chain->Present(0, 0);
}

void renderer::set_full_screen(bool val)
{
	_swap_chain->SetFullscreenState(val ? TRUE : FALSE, nullptr);
}

void renderer::init_dev()
{
	DXGI_SWAP_CHAIN_DESC scd{};
	auto rc = _w.client_rect();
	scd.BufferCount = 1;
	scd.BufferDesc.Width = rc.width();
	scd.BufferDesc.Height = rc.height();
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferDesc.RefreshRate.Numerator = 60;
	scd.BufferDesc.RefreshRate.Denominator = 1;
	scd.Windowed = true;
	scd.OutputWindow = _w;
	scd.SampleDesc.Count = 1;
	scd.SampleDesc.Quality = 0;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	D3D_DRIVER_TYPE driveTypes[] = { D3D_DRIVER_TYPE_HARDWARE, D3D_DRIVER_TYPE_WARP, D3D_DRIVER_TYPE_REFERENCE };
	UINT flags[] = { D3D11_CREATE_DEVICE_DEBUG, 0 };
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

	if (!_dev)
	{
		MessageBoxA(_w, "Unable to Create D3D11 Device, Update your Graphics Card Driver for DirectX 11 Support!", "DirectX Error", MB_OK | MB_ICONERROR);
		exit(0);
	}
	//assert(_dev, "cannot create directx11 device!");
}

void renderer::init_view()
{
	_ctx->ClearState();
	_back_buf_target = nullptr;
	_back_buf_tex = nullptr;

	_hr = _swap_chain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
	_hr = _swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID *)&_back_buf_tex);
	_hr = _dev->CreateRenderTargetView(_back_buf_tex, nullptr, &_back_buf_target);
	_ctx->OMSetRenderTargets(1, &_back_buf_target, nullptr);

	D3D11_VIEWPORT viewport{};
	auto rc = _w.client_rect();
	viewport.Width = static_cast<float>(rc.width());
	viewport.Height = static_cast<float>(rc.height());
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	_ctx->RSSetViewports(1, &viewport);
}

void renderer::load()
{
	/*
	utility::obj_reader r("f15.obj");
	r.read();

	auto &vt = r.vertices();
	auto &tex = r.textures();

	for (auto f : r.faces())
	{
	for (auto iv : *f)
	{
	auto pv = vt.at(iv->pos - 1);
	vertex v;
	v.pos.x = pv->x;
	v.pos.y = pv->y;
	v.pos.z = pv->z;
	//v.w = pv->w;
	if (iv->tex)
	{
	auto pt = tex.at(iv->tex - 1);
	v.tex.s = pt->u;
	v.tex.t = pt->v;
	}

	_verts.push_back(v);
	}
	}
	*/

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

	_hr = _dev->CreateBuffer(&vbd, &res, &vb);
	vertex *vp = nullptr;
	D3D11_INPUT_ELEMENT_DESC ils[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, (UINT)(&vp->pos), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, (UINT)(&vp->tex), D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	shader_factory sf(_dev);

	sf.set_source(std::ifstream("shader.fx"));
	sf.set_flags1(D3DCOMPILE_DEBUG | D3DCOMPILE_ENABLE_STRICTNESS);
	_hr = sf.create_pixel_shader(&ps, "ps_5_0", "ps_main");
	_hr = sf.create_vertex_shader(&vs, "vs_5_0", "vs_main");

	_hr = sf.create_input_layout(ils, ARRAYSIZE(ils), &layout);

	using namespace DirectX;
	D3D11_BUFFER_DESC cb_desc{};
	cb_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb_desc.ByteWidth = sizeof(XMMATRIX);
	cb_desc.Usage = D3D11_USAGE_DEFAULT;

	_hr = _dev->CreateBuffer(&cb_desc, nullptr, &_cb_world_matrix);
	_hr = _dev->CreateBuffer(&cb_desc, nullptr, &_cb_view_matrix);
	_hr = _dev->CreateBuffer(&cb_desc, nullptr, &_cb_proj_matrix);

	_world_matrix = XMMatrixIdentity();
	_world_matrix = XMMatrixTranspose(_world_matrix);

	_view_matrix = XMMatrixIdentity();
	_view_matrix = XMMatrixTranspose(_view_matrix);

	_w.mouse_press() += [&](window &w, mouse_event_args &e)
	{
		_org.x = e.x();
		_org.y = e.y();
	};

	_w.mouse_wheel() += [&](window &w, mouse_event_args &e)
	{
		_scale.x += (e.z() / e.wheel_delta()) * 0.01f;
		if (_scale.x < 0) _scale.x = 0;
		_scale.y = _scale.x;
		_scale.z = _scale.x;
		std::cout << "scale: " << _scale << std::setw(40) << "\r";
	};

	_w.mouse_move() += [&](window &w, mouse_event_args &e)
	{
		auto dx = e.x() - _org.x;
		auto dy = e.y() - _org.y;
		auto tx = dx / _w.client_rect().width();
		auto ty = dy / _w.client_rect().height();

		switch (e.buttons())
		{
		case LeftButton:
			_rot.x = tx;
			_rot.y = ty;
			break;
		case RightButton:break;
		case MiddleButton:

			_trans.x = tx;
			_trans.y = -ty;
			//std::cout << "translate: " << _trans << std::setw(40) << "\n";
			break;
		}
	};

	texture_helper::create_shader_resource(_dev, "chess.png", &_color_tex);
	D3D11_SAMPLER_DESC sd{};
	sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sd.MaxLOD = D3D11_FLOAT32_MAX;
	_hr = _dev->CreateSamplerState(&sd, &_sampler);
}

void renderer::unload()
{

}
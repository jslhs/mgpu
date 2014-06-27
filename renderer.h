#pragma once

#include "win32.hpp"
#include "window.hpp"
#include "dx11.hpp"

class renderer
	: public renderer_base
{
public:
	renderer(window &w);
	virtual ~renderer();
	void update(float dt = 0.0f) override;
	void render() override;
	void set_full_screen(bool val);

private:
	void init_dev();
	void init_view();
	void load();
	void unload();

private:


private:
	hresult _hr;
	window &_w;
	com_ptr<ID3D11Device> _dev;
	com_ptr<ID3D11DeviceContext> _ctx;
	com_ptr<IDXGISwapChain> _swap_chain;
	com_ptr<ID3D11RenderTargetView> _back_buf_target;
	com_ptr<ID3D11Texture2D> _back_buf_tex;
	D3D_FEATURE_LEVEL _level;

	com_ptr<ID3D11InputLayout> layout;
	com_ptr<ID3D11VertexShader> vs;
	com_ptr<ID3D11PixelShader> ps;
	com_ptr<ID3D11Buffer> vb;
	std::vector<vertex> _verts;

	com_ptr<ID3D11Buffer> _cb_world_matrix;
	com_ptr<ID3D11Buffer> _cb_view_matrix;
	com_ptr<ID3D11Buffer> _cb_proj_matrix;
	DirectX::XMMATRIX _world_matrix;
	DirectX::XMMATRIX _view_matrix;
	DirectX::XMMATRIX _proj_matrix;

	vec2 _org;
	vec3 _trans;
	vec3 _scale;
	vec3 _rot;

	com_ptr<ID3D11SamplerState> _sampler;
	com_ptr<ID3D11ShaderResourceView> _color_tex;

};
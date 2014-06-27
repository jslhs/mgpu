#pragma once

#include <vector>

#include "win32.hpp"
#include "dx11.hpp"

class ImageRenderer
{
public:
	explicit ImageRenderer(HWND hwnd);
	~ImageRenderer();

public:
	void Update(void *buf, unsigned int width, unsigned int height);
	void Render();
	void ResizeBuffer();

private:
	void init();
	void resizeBuffer();
	void createTexture(unsigned int width, unsigned int height);
	
private:
	HWND _hwnd;
	std::vector<vertex> _verts;
	hresult _hr;

	com_ptr<ID3D11Device> _dev;
	com_ptr<ID3D11DeviceContext> _ctx;
	com_ptr<IDXGISwapChain> _swap_chain;
	com_ptr<ID3D11RenderTargetView> _back_buf_target;
	com_ptr<ID3D11Texture2D> _back_buf_tex;
	D3D_FEATURE_LEVEL _level;

	com_ptr<ID3D11InputLayout> _layout;
	com_ptr<ID3D11VertexShader> _vs;
	com_ptr<ID3D11PixelShader> _ps;
	com_ptr<ID3D11Buffer> _vb;
	
	com_ptr<ID3D11SamplerState> _sampler;
	com_ptr<ID3D11Texture2D> _tex;
	com_ptr<ID3D11ShaderResourceView> _tex_view;

};

#pragma once

#include <vector>

#include "win32.hpp"
#include "dx11.hpp"

class ImageRenderer;

class Scene
{
public:
	Scene()
	{
	}

	~Scene()
	{
	}

	void Draw(float x, float y, float rw, float rh, void *img, unsigned int width, unsigned int height)
	{
		Sprite s;
		s.pos = vec2(x, y);
		s.size = vec2(rw, rh);
		s.img = img;
		s.width = width;
		s.height = height;
		_sprites.push_back(s);
	}

	bool IsEmpty() const
	{
		return _sprites.empty();
	}

	void Clear()
	{
		_sprites.clear();
	}

private:
	struct Sprite
	{
		vec2 pos;
		vec2 size;
		void *img;
		unsigned int width;
		unsigned int height;
	};

	friend class ImageRenderer;

private:
	std::vector<Sprite> _sprites;
};

class ImageRenderer
{
public:
	explicit ImageRenderer(HWND hwnd);
	~ImageRenderer();

public:
	//void Update(void *buf, unsigned int width, unsigned int height);
	//void Init(Scene &s);
	void Render(const Scene &s);
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
	com_ptr<ID3D11SamplerState> _sampler;
	com_ptr<ID3D11BlendState> _blend;

	//com_ptr<ID3D11Buffer> _vb;
	//com_ptr<ID3D11Texture2D> _tex;
	//com_ptr<ID3D11ShaderResourceView> _tex_view;
	DirectX::XMMATRIX _proj_m;
	com_ptr<ID3D11Buffer> _cb_proj_m;
};

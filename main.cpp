#include <Windows.h>
#include <dxgi1_2.h>
#include <d3d11.h>

#include <iostream>
#include <vector>
#include <string>

#pragma comment(lib, "dxgi")
#pragma comment(lib, "d3d11")

class window
{
public:
	window(const std::string &title, int x, int y, int width, int height)
		: _hwnd(nullptr)
	{
		create(title, x, y, width, height);
		ShowWindow(_hwnd, SW_SHOW);
		UpdateWindow(_hwnd);
	}

	explicit window(const std::string &title = std::string())
		: window(title, CW_USEDEFAULT, 0, 640, 480)
	{
		
	}

	virtual ~window()
	{

	}

	operator HWND() const
	{
		return _hwnd;
	}

protected:
	virtual LRESULT msgproc(UINT msg, WPARAM wparam, LPARAM lparam)
	{
		switch (msg)
		{
		case WM_CREATE:
			_wcount++;
			break;
		case WM_DESTROY:
			_wcount--;
			if (!_wcount) PostQuitMessage(0);
			break;
		case WM_PAINT:
			{
				PAINTSTRUCT ps;
				auto hdc = BeginPaint(_hwnd, &ps);

				EndPaint(_hwnd, &ps);
			}
			break;
		default:
			return DefWindowProcA(_hwnd, msg, wparam, lparam);
		}

		return 0;
	}

private:
	void create(const std::string &title, int x, int y, int width, int height)
	{
		RECT rc{ x, y, x + width, y + height };
		DWORD style = WS_OVERLAPPEDWINDOW;
		AdjustWindowRect(&rc, style, FALSE);
		width = rc.right - rc.left;
		height = rc.bottom - rc.top;
		_hwnd = CreateWindowA((LPSTR)reg("__sui_window__{5A5C7E7A-9204-49BE-8B53-3D7FFF272967}")
			, title.c_str()
			, style
			, x
			, y
			, width
			, height
			, nullptr
			, nullptr
			, nullptr
			, this);
	}

	ATOM reg(const std::string &cls)
	{
		WNDCLASSEXA wcx{ sizeof(wcx) };
		wcx.lpfnWndProc = winproc;
		wcx.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wcx.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wcx.lpszClassName = cls.c_str();
		//wcx.hInstance = GetModuleHandle(nullptr);
		wcx.style = CS_HREDRAW | CS_VREDRAW;
		return RegisterClassExA(&wcx);
	}

	static LRESULT CALLBACK winproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		if (msg == WM_NCCREATE)
		{
			CREATESTRUCTA *cs = reinterpret_cast<CREATESTRUCTA *>(lparam);
#ifdef _WIN64
			SetWindowLongPtrA(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(cs->lpCreateParams));
#else
			SetWindowLongPtrA(hwnd, GWLP_USERDATA, reinterpret_cast<LONG>(cs->lpCreateParams));
#endif
			//return TRUE;
		}

		window *w = reinterpret_cast<window *>(GetWindowLongPtrA(hwnd, GWLP_USERDATA));
		if (w)
		{
			w->_hwnd = hwnd;
			return w->msgproc(msg, wparam, lparam);
		}
		
		return DefWindowProcA(hwnd, msg, wparam, lparam);
	}

private:
	static int _wcount;
	HWND _hwnd;
};

int window::_wcount;

class application
{
public:
	int run()
	{
		MSG msg;
		while (GetMessage(&msg, nullptr, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		return (msg.message == WM_QUIT) ? msg.wParam : 0;
	}
};

template<class T>
class com_ptr
{
public:
	com_ptr()
		: _ptr(nullptr)
	{

	}

	com_ptr(T *ptr)
		: _ptr(ptr)
	{

	}

	com_ptr(T **ptr)
		: _ptr(nullptr)
	{
		if (ptr)
		{
			_ptr = *ptr;
			*ptr = nullptr;
		}
	}

	~com_ptr()
	{
		if (_ptr) _ptr->Release();
		_ptr = nullptr;
	}

	com_ptr(const com_ptr &) = delete;
	com_ptr &operator=(const com_ptr &) = delete;

	com_ptr(com_ptr &&ptr)
	{
		if (&ptr != this)
		{
			std::swap(_ptr, ptr._ptr);
		}
	}

	com_ptr &operator=(com_ptr &&ptr)
	{
		if (&ptr != this)
		{
			std::swap(_ptr, ptr._ptr);
		}

		return *this;
	}

	T *get()
	{
		return _ptr;
	}

	T *operator->()
	{
		return _ptr;
	}

	T **operator&()
	{
		return &_ptr;
	}

private:
	T *_ptr;
};

int main()
{
	application app;
	window w("mgpu");
	IDXGIFactory2 *pDXGIFactory = nullptr;
	com_ptr<IDXGIFactory2> fact;
	auto hr = CreateDXGIFactory1(__uuidof(IDXGIFactory2), (void **)&pDXGIFactory);
	if (FAILED(hr)) std::cout << "unable to create IDXGIFactory2!" << std::endl;

	UINT i = 0;
	IDXGIAdapter2 *pAdapter;
	ID3D11Device *pDevice;
	std::vector<IDXGIAdapter2 *> adapters;
	std::vector<ID3D11Device *> devices;
	while (pDXGIFactory->EnumAdapters1(i++, (IDXGIAdapter1 **)&pAdapter) != DXGI_ERROR_NOT_FOUND)
	{
		adapters.push_back(pAdapter);
		DXGI_ADAPTER_DESC2 desc;
		if (SUCCEEDED(pAdapter->GetDesc2(&desc)))
		{
			if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) continue; // skip software adapter
			std::cout << "Adapter[" << i << "]: ";
			std::wcout << desc.Description << std::endl;
		}
	}

	pAdapter = adapters.front();

	D3D_FEATURE_LEVEL level;
	ID3D11DeviceContext *ctx;
	com_ptr<ID3D11DeviceContext> ctx1;
	D3D11CreateDevice(pAdapter, D3D_DRIVER_TYPE_UNKNOWN, nullptr, D3D11_CREATE_DEVICE_DEBUG, 0, 0, D3D11_SDK_VERSION, &pDevice, &level, &ctx);

	DXGI_SWAP_CHAIN_DESC sc{};
	RECT rc;
	GetClientRect(w, &rc);
	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;
	sc.BufferCount = 1;
	sc.BufferDesc.Width = width;
	sc.BufferDesc.Height = height;
	sc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sc.BufferDesc.RefreshRate.Numerator = 60;
	sc.BufferDesc.RefreshRate.Denominator = 1;
	sc.Windowed = true;
	sc.OutputWindow = w;
	sc.SampleDesc.Count = 1;
	sc.SampleDesc.Quality = 0;
	sc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	IDXGISwapChain *pSwapChain;
	hr = pDXGIFactory->CreateSwapChain(pDevice, &sc, &pSwapChain);
	auto err = GetLastError();

	ID3D11RenderTargetView *back_buf_target;
	ID3D11Texture2D *back_buf_tex;
	hr = pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID *)&back_buf_tex);
	hr = pDevice->CreateRenderTargetView(back_buf_tex, nullptr, &back_buf_target);
	back_buf_tex->Release();

	ctx->OMSetRenderTargets(1, &back_buf_target, nullptr);

	D3D11_VIEWPORT vp;
	vp.Width = static_cast<float>(width);
	vp.Height = static_cast<float>(height);
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;

	ctx->RSSetViewports(1, &vp);

	FLOAT color[] = {0.75f, 0.75f, 0.75f};
	ctx->ClearRenderTargetView(back_buf_target, color);

	pSwapChain->Present(0, 0);
	//pDevice->Release();
	//pAdapter->Release();
	//pDXGIFactory->Release();

	return app.run();
}

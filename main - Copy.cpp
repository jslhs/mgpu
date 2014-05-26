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
		if (msg == WM_CREATE) _wcount++;
		if (msg == WM_DESTROY)
		{
			_wcount--;
			if (!_wcount) PostQuitMessage(0);
		}
		return DefWindowProcA(_hwnd, msg, wparam, lparam);
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
			SetWindowLongPtrA(hwnd, GWLP_USERDATA, (LONG)cs->lpCreateParams);
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

int main()
{
	application app;
	window w("mgpu");
	IDXGIFactory2 *pDXGIFactory = nullptr;
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
	D3D11CreateDevice(pAdapter, D3D_DRIVER_TYPE_UNKNOWN, nullptr, 0, 0, 0, D3D11_SDK_VERSION, &pDevice, &level, 0);

	DXGI_SWAP_CHAIN_DESC scdesc{};
	scdesc.BufferCount = 2;
	IDXGISwapChain *pSwapChain;
	pDXGIFactory->CreateSwapChain(pDevice, &scdesc, &pSwapChain);

	auto c = app.run();

	pDevice->Release();
	pAdapter->Release();
	pDXGIFactory->Release();

	return 0;
}

#include <Windows.h>
#include <dxgi1_2.h>
#include <d3d11.h>

#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <functional>
#include <algorithm>
#include <map>

#pragma comment(lib, "dxgi")
#pragma comment(lib, "d3d11")

std::string get_win32_error_string(DWORD code, DWORD lang = MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT))
{
	char *buf = nullptr;
	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER
		| FORMAT_MESSAGE_FROM_SYSTEM
		| FORMAT_MESSAGE_IGNORE_INSERTS
		, nullptr
		, code
		, lang
		, (LPSTR)&buf
		, 0
		, nullptr);

	std::string msg;
	if (buf)
	{
		msg = buf;
		LocalFree(buf);
	}

	return msg;
}

class window_class
{
public:
	window_class(const std::string &name, WNDPROC winproc)
		: _name(name)
	{
		WNDCLASSEXA wcx{ sizeof(wcx) };
		wcx.lpfnWndProc = winproc;
		wcx.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wcx.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wcx.lpszClassName = name.c_str();
		wcx.style = CS_HREDRAW | CS_VREDRAW;
		_atom = RegisterClassExA(&wcx);
	}

	~window_class()
	{
		UnregisterClassA(_name.c_str(), nullptr);
	}

	operator LPSTR()
	{
		return (LPSTR)_atom;
	}

private:
	std::string _name;
	ATOM _atom;
};

class window;
class rect
{

};

template<class>
class delegate;

template<class... Args>
class delegate<void(Args...)>
{
public:
	typedef std::function<void(Args...)> handler_type;

	delegate &operator=(const handler_type& handler)
	{
		_handlers.clear();
		_handlers.push_back(handler);
		return *this;
	}

	delegate &operator+=(const handler_type& handler)
	{
		_handlers.push_back(handler);
		return *this;
	}

	void operator()(Args... args)
	{
		for (auto &handler : _handlers) if (handler) handler(args...);
	}

private:
	std::vector<handler_type> _handlers;
};

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
		DestroyWindow(_hwnd);
	}

	operator HWND() const
	{
		return _hwnd;
	}

	using will_resize_delegate = delegate < void(UINT, RECT *) >;
	using did_resize_delegate = delegate < void(UINT, int width, int height) >;

	will_resize_delegate &will_resize()
	{
		return _dlgt_will_resize;
	}

	did_resize_delegate &did_resize()
	{
		return _dlgt_did_resize;
	}

	void update();


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
				paint(hdc, ps);
				EndPaint(_hwnd, &ps);
			}
			break;
		case WM_SIZE:
			_dlgt_did_resize(wparam, LOWORD(lparam), HIWORD(lparam));
			return TRUE;
		case WM_SIZING:
			_dlgt_will_resize(wparam, reinterpret_cast<RECT*>(lparam));
			return TRUE;
		default:
			return DefWindowProcA(_hwnd, msg, wparam, lparam);
		}

		return 0;
	}

	virtual void paint(HDC hdc, const PAINTSTRUCT &ps)
	{

	}

private:
	void create(const std::string &title, int x, int y, int width, int height)
	{
		RECT rc{ x, y, x + width, y + height };
		DWORD style = WS_OVERLAPPEDWINDOW;
		AdjustWindowRect(&rc, style, FALSE);
		width = rc.right - rc.left;
		height = rc.bottom - rc.top;
		_hwnd = CreateWindowA(_cls
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
	using msg_translator = std::function < LRESULT(WPARAM, LPARAM) > ;
	std::map<UINT, msg_translator> _msg_trans;
	static int _wcount;
	static window_class _cls;
	HWND _hwnd;
	will_resize_delegate _dlgt_will_resize;
	did_resize_delegate _dlgt_did_resize;
};

int window::_wcount;
window_class window::_cls("__sui_window__{5A5C7E7A-9204-49BE-8B53-3D7FFF272967}", window::winproc);

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
class com_ptr_default_delete
{
public:
	com_ptr_default_delete(bool del = true)
		: _delete(del)
	{

	}

	void operator()(T *ptr)
	{
		if (_delete) ptr->Release();
	}

private:
	bool _delete;
};

template<class T, class D = com_ptr_default_delete<T>>
class com_ptr
{
public:
	typedef T *pointer;
	typedef typename D deleter;

	com_ptr()
		: _ptr(nullptr)
	{

	}

	com_ptr(nullptr_t)
		: _ptr(nullptr)
	{

	}

	explicit com_ptr(pointer ptr)
		: _ptr(ptr)
	{

	}

	explicit com_ptr(pointer* ptr)
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
		reset(nullptr);
	}

	com_ptr(const com_ptr &) = delete;
	com_ptr &operator=(const com_ptr &) = delete;

	com_ptr &operator=(nullptr_t)
	{
		reset(nullptr);
		return *this;
	}

	com_ptr(com_ptr&& ptr)
		: _ptr(nullptr)
	{
		if (ptr._ptr != _ptr)
		{
			_ptr = ptr._ptr;
			_del = ptr._del;
			ptr._ptr = nullptr;
		}
	}

	com_ptr &operator=(com_ptr&& ptr)
	{
		if (ptr._ptr != _ptr)
		{
			reset(nullptr);
			_ptr = ptr._ptr;
			_del = ptr._del;
			ptr._ptr = nullptr;
		}

		return *this;
	}

	pointer get()
	{
		return _ptr;
	}

	pointer operator->()
	{
		return _ptr;
	}

	T &operator*()
	{
		return *_ptr;
	}

	pointer* operator&()
	{
		return &_ptr;
	}

	bool operator==(const com_ptr &ptr)
	{
		return _ptr == ptr._ptr;
	}

	bool operator!=(const com_ptr &ptr)
	{
		return !(*this == ptr);
	}

	operator pointer()
	{
		return _ptr;
	}

	deleter &get_deleter()
	{
		return _del;
	}

	void reset(pointer ptr)
	{
		if (_ptr) _del(_ptr);
		_ptr = ptr;
	}

	pointer release()
	{
		auto ptr = _ptr;
		_ptr = nullptr;
		return ptr;
	}

private:
	pointer _ptr;
	deleter _del;
};

class renderer_base
{
public:
	virtual void update(float dt) = 0;
	virtual void render() = 0;
};

class scene;
class texture;
class material;
class light;
class shader;

class scene
{
public:
	void apply(const shader &s);
};

class renderer
	: public renderer_base
{
public:
	renderer(window &w)
		: _w(w)
	{

	}

	virtual ~renderer();

	void update(float dt = 0.0f) override
	{

	}

	void render() override
	{

	}

private:
	window &_w;
};

class off_screen_renderer
	: public renderer_base
{
public:

};

class run_loop
{
public:
	typedef std::function<void()> task;

	run_loop();
	~run_loop();

	run_loop(const run_loop &) = delete;
	run_loop &operator=(const run_loop &) = delete;

	void run()
	{
		MSG msg;
		while (msg.message != WM_QUIT)
		{
			if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				for (auto &t : _tasks) t();
			}
		}
	}

	size_t add(task t)
	{
		_tasks.push_back(t);
		return _tasks.size() - 1;
	}

	void remove(size_t t)
	{
		if (t >= _tasks.size()) return;
		auto it = _tasks.begin();
		std::advance(it, t);
		_tasks.erase(it);
	}

private:
	std::vector<task> _tasks;
};

class hresult
{
public:
	hresult()
		: _hr(0)
	{

	}

	hresult(HRESULT hr)
	{
		*this = hr;
	}

	hresult &operator=(HRESULT hr)
	{
		_hr = hr;
#ifdef _DEBUG
		_err = get_win32_error_string(_hr);
#endif
		return *this;
	}

	operator HRESULT()
	{
		return _hr;
	}

	std::string error_string() const
	{
		return get_win32_error_string(_hr);
	}
	
private:
#ifdef _DEBUG
	std::string _err;
#endif
	HRESULT _hr;
};

std::ostream &operator<<(std::ostream &os, const hresult &hr)
{
	return os << hr.error_string();
}

int main()
{
	application app;
	window w("mgpu");
	window w0("demo");
	com_ptr<IDXGIFactory2> dxgi_factory;
	hresult hr = CreateDXGIFactory1(__uuidof(IDXGIFactory2), (void **)&dxgi_factory);
	if (FAILED(hr)) std::cout << "unable to create IDXGIFactory2!" << std::endl;
	std::vector<com_ptr<IDXGIAdapter2>> adapters;
	std::vector<com_ptr<ID3D11Device>> devices;
	UINT i = 0;
	{
		com_ptr<IDXGIAdapter2> adapter;
		
		while (dxgi_factory->EnumAdapters1(i++, (IDXGIAdapter1 **)&adapter) != DXGI_ERROR_NOT_FOUND)
		{
			DXGI_ADAPTER_DESC2 desc;
			if (SUCCEEDED(adapter->GetDesc2(&desc)))
			{
				//if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) continue; // skip software adapter
				std::cout << "Adapter[" << i << "]: ";
				std::wcout << desc.Description << std::endl;
			}

			adapters.push_back(std::move(adapter));
		}
	}

	auto &adapter = adapters.front();
	com_ptr<ID3D11Device> device;

	D3D_FEATURE_LEVEL level;
	com_ptr<ID3D11DeviceContext> ctx;
	D3D11CreateDevice(adapter, D3D_DRIVER_TYPE_UNKNOWN, nullptr, D3D11_CREATE_DEVICE_DEBUG, 0, 0, D3D11_SDK_VERSION, &device, &level, &ctx);

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

	com_ptr<IDXGISwapChain> swap_chain;
	hr = dxgi_factory->CreateSwapChain(device, &sc, &swap_chain);
	std::cout << "create swap chain: " << hr << std::endl;

	w.did_resize() += [&swap_chain](UINT flags, int width, int height){
		//swap_chain->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);
		std::cout << "width = " << width << ", height = " << height << std::endl;
	};

	//swap_chain->SetFullscreenState(TRUE, nullptr);
	//swap_chain->ResizeBuffers(1, 1920, 1080, DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);

	com_ptr<ID3D11RenderTargetView>back_buf_target;
	com_ptr<ID3D11Texture2D> back_buf_tex;
	hr = swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID *)&back_buf_tex);
	hr = device->CreateRenderTargetView(back_buf_tex, nullptr, &back_buf_target);

	ctx->OMSetRenderTargets(1, &back_buf_target, nullptr);

	D3D11_VIEWPORT viewport;
	viewport.Width = static_cast<float>(width);
	viewport.Height = static_cast<float>(height);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	ctx->RSSetViewports(1, &viewport);

	float color[] = {0.75f, 0.75f, 0.75f};
	ctx->ClearRenderTargetView(back_buf_target, color);

	swap_chain->Present(0, 0);
	return app.run();
}

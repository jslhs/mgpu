#include "obj_reader.h"

#include <Windows.h>
#include <windowsx.h>
#include <dxgi1_2.h>
#include <d3d11.h>
#include <io.h>
#include <fcntl.h>
#include <d3dcompiler.h>

#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <functional>
#include <algorithm>
#include <map>
#include <fstream>

#pragma comment(lib, "dxgi")
#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")

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

	operator HRESULT() const
	{
		return _hr;
	}

	std::string error_string() const
	{
		return get_win32_error_string(_hr);
	}

	bool failed() const
	{
		return FAILED(_hr) == TRUE;
	}

	bool succeeded() const
	{
		return SUCCEEDED(_hr) == TRUE;
	}

private:
#ifdef _DEBUG
	std::string _err;
#endif
	HRESULT _hr;
};

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
public:
	rect()
		: _rc{}
	{

	}

	rect(const RECT &rc)
		: _rc(rc)
	{

	}

	rect &operator=(const RECT &rc)
	{
		_rc = rc;
	}

	operator RECT&()
	{
		return _rc;
	}

	int width() const
	{
		return _rc.right - _rc.left;
	}

	int height() const
	{
		return _rc.bottom - _rc.top;
	}

	void set_width(int width)
	{
		_rc.right = _rc.left + width;
	}

	void set_height(int height)
	{
		_rc.bottom = _rc.top + height;
	}

	long &x()
	{
		return _rc.left;
	}

	long &y()
	{
		return _rc.top;
	}

	RECT &rc()
	{
		return _rc;
	}

private:
	RECT _rc;
};

class event_args
{
public:
	event_args()
		: _wparam(0)
		, _lparam(0)
	{

	}

	event_args(WPARAM wparam, LPARAM lparam)
		: _wparam(wparam)
		, _lparam(lparam)
	{

	}

	WPARAM wparam() const
	{
		return _wparam;
	}

	LPARAM lparam() const
	{
		return _lparam;
	}
	
private:
	WPARAM _wparam;
	LPARAM _lparam;
};

class size_event_args
	: public event_args
{
public:
	size_event_args(WPARAM wparam, LPARAM lparam)
		: event_args(wparam, lparam)
		, _type(wparam)
		, _width(LOWORD(lparam))
		, _height(HIWORD(lparam))
	{

	}

	int type() const
	{
		return _type;
	}

	int width() const
	{
		return _width;
	}

	int height() const
	{
		return _height;
	}

private:
	int _type;
	int _width;
	int _height;
};

class sizing_event_args
	: public event_args
{
public:
	sizing_event_args(WPARAM wparam, LPARAM lparam)
		: event_args(wparam, lparam)
		, _edge(wparam)
		, _rc(*reinterpret_cast<RECT*>(lparam))
	{
		
	}

	rect &rc()
	{
		return _rc;
	}

	int edge() const
	{
		return _edge;
	}

private:
	rect _rc;
	int _edge;
};

class cancel_event_args : public event_args
{
public:
	cancel_event_args(WPARAM wparam, LPARAM lparam)
		: event_args(wparam, lparam)
	{

	}

	void cancel()
	{
		_canceled = true;
	}

	bool canceled()
	{
		return _canceled;
	}

	void set_canceled(bool val)
	{
		_canceled = val;
	}

private:
	bool _canceled = false;
};

enum MouseButtons
{
	NoMouseButton,
	LeftButton = 1 << 0,
	RightButton = 1 << 1,
	MiddleButton = 1 << 2,
};

enum KeyModifiers
{
	NoKeyModifier,
	ShiftKey = 1 << 0,
	CtrlKey = 1 << 1,
	AltKey = 1 << 2,
};

class mouse_event_args : public event_args
{
public:
	mouse_event_args(WPARAM wparam, LPARAM lparam)
		: event_args(wparam, lparam)
		, _x(GET_X_LPARAM(lparam))
		, _y(GET_X_LPARAM(lparam))
		, _z(GET_WHEEL_DELTA_WPARAM(wparam))
	{
		int b = 0;
		int m = 0;
		if (wparam & MK_LBUTTON)
			b |= LeftButton;
		if (wparam & MK_RBUTTON)
			b |= RightButton;
		if (wparam & MK_MBUTTON)
			b |= MiddleButton;
		if (wparam & MK_CONTROL)
			m |= CtrlKey;
		if (wparam & MK_SHIFT)
			m |= ShiftKey;
		if (GetKeyState(VK_MENU) < 0)
			m |= AltKey;

		_btns = static_cast<MouseButtons>(b);
		_km = static_cast<KeyModifiers>(m);
	}

	int x() const
	{
		return _x;
	}

	int y() const
	{
		return _y;
	}

	int z() const
	{
		return _z;
	}

	static int wheel_delta()
	{
		return WHEEL_DELTA;
	}

	MouseButtons buttons() const
	{
		return _btns;
	}

	KeyModifiers modifiers() const
	{
		return _km;
	}

private:
	int _x;
	int _y;
	int _z;
	MouseButtons _btns;
	KeyModifiers _km;
};

//template<class>
//class event;

//template<class R, class... Args>
//class event<R(Args...)>

template<class... Args>
class event
{
public:
	using delegate = std::function < void(Args...) > ;

	event &operator=(const delegate& handler)
	{
		_handlers.clear();
		_handlers.push_back(handler);
		return *this;
	}

	event &operator+=(const delegate& handler)
	{
		_handlers.push_back(handler);
		return *this;
	}

	bool operator== (nullptr_t)
	{
		return _handlers.size() == 0;
	}

	bool operator!= (nullptr_t)
	{
		return _handlers.size() != 0;
	}

	operator bool()
	{
		_handlers.size() > 0;
	}

	bool operator()(Args... args)
	{
		bool r = (_handlers.size() != 0);
		for (auto &handler : _handlers) if (handler) handler(args...);
		return r;
	}

private:
	std::vector<delegate> _handlers;
};

// for win32 gui app
class console
{
protected:
	console()
	{
		AllocConsole();
		SetConsoleCtrlHandler(ctrl_handler, TRUE);
	}

public:
	static console *instance()
	{
		if (!_inst)
		{
			_inst = std::unique_ptr<console>(new console());
		}

		return _inst.get();
	}

	static void destroy()
	{
		_inst.reset();
	}

	~console()
	{
		SetConsoleCtrlHandler(ctrl_handler, FALSE);
		FreeConsole();
	}

	void redirect()
	{
		auto redir = [](DWORD from_handle, FILE *to_handle, char *mode){
			auto std_handle = GetStdHandle(from_handle);
			auto con_handle = _open_osfhandle((intptr_t)std_handle, _O_TEXT);
			auto fp = _fdopen(con_handle, mode);
			*to_handle = *fp;
			setvbuf(to_handle, nullptr, _IONBF, 0);
		};

		redir(STD_OUTPUT_HANDLE, stdout, "w"); // redirect stdout
		redir(STD_INPUT_HANDLE, stdin, "r"); // redirect stdin
		redir(STD_ERROR_HANDLE, stderr, "w"); // redirect stderr

		// make std::cout, std::cin, std::cerr, std::wcin, std::wcout, std::wcerr, 
		// std::clog and std::wclog point to console as well
		std::ios::sync_with_stdio();
	}

	using ctrl_c_event = event <event_args &>;
	using ctrl_break_event = event < event_args & > ;
	using close_event = event < event_args & > ;
	using logoff_event = event < event_args & > ;
	using shutdown_event = event < event_args & > ;

	ctrl_c_event &ctrl_c_pressed()
	{
		return _ctrl_c;
	}

	ctrl_break_event &ctrl_break_pressed()
	{
		return _ctrl_break;
	}

	close_event &will_close()
	{
		return _close;
	}

	logoff_event &will_logoff()
	{
		return _logoff;
	}

	shutdown_event &will_shutdown()
	{
		return _shutdown;
	}

private:
	static BOOL WINAPI ctrl_handler(DWORD ctrl_type)
	{
		event_args args;
		bool r = false;
		switch (ctrl_type)
		{
		case CTRL_C_EVENT:
			r = _inst->_ctrl_c(args);
			break;
		case CTRL_BREAK_EVENT:
			r = _inst->_ctrl_break(args);
			break;
		case CTRL_CLOSE_EVENT:
			r = _inst->_close(args);
			break;
		case CTRL_LOGOFF_EVENT:
			r = _inst->_logoff(args);
			break;
		case CTRL_SHUTDOWN_EVENT:
			r = _inst->_shutdown(args);
			break;
		}

		return r ? TRUE : FALSE;
	}

	ctrl_c_event _ctrl_c;
	ctrl_break_event _ctrl_break;
	close_event _close;
	logoff_event _logoff;
	shutdown_event _shutdown;

	static std::unique_ptr<console> _inst;
};

std::unique_ptr<console> console::_inst;

class window
{
public:
	window(const std::string &title, int x, int y, int width, int height)
		: _hwnd(nullptr)
	{
		create(title, x, y, width, height);
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

	void close()
	{
		CloseWindow(_hwnd);
	}

	void show()
	{
		ShowWindow(_hwnd, SW_SHOW);
	}

	using will_resize_event = event <window&, sizing_event_args &>;
	using did_resize_event = event < window&, size_event_args & > ;
	using will_close_event = event < window&, cancel_event_args & > ;
	using did_close_event = event < window&, event_args & > ;
	using mouse_move_event = event < window &, mouse_event_args & > ;
	using mouse_press_event = event < window &, mouse_event_args & > ;
	using mouse_release_event = event < window &, mouse_event_args & >;
	using mouse_dbclick_event = event < window &, mouse_event_args & > ;
	using mouse_wheel_event = event < window &, mouse_event_args & > ;

	will_resize_event &will_resize()
	{
		return _will_resize;
	}

	did_resize_event &did_resize()
	{
		return _did_resize;
	}

	will_close_event &will_close()
	{
		return _will_close;
	}

	did_close_event &did_close()
	{
		return _did_close;
	}

	mouse_move_event &mouse_move()
	{
		return _mouse_move;
	}

	mouse_press_event &mouse_press()
	{
		return _mouse_press;
	}

	mouse_release_event &mouse_release()
	{
		return _mouse_release;
	}

	mouse_dbclick_event &mouse_dbclick()
	{
		return _mouse_dbclick;
	}

	mouse_wheel_event &mouse_wheel()
	{
		return _mouse_wheel;
	}

	void update()
	{
		UpdateWindow(_hwnd);
	}

	rect client_rect() const
	{
		RECT rc;
		GetClientRect(_hwnd, &rc);
		return rc;
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
		{
			event_args args;
			_did_close(*this, args);
			_wcount--;
			if (!_wcount) PostQuitMessage(0);
		}	
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
		{
			size_event_args args(wparam, lparam);
			_did_resize(*this, args);
			return TRUE;
		}
			break;
		case WM_SIZING:
		{
			sizing_event_args args(wparam, lparam);
			if (_will_resize(*this, args)) return TRUE;
		}
			break;
		case WM_CLOSE:
		{
			cancel_event_args args(wparam, lparam);
			if (_will_close(*this, args))
			{
				if (!args.canceled()) DestroyWindow(_hwnd);
				return 0;
			}
		}
			break;
		case WM_MOUSEMOVE:
		{
			mouse_event_args args(wparam, lparam);
			if (_mouse_move(*this, args)) return 0;
		}
			break;
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_MBUTTONDOWN:
		{
			mouse_event_args args(wparam, lparam);
			if (_mouse_press(*this, args)) return 0;
		}
			break;
		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
		case WM_MBUTTONUP:
		{
			mouse_event_args args(wparam, lparam);
			if (_mouse_release(*this, args)) return 0;
		}
			break;
		case WM_LBUTTONDBLCLK:
		case WM_RBUTTONDBLCLK:
		case WM_MBUTTONDBLCLK:
		{
			mouse_event_args args(wparam, lparam);
			if (_mouse_dbclick(*this, args)) return 0;
		}
			break;
		case WM_MOUSEWHEEL:
		{
			mouse_event_args args(wparam, lparam);
			if (_mouse_wheel(*this, args)) return 0;
		}
			break;
		}
		
		return DefWindowProcA(_hwnd, msg, wparam, lparam);
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
	//using msg_translator = std::function < LRESULT(WPARAM, LPARAM) > ;
	//std::map<UINT, msg_translator> _msg_trans;
	static int _wcount;
	static window_class _cls;
	HWND _hwnd;
	will_resize_event _will_resize;
	did_resize_event _did_resize;
	will_close_event _will_close;
	did_close_event _did_close;
	mouse_move_event _mouse_move;
	mouse_press_event _mouse_press;
	mouse_release_event _mouse_release;
	mouse_dbclick_event _mouse_dbclick;
	mouse_wheel_event _mouse_wheel;
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

template<size_t N, class T>
struct vec_t;

template<class T>
struct vec_t<2, T>
{
	union
	{
		struct
		{
			T x, y;
		};
		struct
		{
			T s, t;
		};
		struct
		{
			T u, v;
		};
		T data[2];
	};
};

template<class T>
struct vec_t<3, T>
{
	union
	{
		struct
		{
			T x, y, z;
		};
		struct
		{
			T r, g, b;
		};
		struct
		{
			T u, v, w;
		};
		struct
		{
			T i, j, k;
		};
		T data[3];
	};
};

template<class T>
struct vec_t<4, T>
{
	union
	{
		struct
		{
			T x, y, z, w;
		};
		struct
		{
			T r, g, b, a;
		};
		T data[4];
	};
};

using vec2 = vec_t < 2, float > ;
using vec3 = vec_t < 3, float > ;
using vec4 = vec_t < 4, float > ;

struct vertex
{
	vec3 pos;
	//vec4 color;
	//vec3 norm;
	//vec2 uv;
};

class shader_factory
{
public:
	shader_factory(com_ptr<ID3D11Device> &dev)
		: _dev(dev)
		, _flags1(0)
		, _flags2(0)
		, _defs(nullptr)
		, _inc(nullptr)
		, _dirty(true)
	{

	}

	~shader_factory()
	{

	}

	hresult create_vertex_shader(ID3D11VertexShader **s, const std::string &target = "", const std::string &entry_point = "", ID3D11ClassLinkage *link = nullptr)
	{
		compile(entry_point.empty() ? _entry : entry_point, target.empty() ? _target : target);
		if (_hr.succeeded())
		{
			_hr = _dev->CreateVertexShader(_bin->GetBufferPointer(), _bin->GetBufferSize(), link, s);
		}
		return _hr;
	}

	hresult create_pixel_shader(ID3D11PixelShader **s, const std::string &target = "", const std::string &entry_point = "", ID3D11ClassLinkage *link = nullptr)
	{
		compile(entry_point.empty() ? _entry : entry_point, target.empty() ? _target : target);
		if (_hr.succeeded())
		{
			_hr = _dev->CreatePixelShader(_bin->GetBufferPointer(), _bin->GetBufferSize(), link, s);
		}
		return _hr;
	}

	hresult create_compute_shader(ID3D11ComputeShader **s, const std::string &target = "", const std::string &entry_point = "", ID3D11ClassLinkage *link = nullptr)
	{
		compile(entry_point.empty() ? _entry : entry_point, target.empty() ? _target : target);
		if (_hr.succeeded())
		{
			_hr = _dev->CreateComputeShader(_bin->GetBufferPointer(), _bin->GetBufferSize(), link, s);
		}
		return _hr;
	}

	hresult create_geometry_shader(ID3D11GeometryShader **s, const std::string &target = "", const std::string &entry_point = "", ID3D11ClassLinkage *link = nullptr)
	{
		compile(entry_point.empty() ? _entry : entry_point, target.empty() ? _target : target);
		if (_hr.succeeded())
		{
			_hr = _dev->CreateGeometryShader(_bin->GetBufferPointer(), _bin->GetBufferSize(), link, s);
		}
		return _hr;
	}

	hresult create_input_layout(const D3D11_INPUT_ELEMENT_DESC *desc, UINT count, ID3D11InputLayout **layout)
	{
		if (_bin)
		{
			_hr = _dev->CreateInputLayout(desc, count, _bin->GetBufferPointer(), _bin->GetBufferSize(), layout);
		}

		return _hr;
	}

	void set_source(const std::string &src)
	{
		_src = src;
		_dirty = true;
	}

	void set_source(std::istream &file)
	{
		_src = std::string{ std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>() };
		_dirty = true;
	}

	void set_flags1(UINT flags)
	{
		_flags1 = flags;
		_dirty = true;
	}

	void set_flags2(UINT flags)
	{
		_flags2 = flags;
		_dirty = true;
	}

	UINT flags1() const
	{
		return _flags1;
	}

	UINT flags2() const
	{
		return _flags2;
	}

	UINT &flags1()
	{
		_dirty = true;
		return _flags1;
	}

	UINT &flags2()
	{
		_dirty = true;
		return _flags2;
	}

	void set_entry_point(const std::string &entry)
	{
		_entry = entry;
		_dirty = true;
	}

	void set_target(const std::string &target)
	{
		_target = target;
		_dirty = true;
	}

	void set_defines(const D3D_SHADER_MACRO *defs)
	{
		_defs = defs;
		_dirty = true;
	}

	void set_includes(ID3DInclude *inc)
	{
		_inc = inc;
		_dirty = true;
	}

private:
	void compile(const std::string &entry, const std::string &target)
	{
		if (_src.empty()) return;
		if (entry != _entry || target != _target) _dirty = true;
		if (!_dirty) return;
		
		_bin = nullptr;
		_msg = nullptr;

		_hr = D3DCompile(&_src[0], _src.size(), nullptr, _defs, _inc, entry.c_str(), target.c_str(), _flags1, _flags2, &_bin, &_msg);
		_dirty = false;
	}

private:
	com_ptr<ID3D11Device> &_dev;
	std::string _src;
	UINT _flags1;
	UINT _flags2;
	std::string _entry;
	std::string _target;
	const D3D_SHADER_MACRO *_defs;
	ID3DInclude *_inc;
	com_ptr<ID3DBlob> _bin;
	com_ptr<ID3DBlob> _msg;
	hresult _hr;
	bool _dirty;
};

class renderer
	: public renderer_base
{
public:
	renderer(window &w)
		: _w(w)
	{
		init_dev();
		_w.did_resize() += [&](window&, size_event_args &e){
			std::cout << "width = " << e.width() << ", height = " << e.height() << std::endl;
			if (e.type() != SIZE_MINIMIZED && e.type() != SIZE_MAXHIDE)
				init_view();
		};

		load();
	}

	virtual ~renderer()
	{
		//_dev->CreateVertexShader()
	}

	void update(float dt = 0.0f) override
	{

	}

	void render() override
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

		_ctx->Draw(_verts.size(), 0);
		_hr = _swap_chain->Present(0, 0);
	}

	void set_full_screen(bool val)
	{
		_swap_chain->SetFullscreenState(val ? TRUE : FALSE, nullptr);
	}

private:
	void init_dev()
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
		scd.SampleDesc.Count = 4;
		scd.SampleDesc.Quality = 0;
		scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		_hr = D3D11CreateDeviceAndSwapChain(nullptr
			, D3D_DRIVER_TYPE_HARDWARE
			, nullptr
			, D3D11_CREATE_DEVICE_DEBUG
			, nullptr
			, 0
			, D3D11_SDK_VERSION
			, &scd
			, &_swap_chain
			, &_dev
			, &_level
			, &_ctx);
	}

	void init_view()
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

	void load()
	{
		utility::obj_reader r("f15.obj");
		r.read();

		auto &vt = r.vertices();
		
		for (auto f : r.faces())
		{
			for (auto iv : *f)
			{
				auto pv = vt.at(iv->pos - 1);
				vec3 v;
				v.x = pv->x * 0.2;
				v.y = pv->y * 0.2;
				v.z = pv->z * 0.2;
				//v.w = pv->w;
				_verts.push_back(v);
			}
		}

		//std::cout << "vertex = " << sizeof(vertex) << std::endl;
		//vertex v[3]
		//{
		//	vec3{ 0.5f, 0.5f, 0.5f },
		//	vec3{ 0.5f, -0.5f, 0.5f },
		//	vec3{ -0.5f, -0.5f, 0.5f }
		//};

		D3D11_BUFFER_DESC vbd{};
		vbd.Usage = D3D11_USAGE_DEFAULT;
		vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbd.ByteWidth = sizeof(vec3) * _verts.size();

		D3D11_SUBRESOURCE_DATA res{};
		res.pSysMem = &_verts[0];

		_hr = _dev->CreateBuffer(&vbd, &res, &vb);

		D3D11_INPUT_ELEMENT_DESC posd{};
		posd.SemanticName = "POSITION";
		posd.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		
		shader_factory sf(_dev);
		
		sf.set_source(std::ifstream("shader.fx"));
		sf.set_flags1(D3DCOMPILE_DEBUG | D3DCOMPILE_ENABLE_STRICTNESS);
		_hr = sf.create_pixel_shader(&ps, "ps_5_0", "ps_main");
		_hr = sf.create_vertex_shader(&vs, "vs_5_0", "vs_main");

		_hr = sf.create_input_layout(&posd, 1, &layout);
		
		_w.mouse_press() += [&](window &w, mouse_event_args &e)
		{

		};

		_w.mouse_wheel() += [&](window &w, mouse_event_args &e)
		{
			_scale += e.z() / e.wheel_delta();
			scale(_scale);
		};

		_w.mouse_move() += [&](window &w, mouse_event_args &e)
		{
			switch (e.buttons())
			{
			case LeftButton:break;
			case RightButton:break;
			case MiddleButton:
				translate(e.x(), e.y());
				break;
			}
		};
	}

	void unload()
	{

	}

private:
	void scale(float f)
	{
		std::cout << "scale: " << f << std::endl;
	}

	void translate(int x, int y)
	{
		std::cout << "translate: " << x << ", " << y << std::endl;
	}

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
	std::vector<vec3> _verts;

	float _scale;
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

	run_loop()
		: _stop(false)
		, _stopped(false)
	{
	}

	~run_loop()
	{

	}

	run_loop(const run_loop &) = delete;
	run_loop &operator=(const run_loop &) = delete;

	void run()
	{
		_stop = false;
		_stopped = false;
		MSG msg{};
		while (!_stop && (msg.message != WM_QUIT))
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

		_stopped = true;
	}

	void stop()
	{
		_stop = true;
	}

	bool stopped()
	{
		return _stopped;
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
	bool _stop;
	bool _stopped;
};

std::ostream &operator<<(std::ostream &os, const hresult &hr)
{
	return os << hr.error_string();
}

int main()
{
	//application app;
	window w("mgpu");
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

	//w.will_close() += [](window &w, cancel_event_args &e)
	//{
	//	e.accept();
	//	auto r = MessageBoxA(w, "Do you really want to close?", "Confirm", MB_YESNO | MB_ICONQUESTION);
	//	if (r != IDYES) e.cancel();
	//};

	w.mouse_press() += [](window &w, mouse_event_args &e)
	{
		std::cout << "mouse press: " << "(" << e.x() << ", " << e.y() << ")" << std::endl;
	};

	w.mouse_wheel() += [](window &w, mouse_event_args &e)
	{
		std::cout << "wheel: " << e.z() << std::endl;
	};

	renderer r(w);
	
	w.show();
	w.update();

	//r.set_full_screen(true);
	r.render();

	run_loop loop;
	loop.add([&](){
		r.update();
		r.render();
	});

	console::instance()->will_close() += [&](event_args &e)
	{
		loop.stop();
		ExitProcess(0);
	};

	loop.run();

	return 0;// app.run();
}

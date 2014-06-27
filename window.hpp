#pragma once

#include "win32.hpp"
class window;
class rect
{
public:
#if (_MSC_VER >= 1800) || (defined(_GNU_G_))
	rect()
		: _rc{}
	{

	}
#else
	rect()
	{
		_rc.left = 0;
		_rc.right = 0;
		_rc.top = 0;
		_rc.bottom = 0;
	}
#endif

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
		, _canceled(false)
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
	bool _canceled;
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
		, _y(GET_Y_LPARAM(lparam))
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

enum class VirtualKey
{
	Unknown
	, Back = VK_BACK
	, Tab = VK_TAB
	, Clear = VK_CLEAR
	, Return = VK_RETURN
	, Shift = VK_SHIFT
	, Control = VK_CONTROL
	, Menu = VK_MENU
	, Pause = VK_PAUSE
	, CapsLock = VK_CAPITAL
	, Kana = VK_KANA
	, Hangeul = VK_HANGEUL
	, Hangul = VK_HANGUL
	, Junja = VK_JUNJA
	, Final = VK_FINAL
	, Hanja = VK_HANJA
	, Kanji = VK_KANJI
	, Escape = VK_ESCAPE
	, Convert = VK_CONVERT
	, NonConvert = VK_NONCONVERT
	, Accept = VK_ACCEPT
	, ModeChange = VK_MODECHANGE
	, Space = VK_SPACE
	, PageUp = VK_PRIOR
	, PageDown = VK_NEXT
	, End = VK_END
	, Home = VK_HOME
	, Left = VK_LEFT
	, Up = VK_UP
	, Right = VK_RIGHT
	, Down = VK_DOWN
	, Select = VK_SELECT
	, Print = VK_PRINT
	, Execute = VK_EXECUTE
	, Snapshot = VK_SNAPSHOT
	, Delete = VK_DELETE
	, Help = VK_HELP
	, LeftWin = VK_LWIN
	, RightWin = VK_RWIN
	, Apps = VK_APPS
	, Sleep = VK_SLEEP
	, NumPad0 = VK_NUMPAD0
	, NumPad1 = VK_NUMPAD1
	, NumPad2 = VK_NUMPAD2
	, NumPad3 = VK_NUMPAD3
	, NumPad4 = VK_NUMPAD4
	, NumPad5 = VK_NUMPAD5
	, NumPad6 = VK_NUMPAD6
	, NumPad7 = VK_NUMPAD7
	, NumPad8 = VK_NUMPAD8
	, NumPad9 = VK_NUMPAD9
	, Multiply = VK_MULTIPLY
	, Add = VK_ADD
	, Separator = VK_SEPARATOR
	, Subtract = VK_SUBTRACT
	, Decimal = VK_DECIMAL
	, Divide = VK_DIVIDE
	, F1 = VK_F1
	, F2 = VK_F2
	, F3 = VK_F3
	, F4 = VK_F4
	, F5 = VK_F5
	, F6 = VK_F6
	, F7 = VK_F7
	, F8 = VK_F8
	, F9 = VK_F9
	, F10 = VK_F10
	, NumLock = VK_NUMLOCK
	, Scroll = VK_SCROLL
	, LeftShift = VK_LSHIFT
	, RightShift = VK_RSHIFT
	, LeftCtrl = VK_LCONTROL
	, RightCtrl = VK_RCONTROL
	, LeftMenu = VK_LMENU
	, RightMenu = VK_RMENU
};

class key_event_args
	: public event_args
{
public:
	key_event_args(WPARAM wparam, LPARAM lparam)
		: event_args(wparam, lparam)
		, _key(static_cast<VirtualKey>(wparam))
		, _ch(wparam)
		, _rpt_cnt(LOWORD(lparam))
	{
	}

	VirtualKey key() const
	{
		return _key;
	}

	char ch() const
	{
		return _ch;
	}

	int repeat_count() const
	{
		return _rpt_cnt;
	}

private:
	VirtualKey _key;
	char _ch;
	int _rpt_cnt;
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

class window
{
public:
	window(const std::string &title, int x, int y, int width, int height)
		: _hwnd(nullptr)
		, _hook(false)
		, _org_wproc(nullptr)
	{
		create(title, x, y, width, height);
	}

	explicit window(const std::string &title = std::string())
		: window(title, CW_USEDEFAULT, 0, 640, 480)
	{

	}

	explicit window(HWND hwnd)
		: _hwnd(hwnd)
		, _hook(true)
	{
		_org_wproc = (WNDPROC)GetClassLongPtr(hwnd, GCLP_WNDPROC);
		SetClassLongPtr(hwnd, GCLP_WNDPROC, (LONG_PTR)hook_winproc);
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

#if (_MSC_VER >= 1800) || defined(__GNUG__)
	using will_resize_event = event <window&, sizing_event_args &>;
	using did_resize_event = event < window&, size_event_args & >;
	using will_close_event = event < window&, cancel_event_args & >;
	using did_close_event = event < window&, event_args & >;
	using mouse_move_event = event < window &, mouse_event_args & >;
	using mouse_press_event = event < window &, mouse_event_args & >;
	using mouse_release_event = event < window &, mouse_event_args & >;
	using mouse_dbclick_event = event < window &, mouse_event_args & >;
	using mouse_wheel_event = event < window &, mouse_event_args & >;
	using key_press_event = event < window &, key_event_args & >;
	using key_release_event = event < window &, key_event_args & >;
	using key_char_event = event < window &, key_event_args & >;
#else
	typedef event<window &, sizing_event_args &> will_resize_event;
	typedef event<window &, size_event_args &> did_resize_event;
	typedef event<window &, cancel_event_args &> will_close_event;
	typedef event<window &, event_args &> did_close_event;
	typedef event<window &, mouse_event_args &> mouse_move_event;
	typedef event<window &, mouse_event_args &> mouse_press_event;
	typedef event<window &, mouse_event_args &> mouse_release_event;
	typedef event<window &, mouse_event_args &> mouse_dbclick_event;
	typedef event<window &, mouse_event_args &> mouse_wheel_event;
	typedef event<window &, key_event_args &> key_press_event;
	typedef event<window &, key_event_args &> key_release_event;
	typedef event<window &, key_event_args &> key_char_event;
#endif

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

	key_press_event &key_press()
	{
		return _key_press;
	}

	key_release_event &key_release()
	{
		return _key_release;
	}

	key_char_event &key_char()
	{
		return _key_char;
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
		case WM_KEYDOWN:
		{
			key_event_args args(wparam, lparam);
			if (_key_press(*this, args)) return 0;
		}
			break;
		case WM_KEYUP:
		{
			key_event_args args(wparam, lparam);
			if (_key_release(*this, args)) return 0;
		}
			break;
		case WM_CHAR:
		{
			key_event_args args(wparam, lparam);
			if (_key_char(*this, args)) return 0;
		}
			break;
		}

		return _hook ? 0 : DefWindowProcA(_hwnd, msg, wparam, lparam);
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
			SetWindowLongPtrA(hwnd, GWLP_USERDATA, (LONG_PTR)(cs->lpCreateParams));
		}

		window *w = reinterpret_cast<window *>(GetWindowLongPtrA(hwnd, GWLP_USERDATA));
		if (w)
		{
			w->_hwnd = hwnd;
			return w->msgproc(msg, wparam, lparam);
		}

		return DefWindowProcA(hwnd, msg, wparam, lparam);
	}

	static LRESULT CALLBACK hook_winproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		window *w = reinterpret_cast<window *>(GetWindowLongPtrA(hwnd, GWLP_USERDATA));
		if (w)
		{
			w->msgproc(msg, wparam, lparam);
			if (w->_org_wproc)
				return CallWindowProc(w->_org_wproc, hwnd, msg, wparam, lparam);
		}
		return DefWindowProc(hwnd, msg, wparam, lparam);
	}

private:
	//using msg_translator = std::function < LRESULT(WPARAM, LPARAM) > ;
	//std::map<UINT, msg_translator> _msg_trans;
	static int _wcount;
	static window_class _cls;
	HWND _hwnd;
	bool _hook;
	WNDPROC _org_wproc;
	will_resize_event _will_resize;
	did_resize_event _did_resize;
	will_close_event _will_close;
	did_close_event _did_close;
	mouse_move_event _mouse_move;
	mouse_press_event _mouse_press;
	mouse_release_event _mouse_release;
	mouse_dbclick_event _mouse_dbclick;
	mouse_wheel_event _mouse_wheel;
	key_press_event _key_press;
	key_release_event _key_release;
	key_char_event _key_char;
};

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

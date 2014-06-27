#pragma once

#include <Windows.h>
#include <windowsx.h>
#include <io.h>
#include <fcntl.h>

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <memory>
#include <functional>
#include <algorithm>
#include <map>
#include <fstream>
#include <cassert>
#include <cmath>

static std::string get_win32_error_string(DWORD code, DWORD lang = MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT))
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

	operator bool() const
	{
		return succeeded();
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

//template<class>
//class event;

//template<class R, class... Args>
//class event<R(Args...)>

template<class... Args>
class event
{
public:
#if _MSC_VER >= 1800
	using delegate = std::function < void(Args...) > ;
#else
	typedef std::function < void(Args...) > ;
#endif

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

#if _MSC_VER >= 1800
	using ctrl_c_event = event <event_args &>;
	using ctrl_break_event = event < event_args & > ;
	using close_event = event < event_args & > ;
	using logoff_event = event < event_args & > ;
	using shutdown_event = event < event_args & > ;
#else
	typedef event<event_args &> ctrl_c_event;
	typedef event<event_args &> ctrl_break_event;
	typedef event<event_args &> close_event;
	typedef event<event_args &> logoff_event;
	typedef event<event_args &> shutdown_event;
#endif

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

	bool operator==(nullptr_t)
	{
		return _ptr == nullptr;
	}

	bool operator!=(nullptr_t)
	{
		return _ptr != nullptr;
	}

	operator pointer()
	{
		return _ptr;
	}

	operator bool()
	{
		return _ptr != nullptr;
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
	vec_t()
		: x(0)
		, y(0)
	{

	}

	vec_t(T v1, T v2)
		: x(v1)
		, y(v2)
	{

	}

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
	vec_t()
		: x(0)
		, y(0)
		, z(0)
	{
	}

	vec_t(T v1, T v2, T v3)
		: x(v1)
		, y(v2)
		, z(v3)
	{

	}

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
	vec_t()
		: x(0)
		, y(0)
		, z(0)
		, w(0)
	{
	}

	vec_t(T v1, T v2, T v3, T v4)
		: x(v1)
		, y(v2)
		, z(v3)
		, w(v4)
	{

	}

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

template<class T>
std::ostream &operator << (std::ostream &os, const vec_t<2, T> &v)
{
	os << "(" << v.x << ", " << v.y << ")";
	return os;
}

template<class T>
std::ostream &operator << (std::ostream &os, const vec_t<3, T> &v)
{
	os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
	return os;
}

template<class T>
std::ostream &operator << (std::ostream &os, const vec_t<4, T> &v)
{
	os << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
	return os;
}

struct vertex
{
	vec3 pos;
	vec2 tex;
	//vec4 color;
	//vec3 norm;
	//vec2 uv;
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

static std::ostream &operator<<(std::ostream &os, const hresult &hr)
{
	return os << hr.error_string();
}


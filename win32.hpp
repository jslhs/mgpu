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
	com_ptr(const com_ptr &);
	com_ptr &operator=(const com_ptr &);
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

#if _MSC_VER >= 1800
using vec2 = vec_t < 2, float > ;
using vec3 = vec_t < 3, float > ;
using vec4 = vec_t < 4, float > ;
#else
typedef vec_t<2, float> vec2;
typedef vec_t<3, float> vec3;
typedef vec_t<4, float> vec4;
#endif

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

	vertex()
	{
	}

	vertex(const vec3 &p, const vec2 &t)
		: pos(p)
		, tex(t)
	{

	}
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

	void run()
	{
		_stop = false;
		_stopped = false;
		MSG msg = {};
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
	run_loop(const run_loop &);
	run_loop &operator=(const run_loop &);
	std::vector<task> _tasks;
	bool _stop;
	bool _stopped;
};

static std::ostream &operator<<(std::ostream &os, const hresult &hr)
{
	return os << hr.error_string();
}

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

	rect(int x, int y, int width, int height)
	{
		_rc.left = x;
		_rc.top = y;
		_rc.right = x + width;
		_rc.bottom = y + height;
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
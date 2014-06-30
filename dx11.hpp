#pragma once

#include <dxgi.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <wincodec.h>

#pragma comment(lib, "windowscodecs")
#pragma comment(lib, "dxgi")
#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")

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
		_src = std::string( std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>() );
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

#include <codecvt>

class texture_helper
{
public:
	static hresult create_shader_resource(ID3D11Device *dev, const std::string &filename, ID3D11ShaderResourceView **view)
	{
		//dev->CreateTexture2D()
		///dev->CreateShaderResourceView()
		auto &f = factory();

		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> cvt;
		auto wpath = cvt.from_bytes(filename);
		com_ptr<IWICBitmapDecoder> decoder;
		hresult hr = f->CreateDecoderFromFilename(wpath.c_str()
			, nullptr
			, GENERIC_READ
			, WICDecodeMetadataCacheOnDemand
			, &decoder);

		if (hr)
		{
			com_ptr<IWICBitmapFrameDecode> frame;
			hr = decoder->GetFrame(0, &frame);
			if (hr)
			{
				UINT width, height;
				hr = frame->GetSize(&width, &height);
				com_ptr<IWICFormatConverter> fmt_cvt;
				hr = f->CreateFormatConverter(&fmt_cvt);
				hr = fmt_cvt->Initialize(frame
					, GUID_WICPixelFormat32bppPBGRA
					, WICBitmapDitherTypeNone
					, nullptr
					, 0.0f
					, WICBitmapPaletteTypeCustom);
				com_ptr<ID3D11Texture2D> tex;
				D3D11_TEXTURE2D_DESC desc = {};
				desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
				desc.ArraySize = 1;
				desc.Width = width;
				desc.Height = height;
				desc.SampleDesc.Count = 1;
				desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
				desc.Usage = D3D11_USAGE_DYNAMIC;
				desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
				desc.MipLevels = 1;
				hr = dev->CreateTexture2D(&desc, nullptr, &tex);
				com_ptr<ID3D11DeviceContext> ctx;
				dev->GetImmediateContext(&ctx);
				D3D11_MAPPED_SUBRESOURCE res = {};
				hr = ctx->Map(tex, 0, D3D11_MAP_WRITE_DISCARD, 0, &res);
				hr = fmt_cvt->CopyPixels(nullptr, res.RowPitch, res.DepthPitch, (BYTE *)res.pData);
				ctx->Unmap(tex, 0);
				hr = dev->CreateShaderResourceView(tex, nullptr, view);

			}
		}

		return hr;
	}

	static com_ptr<IWICImagingFactory> &factory()
	{
		if (!_factory)
		{
			CoInitialize(nullptr);
			hresult hr = CoCreateInstance(CLSID_WICImagingFactory
				, nullptr
				, CLSCTX_INPROC_SERVER
				, __uuidof(IWICImagingFactory)
				, (PVOID *)&_factory);
		}
		return _factory;
	}

private:
	static com_ptr<IWICImagingFactory> _factory;
};
#include "win32.hpp"
#include "renderer.h"

int main()
{
	//application app;
	window w("mgpu");
	com_ptr<IDXGIFactory1> dxgi_factory;
	hresult hr = CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void **)&dxgi_factory);
	if (FAILED(hr)) std::cout << "unable to create IDXGIFactory2!" << std::endl;
	std::vector<com_ptr<IDXGIAdapter1>> adapters;
	std::vector<com_ptr<ID3D11Device>> devices;
	UINT i = 0;
	{
		com_ptr<IDXGIAdapter1> adapter;

		while (dxgi_factory->EnumAdapters1(i++, (IDXGIAdapter1 **)&adapter) != DXGI_ERROR_NOT_FOUND)
		{
			DXGI_ADAPTER_DESC1 desc;
			if (SUCCEEDED(adapter->GetDesc1(&desc)))
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
		std::cout << "mouse press: " << "(" << e.x() << ", " << e.y() << ")" << std::setw(40) << "\r";
	};

	//w.mouse_wheel() += [](window &w, mouse_event_args &e)
	//{
	//	std::cout << "wheel: " << e.z() << std::endl;
	//};

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

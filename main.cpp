#include <Windows.h>
#include <dxgi.h>
#include <iostream>
#include <vector>

#pragma comment(lib, "dxgi")

int main()
{
	IDXGIFactory1 *pDXGIFactory = nullptr;
	auto hr = CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void **)&pDXGIFactory);
	if (FAILED(hr)) std::cout << "unable to create IDXGIFactory1!" << std::endl;

	UINT i = 0;
	IDXGIAdapter1 *pAdapter;
	std::vector<IDXGIAdapter1 *> adapters;
	while (pDXGIFactory->EnumAdapters1(i++, &pAdapter) != DXGI_ERROR_NOT_FOUND)
	{
		adapters.push_back(pAdapter);
		DXGI_ADAPTER_DESC1 desc;
		if (SUCCEEDED(pAdapter->GetDesc1(&desc)))
		{
			if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) continue; // skip software adapter
			std::cout << "Adapter[" << i << "]: ";
			std::wcout << desc.Description << std::endl;
		}
	}

	std::cin.ignore(1);
	return 0;
}
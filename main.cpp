#include <Windows.h>
#include <dxgi1_2.h>
#include <d3d11.h>

#include <iostream>
#include <vector>

#pragma comment(lib, "dxgi")
#pragma comment(lib, "d3d11")

int main()
{
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

 	pDevice->Release();
	pAdapter->Release();
	pDXGIFactory->Release();

	std::cin.ignore(1);
	return 0;
}
#include "win32.hpp"
#include "window.hpp"
#include "dx11.hpp"

com_ptr<IWICImagingFactory> texture_helper::_factory;
std::unique_ptr<console> console::_inst;
int window::_wcount;
window_class window::_cls("__sui_window__{5A5C7E7A-9204-49BE-8B53-3D7FFF272967}", window::winproc);
#include "rendering.h"

#include <cstddef>

static_assert(offsetof(dsl::Win32Window, _mouse_in_window) == 113);
static_assert(offsetof(dsl::Win32Window, _mouse_x) == 96);
static_assert(offsetof(dsl::Win32Window, _mouse_y) == 100);

#include "input.h"
#include <MinHook.h>


#include "ews/AppWindow.h"

//#pragma comment(lib, "dxgi.lib")
#include <d3d11.h>

extern unsigned __int64 FindPattern(const char* module, const char* funcname, const char* pattern, const char* mask);

typedef char(__thiscall* RenderInterface__open_renderer_t)(RenderInterface* this_, __int64 params);
RenderInterface__open_renderer_t RenderInterface__open_renderer_o;

AppWindow* appwindow;
void set_appwindow(AppWindow* new_appwindow) {
  appwindow = new_appwindow;

  for (auto& device : get_directinput_devices()) {
    //device->SetCooperativeLevel(wxTheApp->GetTopWindow()->GetHWND(), DISCL_FOREGROUND);
  }
}

void create_swapchain_hook();
void setup_rendering_hooks() {
  create_swapchain_hook();
}

class CDXGIFactory1_clazz;
HRESULT(__stdcall* CreateDXGIFactory1_o)(REFIID riid, void** ppFactory);
HRESULT(__stdcall* CDXGIFactory__CreateSwapChain_o)(CDXGIFactory1_clazz* factory, IUnknown* pDevice, DXGI_SWAP_CHAIN_DESC* pDesc, IDXGISwapChain** ppSwapChain);

HWND real_raid_window;
HRESULT __stdcall CDXGIFactory__CreateSwapChain_h(CDXGIFactory1_clazz* factory, IUnknown* pDevice, DXGI_SWAP_CHAIN_DESC* pDesc, IDXGISwapChain** ppSwapChain) {
  if (appwindow) {
    auto wxappwin = appwindow->get_internal_object_type<wxAppWindow>();
    if (pDesc->OutputWindow != wxappwin->GetHWND()) {
      real_raid_window = pDesc->OutputWindow;
      //ShowWindow(real_raid_window, SW_HIDE);

//      auto dwmapi = LoadLibrary("dwmapi.dll");
//      auto DwmSetWindowAttribute = (HRESULT(*)(HWND, DWORD, LPCVOID, DWORD))GetProcAddress(dwmapi, "DwmSetWindowAttribute");
//#define DWMWA_USE_IMMERSIVE_DARK_MODE 20
//      BOOL use_immersive_dark_mode = TRUE;
//      DwmSetWindowAttribute(real_raid_window, DWMWA_USE_IMMERSIVE_DARK_MODE, &use_immersive_dark_mode, sizeof(use_immersive_dark_mode));

      wxappwin->set_real_raid_window(real_raid_window);
    }

    //pDesc->OutputWindow = wxappwin->GetHWND();
    //pDesc->BufferDesc.Width = wxappwin->GetSize().x;
    //pDesc->BufferDesc.Height = wxappwin->GetSize().y;
    //SetParent(real_raid_window, wxappwin->GetHWND());
  }

  auto result = CDXGIFactory__CreateSwapChain_o(factory, pDevice, pDesc, ppSwapChain);
  return result;
}


HRESULT __stdcall CreateDXGIFactory1_h(REFIID riid, void** ppFactory) {
  auto result = CreateDXGIFactory1_o(riid, ppFactory);
  uintptr_t** DXGIFactory1_vtbl = *reinterpret_cast<uintptr_t***>(*ppFactory);

  // 10th entry in in a IDXGIFactory object is CDXGIFactory::CreateSwapChain
  auto CDXGIFactory__CreateSwapChain_ptr = DXGIFactory1_vtbl[10];

  MH_CreateHook((LPVOID)CDXGIFactory__CreateSwapChain_ptr, &CDXGIFactory__CreateSwapChain_h, (LPVOID*)&CDXGIFactory__CreateSwapChain_o);
  MH_EnableHook((LPVOID)CDXGIFactory__CreateSwapChain_ptr);

  return result;
}

void create_swapchain_hook() {

  auto dxgi = GetModuleHandleA("dxgi.dll");
  auto CreateDXGIFactory1_ptr = GetProcAddress(dxgi, "CreateDXGIFactory1");

  MH_CreateHook((LPVOID)CreateDXGIFactory1_ptr, &CreateDXGIFactory1_h, (LPVOID*)&CreateDXGIFactory1_o);
  MH_EnableHook((LPVOID)CreateDXGIFactory1_ptr);
}
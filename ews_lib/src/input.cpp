#include "input.h"

#define WIN32_LEAN_AND_MEAN 1
#define NOMINMAX

#include <dinput.h>
#include <wx/wx.h>

#include <MinHook.h>

#define DEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
        EXTERN_C const GUID DECLSPEC_SELECTANY name \
                = { l, w1, w2, { b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } }

DEFINE_GUID(GUID_SysMouse, 0x6F1D2B60, 0xD5A0, 0x11CF, 0xBF, 0xC7, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00);
DEFINE_GUID(GUID_SysKeyboard, 0x6F1D2B61, 0xD5A0, 0x11CF, 0xBF, 0xC7, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00);

#undef DEFINE_GUID

class IDirectInputDevice8W_clazz;
class IDirectInput8W_clazz;


char* keyboard_input_source = nullptr;
void set_keyboard_input_source(char* source) {
  keyboard_input_source = source;
}

HRESULT(__stdcall* IDirectInputDevice8W__GetDeviceState_mouse_o)(IDirectInputDevice8W_clazz* thiz, DWORD size, LPVOID out);
HRESULT __stdcall IDirectInputDevice8W__GetDeviceState_mouse_h(IDirectInputDevice8W_clazz* thiz, DWORD size, LPVOID out) {
  auto result = IDirectInputDevice8W__GetDeviceState_mouse_o(thiz, size, out);

  if (size == sizeof(DIMOUSESTATE) || size == sizeof(DIMOUSESTATE2)) {
    //__debugbreak();
  }
  if (size == 256) {
    if (keyboard_input_source) {
      for (int i = 0; i < 256; i++) {
        //((char*)out)[i] = keyboard_input_source[i];
      }
    }
  }
  return result;
}

std::vector<LPDIRECTINPUTDEVICE8W> created_directinput_devices;

std::vector<LPDIRECTINPUTDEVICE8W>& get_directinput_devices() {
  return created_directinput_devices;
}

HRESULT(__stdcall* IDirectInput8W__CreateDeviceW_o)(IDirectInput8W_clazz* thiz, const GUID& rguid, LPDIRECTINPUTDEVICE8W* lplpDirectInputDevice, LPUNKNOWN pUnkOuter);
HRESULT __stdcall IDirectInput8W__CreateDeviceW_h(IDirectInput8W_clazz* thiz, const GUID& rguid, LPDIRECTINPUTDEVICE8W* lplpDirectInputDevice, LPUNKNOWN pUnkOuter) {
  auto result = IDirectInput8W__CreateDeviceW_o(thiz, rguid, lplpDirectInputDevice, pUnkOuter);

  auto IDirectInputDevice8W_vtbl = *reinterpret_cast<uintptr_t***>(*lplpDirectInputDevice);
  auto IDirectInputDevice8W__GetDeviceState_ptr = IDirectInputDevice8W_vtbl[9];
  auto IDirectInputDevice8W__SetCooperativeLevel_ptr = IDirectInputDevice8W_vtbl[13];

  //created_directinput_devices.push_back(*lplpDirectInputDevice);

  if ((rguid == GUID_SysKeyboard || rguid == GUID_SysMouse) && IDirectInputDevice8W__GetDeviceState_mouse_o == nullptr) {
    MH_CreateHook((LPVOID)IDirectInputDevice8W__GetDeviceState_ptr, &IDirectInputDevice8W__GetDeviceState_mouse_h, (LPVOID*)&IDirectInputDevice8W__GetDeviceState_mouse_o);
    MH_EnableHook((LPVOID)IDirectInputDevice8W__GetDeviceState_ptr);
  }

  //if (rguid == GUID_SysMouse) {
  //  if (IDirectInputDevice8W__GetDeviceState_mouse_o == nullptr) {
  //
  //    MH_CreateHook((LPVOID)IDirectInputDevice8W__GetDeviceState_ptr, &IDirectInputDevice8W__GetDeviceState_mouse_h, (LPVOID*)&IDirectInputDevice8W__GetDeviceState_mouse_o);
  //    MH_EnableHook((LPVOID)IDirectInputDevice8W__GetDeviceState_ptr);
  //  }
  //}
  //else if (rguid == GUID_SysKeyboard) {
  //  if (IDirectInputDevice8W__GetDeviceState_keyboard_o == nullptr) {
  //
  //    MH_CreateHook((LPVOID)IDirectInputDevice8W__GetDeviceState_ptr, &IDirectInputDevice8W__GetDeviceState_keyboard_h, (LPVOID*)&IDirectInputDevice8W__GetDeviceState_keyboard_o);
  //    MH_EnableHook((LPVOID)IDirectInputDevice8W__GetDeviceState_ptr);
  //  }
  //}

  return result;
}

HRESULT(WINAPI* DirectInput8Create_o)(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID* ppvOut, LPUNKNOWN punkOuter);
HRESULT WINAPI DirectInput8Create_h(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID* ppvOut, LPUNKNOWN punkOuter) {
  auto result = DirectInput8Create_o(hinst, dwVersion, riidltf, ppvOut, punkOuter);

  auto IDirectInput8_vtbl = *reinterpret_cast<uintptr_t***>(*ppvOut);

  IDirectInput8* dinput = *(IDirectInput8**)ppvOut;

  auto IDirectInput8W__CreateDeviceW_ptr = IDirectInput8_vtbl[3];
  //auto IDirectInput8W__ConfigureDevicesW_ptr = IDirectInput8_vtbl[10];

  MH_CreateHook((LPVOID)IDirectInput8W__CreateDeviceW_ptr, &IDirectInput8W__CreateDeviceW_h, (LPVOID*)&IDirectInput8W__CreateDeviceW_o);
  MH_EnableHook((LPVOID)IDirectInput8W__CreateDeviceW_ptr);

  return result;
}

void setup_input_hooks() {
  auto dinput8 = GetModuleHandleA("dinput8.dll");
  auto DirectInput8Create_ptr = GetProcAddress(dinput8, "DirectInput8Create");

  MH_CreateHook((LPVOID)DirectInput8Create_ptr, &DirectInput8Create_h, (LPVOID*)&DirectInput8Create_o);
  MH_EnableHook((LPVOID)DirectInput8Create_ptr);
}
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <lua_funcs.h>

#include <EWS.h>

BOOL __stdcall DllMain(HINSTANCE, DWORD reason, LPVOID reserved) {
  if(reason == DLL_PROCESS_ATTACH) {
    pdth_lua_setup();
    EWS::add_members(NULL);
  }
  return TRUE;
}
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <lua_funcs.h>

#include <EWS.h>

#pragma comment(lib, "User32.lib")
#pragma comment(lib, "Gdi32.lib")
#pragma comment(lib, "Ole32.lib")
#pragma comment(lib, "Winspool.lib")
#pragma comment(lib, "Shell32.lib")
#pragma comment(lib, "Rpcrt4.lib")
#pragma comment(lib, "Comdlg32.lib")
#pragma comment(lib, "Comctl32.lib")
#pragma comment(lib, "OleAut32.lib")
#pragma comment(lib, "Advapi32.lib")
#pragma comment(lib, "Version.lib")

BOOL __stdcall DllMain(HINSTANCE, DWORD reason, LPVOID reserved) {
  if(reason == DLL_PROCESS_ATTACH) {
    pd2_lua_setup();
    EWS::add_members(NULL);
  }
  return TRUE;
}
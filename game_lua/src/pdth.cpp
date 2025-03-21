#ifndef WIN64
#define INTERNAL_USE
#include "internal_header.h"
#undef INTERNAL_USE

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <stdint.h>

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

#define POPULATE_FUNC(name, offset) \
  funcptr_##name = (lua_funcs_types_##name##_t)(((uint8_t *)GetModuleHandle("payday_win32_release.exe")) + offset);

void pdth_lua_setup() {
  POPULATE_FUNC(lua_newstate, 0x00399C90)
  POPULATE_FUNC(lua_pushcclosure, 0x003A4260)

  POPULATE_FUNC(lua_pushstring, 0x00000000) // TODO: fill this in

  POPULATE_FUNC(lua_newuserdata, 0x0039AFA0)
  POPULATE_FUNC(lua_tolstring, 0x00394BA0)
  POPULATE_FUNC(lua_touserdata, 0x0038F480)
  POPULATE_FUNC(lua_tonumber, 0x00392760)
  POPULATE_FUNC(lua_toboolean, 0x0038F450)
  POPULATE_FUNC(lua_setfield, 0x00397520)
  POPULATE_FUNC(lua_pcall, 0x0039AF30)
  POPULATE_FUNC(luaL_loadfile, 0x003A9EC0)
  POPULATE_FUNC(lua_type, 0x0038F3B0)
  //POPULATE_FUNC(lua_gettop, 0x0038F2A0)
}

#endif
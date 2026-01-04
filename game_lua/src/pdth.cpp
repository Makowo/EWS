#ifndef WIN64
#include "lua_funcs.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <stdint.h>

#define POPULATE_FUNC(name, offset) \
  funcptr_##name = (decltype(funcptr_##name))(((uint8_t *)GetModuleHandle("payday_win32_release.exe")) + offset);

void pdth_lua_setup(SignatureScannerFunction scanner) {
  POPULATE_FUNC(lua_newstate, 0x00399C90);

  POPULATE_FUNC(lua_createtable, 0x00394C30);
  POPULATE_FUNC(lua_getfield, 0x003974C0);
  POPULATE_FUNC(lua_getmetatable, 0x0038F610);
  funcptr_lua_setmetatable = (decltype(funcptr_lua_setmetatable))scanner("payday_win32_release.exe", "", "\x8B\x4C\x24\x00\x56\x8B\x74\x24\x00\x57\x8B\xD6\xE8", "xxx?xxxx?xxxx");
  POPULATE_FUNC(lua_pcall, 0x0039AF30);
  POPULATE_FUNC(lua_pushboolean, 0x0038F5A0);
  POPULATE_FUNC(lua_pushcclosure, 0x003A4260);

  funcptr_lua_pushlightuserdata = (decltype(funcptr_lua_pushlightuserdata))scanner("payday_win32_release.exe", "", "\x8B\x44\x24\x00\x8B\x48\x00\x8B\x54\x24\x00\x89\x11", "xxx?xx?xxx?xx");

  POPULATE_FUNC(lua_pushlstring, 0x00797420);
  POPULATE_FUNC(lua_pushstring, 0x00397480);

  POPULATE_FUNC(lua_setfield, 0x00397520);
  //POPULATE_FUNC(lua_toboolean, 0x00397520);
  POPULATE_FUNC(lua_toboolean, 0x38F450);
  POPULATE_FUNC(lua_tolstring, 0x00394BA0);
  POPULATE_FUNC(lua_tonumber, 0x00392760);
  POPULATE_FUNC(lua_touserdata, 0x0038F480);
  POPULATE_FUNC(lua_type, 0x0038F3B0);
  POPULATE_FUNC(luaL_loadstring, 0x003AA160);

  POPULATE_FUNC(lua_newuserdata, 0x0039AFA0);
  POPULATE_FUNC(luaL_loadfile, 0x003A9EC0);

  POPULATE_FUNC(lua_pushnumber, 0x0038F560);

  POPULATE_FUNC(lua_settop, 0x0038F2B0);
  POPULATE_FUNC(luaL_ref, 0x00394310);
  POPULATE_FUNC(lua_rawgeti, 0x003928C0);

  POPULATE_FUNC(lua_pushvalue, 0x0038F380);
}

#endif
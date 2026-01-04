#pragma once

#include "game_lua.h"

typedef void* (*lua_Alloc) (void* ud, void* ptr, size_t osize, size_t nsize);
typedef const char* (*lua_Reader) (lua_State* L, void* ud, size_t* sz);
typedef int (*lua_Writer) (lua_State* L, const void* p, size_t sz, void* ud);
typedef int (*lua_CFunction) (lua_State* L);

#ifndef GAME_LUA_INTERNAL
#define FUNC_DEFINE(func_name, retn, args)     \
  extern retn func_name##(args); \
  extern retn(__cdecl*funcptr_##func_name)(args); 
#else
#define FUNC_DEFINE(func_name, ret, args) \
  ret(__cdecl*funcptr_##func_name)(args);
#endif

typedef size_t(*SignatureScannerFunction)(const char* exe, const char* name, const char* pattern, const char* mask);
#ifdef WIN64
extern void raidww2_lua_setup();
#else
extern void pdth_lua_setup(SignatureScannerFunction scanner);
extern void pd2_lua_setup();
#endif

#define CONCAT_IMPL(...) __VA_ARGS__
#define CONCAT_ARGS(...) CONCAT_IMPL(__VA_ARGS__)


FUNC_DEFINE(lua_createtable, void, CONCAT_ARGS(lua_State* L, int narr, int nrec));
FUNC_DEFINE(lua_getfield, void, CONCAT_ARGS(lua_State* L, int idx, const char* k));
FUNC_DEFINE(lua_getmetatable, int, CONCAT_ARGS(lua_State* L, int idx));
FUNC_DEFINE(lua_setmetatable, int, CONCAT_ARGS(lua_State* L, int index));
extern int lua_gettop(lua_State* L);
FUNC_DEFINE(lua_pcall, int, CONCAT_ARGS(lua_State* L, int nargs, int nresults, int errfunc));
FUNC_DEFINE(lua_pushboolean, void, CONCAT_ARGS(lua_State* L, int b));
FUNC_DEFINE(lua_pushcclosure, void, CONCAT_ARGS(lua_State* L, lua_CFunction fn, int n));
FUNC_DEFINE(lua_pushlstring, void, CONCAT_ARGS(lua_State* L, const char* s, size_t l));
FUNC_DEFINE(lua_pushstring, void, CONCAT_ARGS(lua_State* L, const char* s));
FUNC_DEFINE(lua_pushlightuserdata, void, CONCAT_ARGS(lua_State* L, void* p));
FUNC_DEFINE(lua_setfield, void, CONCAT_ARGS(lua_State* L, int idx, const char* k));
FUNC_DEFINE(lua_toboolean, int, CONCAT_ARGS(lua_State* L, int idx));
FUNC_DEFINE(lua_tolstring, const char*, CONCAT_ARGS(lua_State* L, int idx, size_t* len));
FUNC_DEFINE(lua_tonumber, lua_Number, CONCAT_ARGS(lua_State* L, int idx));
FUNC_DEFINE(lua_touserdata, void*, CONCAT_ARGS(lua_State* L, int idx));
FUNC_DEFINE(lua_type, int, CONCAT_ARGS(lua_State* L, int idx));
FUNC_DEFINE(luaL_loadstring, int, CONCAT_ARGS(lua_State* L, const char* s));
FUNC_DEFINE(lua_loadx, int, CONCAT_ARGS(lua_State* L, lua_Reader reader, void* data, const char* chunkname, const char* mode));

FUNC_DEFINE(lua_newuserdata, void*, CONCAT_ARGS(lua_State* L, size_t size));
FUNC_DEFINE(lua_newstate, lua_State*, CONCAT_ARGS(lua_Alloc allocf, void* allocd));
FUNC_DEFINE(luaL_loadfile, int, CONCAT_ARGS(lua_State* L, const char* filename));

FUNC_DEFINE(lua_pushnumber, void, CONCAT_ARGS(lua_State* L, lua_Number n));

FUNC_DEFINE(lua_settop, void, CONCAT_ARGS(lua_State* L, int idx));
FUNC_DEFINE(luaL_ref, int, CONCAT_ARGS(lua_State* L, int t));
FUNC_DEFINE(lua_rawgeti, void, CONCAT_ARGS(lua_State* L, int idx, int n));

FUNC_DEFINE(lua_pushvalue, void, CONCAT_ARGS(lua_State* L, int idx));

#undef FUNC_DEFINE
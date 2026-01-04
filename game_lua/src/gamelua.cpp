#define GAME_LUA_INTERNAL
#include "lua_funcs.h"

//#define EXPORT_API __declspec(dllexport)
#define EXPORT_API

#include "internal_header.h"

#include <cstdlib>

EXPORT_API void lua_createtable(lua_State* L, int narr, int nrec)
{
  return funcptr_lua_createtable(L, narr, nrec);
}

EXPORT_API int lua_setmetatable(lua_State* L, int index)
{
  return funcptr_lua_setmetatable(L, index);
}

EXPORT_API void lua_pushlightuserdata(lua_State* L, void* p)
{
  funcptr_lua_pushlightuserdata(L, p);
}

EXPORT_API lua_State* lua_newstate(lua_Alloc f, void *ud) { return funcptr_lua_newstate(f, ud); }
EXPORT_API void lua_pushcclosure(lua_State *L, lua_CFunction fn, int n) { return funcptr_lua_pushcclosure(L, fn, n); }
EXPORT_API void lua_pushstring(lua_State *L, const char *s) { return funcptr_lua_pushstring(L, s); }

EXPORT_API void* lua_newuserdata(lua_State *L, size_t sz) {
  return funcptr_lua_newuserdata(L, sz);
}

EXPORT_API const char* lua_tolstring(lua_State *L, int idx, size_t *len) { return funcptr_lua_tolstring(L, idx, len); }

EXPORT_API void* lua_touserdata(lua_State *L, int idx) { return funcptr_lua_touserdata(L, idx); }

EXPORT_API lua_Number lua_tonumber(lua_State *L, int idx) { return funcptr_lua_tonumber(L, idx); }
EXPORT_API int lua_toboolean(lua_State *L, int idx) { return funcptr_lua_toboolean(L, idx); }
EXPORT_API void lua_setfield(lua_State *L, int idx, const char *k) { return funcptr_lua_setfield(L, idx, k); }
EXPORT_API void lua_getfield(lua_State* L, int idx, const char* k) { return funcptr_lua_getfield(L, idx, k); }
EXPORT_API int lua_pcall(lua_State *L, int nargs, int nresults, int errfunc) { return funcptr_lua_pcall(L, nargs, nresults, errfunc); }
EXPORT_API int luaL_loadfile(lua_State *L, const char *filename) { return funcptr_luaL_loadfile(L, filename); }
EXPORT_API int lua_type(lua_State *L, int idx) { return funcptr_lua_type(L, idx); }
EXPORT_API int lua_gettop(lua_State *L) {
  //return (int)(*(unsigned long long*)((unsigned long long)L + 40) - *(unsigned long long*)((unsigned long long)L + 32)) / 8;
  return (int)(L->top - L->base);
}
EXPORT_API void lua_settop(lua_State* L, int idx) { return funcptr_lua_settop(L, idx); }

EXPORT_API int luaL_ref(lua_State* L, int t) { return funcptr_luaL_ref(L, t); }
EXPORT_API void lua_rawgeti(lua_State* L, int idx, int n) { return funcptr_lua_rawgeti(L, idx, n); }

EXPORT_API void lua_pushboolean(lua_State *L, int b) {
  return funcptr_lua_pushboolean(L, b);
}
EXPORT_API void lua_pushnumber(lua_State *L, lua_Number n) {
  return funcptr_lua_pushnumber(L, n);
}
EXPORT_API void lua_pushvalue(lua_State* L, int idx) {
  return funcptr_lua_pushvalue(L, idx);
}
EXPORT_API int luaL_loadstring(lua_State* L, const char* s) {
  return funcptr_luaL_loadstring(L, s);
}

static void *l_alloc(void *ud, void *ptr, size_t osize, size_t nsize)
{
  (void)ud;
  (void)osize;
  if (nsize == 0)
  {
    free(ptr);
    return NULL;
  }
  else
    return realloc(ptr, nsize);
}

lua_State *luaL_newstate(void)
{
  return lua_newstate(l_alloc, NULL);
}
#include "diesel.h"


void mvector(lua_State *L)
{
  #ifndef GAME
  lua_createtable(L, 0, 0);
  lua_pushvalue(L, -1);
  lua_setglobal(L, "mvector3");
  lua_pushcclosure(L, mvector::x, 0);
  lua_setfield(L, -2, "x");
  lua_pushcclosure(L, mvector::y, 0);
  lua_setfield(L, -2, "y");
  lua_pushcclosure(L, mvector::z, 0);
  lua_setfield(L, -2, "z");
  #endif
}

int vec3(lua_State *L)
{
  #ifndef GAME
  if (lua_gettop(L) == 0)
  {
    Vector3 *vec = (Vector3 *)lua_newuserdata(L, sizeof(Vector3));
    return 1;
  }
  if (lua_gettop(L) == 3)
  {
    float x = lua_tointeger(L, 1);
    float y = lua_tointeger(L, 2);
    float z = lua_tointeger(L, 3);
    Vector3 *vec = (Vector3 *)lua_newuserdata(L, sizeof(Vector3));
    vec->x = x;
    vec->y = y;
    vec->z = z;
    return 1;
  }
  #else
  return 0;
  #endif
}

int mvector::x(lua_State *L)
{
  #ifndef GAME
  Vector3 *vec = (Vector3 *)lua_touserdata(L, 1);
  lua_pushnumber(L, vec->x);
  return 1;
  #else
  return 0;
  #endif
}
int mvector::y(lua_State *L)
{
  #ifndef GAME
  Vector3 *vec = (Vector3 *)lua_touserdata(L, 1);
  lua_pushnumber(L, vec->y);
  return 1;
  #else
  return 0;
  #endif
}
int mvector::z(lua_State *L)
{
  #ifndef GAME
  Vector3 *vec = (Vector3 *)lua_touserdata(L, 1);
  lua_pushnumber(L, vec->z);
  return 1;
  #else
  return 0;
  #endif
}
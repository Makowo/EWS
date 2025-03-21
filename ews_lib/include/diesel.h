#pragma once

// diesel.h and diesel.cpp are recreations of diesel types, for use in an outside test environment

#include "ews_lua.hpp"

class Vector3
{
public:
  float x, y, z;
};

class mvector
{
public:
  static int x(lua_State *L);
  static int y(lua_State *L);
  static int z(lua_State *L);
};

void mvector(lua_State *L);

int vec3(lua_State *L);
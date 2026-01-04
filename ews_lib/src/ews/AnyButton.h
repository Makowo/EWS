#pragma once

#include "./Control.h"

class AnyButton : public Control {
public:
  static void AddLuaFunctions(lua_State* L);
};
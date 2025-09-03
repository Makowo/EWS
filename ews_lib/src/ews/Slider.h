
#pragma once

#include "Control.h"

class Slider : public Control {
  COMPONENT_CHILD_CLASS(Slider);
public:
  static void AddLuaFunctions(lua_State* L);

public:
  static int Lua_Create(lua_State* L);

private:
  void init(Window* parent, int value, int min_value, int max_value);
};

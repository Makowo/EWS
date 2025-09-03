
#pragma once

#include "Control.h"

class ListBox : public Control {
  COMPONENT_CHILD_CLASS(ListBox);
public:
  static void AddLuaFunctions(lua_State* L);

public:
  static int Lua_Create(lua_State* L);

private:
  void init(Window* parent);
};

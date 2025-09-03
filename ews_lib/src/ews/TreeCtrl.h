
#pragma once

#include "Control.h"

class TreeCtrl : public Control {
  COMPONENT_CHILD_CLASS(TreeCtrl);
public:
  static void AddLuaFunctions(lua_State* L);

public:
  static int Lua_Create(lua_State* L);

private:
  void init(Window* parent, const std::string& style);
};

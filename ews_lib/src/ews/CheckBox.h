
#pragma once

#include "Control.h"

class CheckBox : public Control {
  COMPONENT_CHILD_CLASS(CheckBox);
public:
  static void AddLuaFunctions(lua_State* L);

public:
  static int Lua_Create(lua_State* L);

  static int Lua_GetValue(lua_State* L);
  static int Lua_SetValue(lua_State* L);

private:
  void init(Window* parent, std::string label, std::string style);

};

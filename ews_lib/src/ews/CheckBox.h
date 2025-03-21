
#pragma once

#include "Control.h"

class CheckBox : public Control {
public:
  static void Add_CheckBox_Funcs(lua_State* L);

public:
  static int Lua_Create(lua_State* L);

  static int Lua_GetValue(lua_State* L);
  static int Lua_SetValue(lua_State* L);

private:
  void init(Window* parent, std::string label, std::string style);

};

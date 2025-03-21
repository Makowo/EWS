
#pragma once

#include "Control.h"

#include <wx/spinctrl.h>

class SpinCtrl : public Control {
public:
  static void Add_SpinCtrl_Funcs(lua_State* L);

public:
  static int Lua_Create(lua_State* L);

  static int Lua_SetValue(lua_State* L);
  static int Lua_GetValue(lua_State* L);
  static int Lua_SetRange(lua_State* L);

private:
  void init(Window* parent, std::string style);
};

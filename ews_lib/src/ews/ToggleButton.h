
#pragma once

#include "AnyButton.h"

#include <wx/tglbtn.h>

class ToggleButton : public AnyButton {
public:
  static void Add_ToggleButton_Funcs(lua_State* L);

public:
  static int Lua_Create(lua_State* L);

  static int Lua_SetValue(lua_State* L);
  static int Lua_GetValue(lua_State* L);

private:
  void init(Window* parent, std::string id, std::string label, std::string style);
};

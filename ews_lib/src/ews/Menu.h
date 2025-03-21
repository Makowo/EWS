#pragma once

#include "ews_utils.h"

#include "component.h"

#include <wx/wx.h>

class Menu : public Component {
  public:
  static void Add_Menu_Funcs(lua_State* L);

  public:
  static int Lua_Create(lua_State* L);

  static int Lua_AppendItem(lua_State* L);
  static int Lua_AppendSeparator(lua_State* L);
  static int Lua_AppendMenu(lua_State *L);
  static int Lua_AppendCheckItem(lua_State *L);
  static int Lua_AppendRadioItem(lua_State* L);

  static int Lua_Clear(lua_State* L);

  static int Lua_SetChecked(lua_State* L);
  static int Lua_IsChecked(lua_State* L);

  private:
  void init(std::string style);
};
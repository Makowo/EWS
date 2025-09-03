#pragma once

#include <wx/wx.h>

#include "Window.h"

#include "ews_utils.h"

class Dialog : public Window {
  COMPONENT_CHILD_CLASS(Dialog);
public:
  static void AddLuaFunctions(lua_State* L);

public:
  static int Lua_Create(lua_State* L);
  static int Lua_Create_Simple(lua_State* L);
  static int Lua_Create_Complex(lua_State* L);

public:
  static int Lua_ShowModal(lua_State* L);
  static int Lua_EndModal(lua_State* L);
  static int Lua_SetIcon(lua_State* L);
  static int Lua_Visible(lua_State* L);

private:
  void init(Window* parent, std::string title, std::string style, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize);
};
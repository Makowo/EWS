#pragma once

#include <wx/wx.h>

#include "Window.h"

class MenuBar : public Window {
  COMPONENT_CHILD_CLASS(MenuBar);
public:
  static void AddLuaFunctions(lua_State* L);

  public:
  static int Lua_Create(lua_State* L);
  static int Lua_Append(lua_State* L);

  private:
  void init();
};
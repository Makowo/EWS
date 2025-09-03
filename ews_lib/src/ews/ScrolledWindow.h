#pragma once

#include <wx/wx.h>

#include "Panel.h"


class ScrolledWindow : public Panel {
  COMPONENT_CHILD_CLASS(ScrolledWindow);
public:
  static void AddLuaFunctions(lua_State* L);

public:
  static int Lua_Create(lua_State* L);

public:
  static int Lua_SetScrollRate(lua_State* L);

private:
  void init(Window* parent, const std::string& name, const std::string& style);
};
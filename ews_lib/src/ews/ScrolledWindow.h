#pragma once

#include <wx/wx.h>

#include "Panel.h"


class ScrolledWindow : public Panel {
public:
  static void Add_ScrolledWindow_Funcs(lua_State* L);

public:
  static int Lua_Create(lua_State* L);

public:
  static int Lua_SetScrollRate(lua_State* L);

private:
  void init(Window* parent, const std::string& name, const std::string& style);
};
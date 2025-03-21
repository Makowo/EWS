#pragma once

#include "ews_utils.h"

#include "Window.h"

class Panel : public Window {
  public:
  static void Add_Panel_Funcs(lua_State* L);

  public:
  static int Lua_Create(lua_State* L);
private:
  void init(Window *parent, std::string name, std::string style);

};
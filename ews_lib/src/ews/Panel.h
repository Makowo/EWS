#pragma once

#include "ews_utils.h"

#include "Window.h"

class Panel : public Window {
  COMPONENT_CHILD_CLASS(Window);
public:
  static void AddLuaFunctions(lua_State* L);

  public:
  static int Lua_Create(lua_State* L);
private:
  void init(Window *parent, std::string name, std::string style);

};
#pragma once

#include "Window.h"

class StaticText : public Window {
  COMPONENT_CHILD_CLASS(StaticText);
public:
  static void AddLuaFunctions(lua_State* L);

  public:
  static int Lua_Create(lua_State* L);

  static int Lua_SetLabel(lua_State* L);
  static int Lua_SetValue(lua_State* L);

  private:
  void init(Window* parent, std::string text, int unknown, std::string style);
  public:
  wxStaticText* internal_text;
};
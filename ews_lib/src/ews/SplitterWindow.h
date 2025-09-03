#pragma once

#include <wx/splitter.h>

#include "Window.h"

class SplitterWindow : public Window {
  COMPONENT_CHILD_CLASS(SplitterWindow);
public:
  static void AddLuaFunctions(lua_State* L);

  public:
  static int Lua_Create(lua_State* L);
  static int Lua_SplitHorizontally(lua_State* L);
  static int Lua_SplitVertically(lua_State* L);

  static int Lua_SetMinimumPaneSize(lua_State* L);

private:
  void init(Window* parent, std::string name, std::string style);
};
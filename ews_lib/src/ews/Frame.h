#pragma once

#include "ews_utils.h"

#include <string>

#include <wx/wx.h>

#include "Window.h"

class Frame : public Window {
  COMPONENT_CHILD_CLASS(Frame);
public:
  static void AddLuaFunctions(lua_State* L);

public:
  static int Lua_Create(lua_State* L);
  static int Lua_Set_Icon(lua_State* L);
  static int Lua_Set_MenuBar(lua_State* L);
  static int Lua_Set_ToolBar(lua_State* L);

  static int Lua_Set_StatusBar(lua_State* L);

  static int Lua_IsIconized(lua_State* L);

private:
  void init(std::string name, Vector3* start_pos, Vector3* size, std::string style, Frame* parent);
};
#pragma once

#include "ews_utils.h"

#include <wx/wx.h>

#include "Window.h"

class ToolBar : public Window {
  COMPONENT_CHILD_CLASS(ToolBar);
public:
  static void AddLuaFunctions(lua_State* L);

  public:
  static int Lua_Create(lua_State* L);
  
  public:
  static int Lua_AddSeparator(lua_State* L);
  static int Lua_AddTool(lua_State* L);
  static int Lua_AddCheckTool(lua_State* L);
  static int Lua_AddRadioTool(lua_State* L);
  static int Lua_AddControl(lua_State* L);

  static int Lua_GetToolState(lua_State* L);
  static int Lua_SetToolState(lua_State* L);
  static int Lua_SetToolEnabled(lua_State* L);

  static int Lua_Realize(lua_State* L);

  private:
  void init(Window* parent, std::string name, std::string style);
};
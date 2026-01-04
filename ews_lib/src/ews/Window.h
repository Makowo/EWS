#pragma once

#include <wx/wx.h>

#include "component.h"

#include <iostream>

class Window : public Component {
  COMPONENT_CHILD_CLASS(Window);
public:
  static void AddLuaFunctions(lua_State* L);

public:
  static int Lua_Set_Sizer(lua_State* L);
  static int Lua_Set_Visible(lua_State* L);

  static int Lua_Layout(lua_State* L);
  static int Lua_Update(lua_State* L);
  static int Lua_Refresh(lua_State* L);

  static int Lua_Fit(lua_State* L);
  static int Lua_FitInside(lua_State* L);

  static int Lua_SetSize(lua_State* L);
  static int Lua_GetSize(lua_State* L);

  static int Lua_SetMinSize(lua_State* L);
  static int Lua_GetMinSize(lua_State* L);

  static int Lua_SetMaxSize(lua_State* L);

  static int Lua_Destroy(lua_State* L);
  static int Lua_DestroyChildren(lua_State* L);

  static int Lua_Freeze(lua_State* L);
  static int Lua_Thaw(lua_State* L);

  static int Lua_SetEnabled(lua_State* L);
  static int Lua_Enabled(lua_State* L);

  static int Lua_SetFocus(lua_State* L);
  static int Lua_InFocus(lua_State* L);

  static int Lua_GetPosition(lua_State* L);

  // wxTopLevelWindow functions
  static int Lua_TopLevelWindow_SetTitle(lua_State* L);
  static int Lua_TopLevelWindow_IsActive(lua_State* L);
  static int Lua_TopLevelWindow_IsMaximized(lua_State* L);
  
};
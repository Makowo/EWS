#pragma once

#include "ews_utils.h"
#include <wx/wx.h>

#include "./Control.h"

class Window;

class StatusBar : public Control {
  public:
  static void Add_StatusBar_Funcs(lua_State* L);

  public:
  static int Lua_Create(lua_State* L);
  
public:
  static int Lua_SetStatusText(lua_State* L);

private:
  void init(Window* parent, std::string unknown1, std::string unknown2);
};
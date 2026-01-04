#pragma once

#include <wx/wx.h>

#include "component.h"

#include <sstream>
#include <iostream>

int wxsizer_flags_to_int(std::string flags);

class Sizer : public Component {
  COMPONENT_CHILD_CLASS(Sizer);
public:
  static void AddLuaFunctions(lua_State* L);

public:
    static int Lua_Add(lua_State* L);

    static int Lua_Add_WindowObj(lua_State *L);
    static int Lua_Add_SizerObj(lua_State *L);

    static int Lua_AddSpacer(lua_State* L);
    static int Lua_AddStretchSpacer(lua_State* L);
};
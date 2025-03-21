#pragma once

#include <wx/wx.h>

#include "component.h"

#include <sstream>
#include <iostream>

int wxsizer_flags_to_int(std::string flags);

class Sizer : public Component {
  public:
  static void Add_Sizer_Funcs(lua_State* L);

  public:
    static int Lua_Add_WindowObj(lua_State *L);
    static int Lua_Add_SizerObj(lua_State *L);

    static int Lua_AddStrechSpacer(lua_State* L);
};
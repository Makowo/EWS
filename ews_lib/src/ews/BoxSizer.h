#pragma once

#include "ews_utils.h"

#include <wx/wx.h>

#include "Sizer.h"

class BoxSizer : public Sizer {
  public:
  static void Add_BoxSizer_Funcs(lua_State* L);

  public:
  static int Lua_Create(lua_State* L);
  static int Lua_Add(lua_State* L);

  private:
  void init(std::string orient);
};
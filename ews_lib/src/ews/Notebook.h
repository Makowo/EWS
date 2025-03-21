#pragma once

#include <wx/notebook.h>

#include "Window.h"

#include "ews_utils.h"

class Notebook : Window {
  public:
  static void Add_Notebook_Funcs(lua_State* L);

  public:
  static int Lua_Create(lua_State* L);
  static int Lua_Add_Page(lua_State* L);

  static int Lua_GetPageCount(lua_State* L);
  static int Lua_SetPageText(lua_State* L);
  static int Lua_GetPageText(lua_State* L);
  static int Lua_Fit(lua_State* L);
  static int Lua_SetPage(lua_State* L);
  static int Lua_GetCurrentPage(lua_State* L);

  private:
  void init(Window* parent, std::string id, std::string style);
};

#pragma once

#include "Control.h"

class ListCtrl : public Control {
  COMPONENT_CHILD_CLASS(ListCtrl);
public:
  static void AddLuaFunctions(lua_State* L);

public:
  static int Lua_Create(lua_State* L);

public:
  static int Lua_ClearAll(lua_State* L);
  static int Lua_AppendColumn(lua_State* L);
  static int Lua_AppendItem(lua_State* L);
  static int Lua_SetItemData(lua_State* L);

private:
  void init(Window* parent, const std::string& style);
};

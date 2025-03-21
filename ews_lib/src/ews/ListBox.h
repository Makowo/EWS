
#pragma once

#include "Control.h"

class ListBox : public Control {
public:
  static void Add_ListBox_Funcs(lua_State* L);

public:
  static int Lua_Create(lua_State* L);

private:
  void init();
};

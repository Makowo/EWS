
#pragma once

#include "Control.h"

class ListCtrl : public Control {
public:
  static void Add_ListCtrl_Funcs(lua_State* L);

public:
  static int Lua_Create(lua_State* L);

private:
  void init();
};

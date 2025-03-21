
#pragma once

#include "Control.h"

class TreeCtrl : public Control {
public:
  static void Add_TreeCtrl_Funcs(lua_State* L);

public:
  static int Lua_Create(lua_State* L);

private:
  void init(Window* parent, const std::string& style);
};

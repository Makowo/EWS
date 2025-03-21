
#pragma once

#include "Control.h"

class Slider : public Control {
public:
  static void Add_Slider_Funcs(lua_State* L);

public:
  static int Lua_Create(lua_State* L);

private:
  void init();
};

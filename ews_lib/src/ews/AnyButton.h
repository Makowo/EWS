#pragma once

#include "./Control.h"

class AnyButton : public Control {
public:
  static void Add_AnyButton_Funcs(lua_State* L);

};
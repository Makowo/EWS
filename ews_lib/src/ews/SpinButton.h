
#pragma once

#include "Control.h"

class SpinButton : public Control {
public:
  static void Add_SpinButton_Funcs(lua_State* L);

public:
  static int Lua_Create(lua_State* L);

private:
  void init(Window* parent, const std::string& id, const std::string& style);
};

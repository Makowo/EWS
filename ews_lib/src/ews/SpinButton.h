
#pragma once

#include "Control.h"

class SpinButton : public Control {
  COMPONENT_CHILD_CLASS(SpinButton);
public:
  static void AddLuaFunctions(lua_State* L);

public:
  static int Lua_Create(lua_State* L);

private:
  void init(Window* parent, const std::string& id, const std::string& style);
};

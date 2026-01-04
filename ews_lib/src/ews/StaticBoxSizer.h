
#pragma once

#include "BoxSizer.h"

class Window;
class StaticBoxSizer : public BoxSizer {
  COMPONENT_CHILD_CLASS(StaticBoxSizer);
public:
  static void AddLuaFunctions(lua_State* L);

public:
  static int Lua_Create(lua_State* L);

private:
  void init(Window* win, std::string orient, std::string title);

};

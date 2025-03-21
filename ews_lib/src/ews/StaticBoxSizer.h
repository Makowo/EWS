
#pragma once

#include "BoxSizer.h"

class Window;
class StaticBoxSizer : public BoxSizer {
public:
  static void Add_StaticBoxSizer_Funcs(lua_State* L);

public:
  static int Lua_Create(lua_State* L);

private:
  void init(Window* win, std::string orient, std::string title);

};

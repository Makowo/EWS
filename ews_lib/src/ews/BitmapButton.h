
#pragma once

#include "Button.h"

class BitmapButton : public Button {
  COMPONENT_CHILD_CLASS(BitmapButton);
public:
  static void AddLuaFunctions(lua_State* L);

public:
  static int Lua_Create(lua_State* L);

private:
  void init(Window* parent, const wxBitmapBundle& bitmap, const std::string& style);
};


#pragma once

#include "Button.h"

class BitmapButton : public Button {
public:
  static void Add_BitmapButton_Funcs(lua_State* L);

public:
  static int Lua_Create(lua_State* L);

private:
  void init(Window* parent, const wxBitmapBundle& bitmap, const std::string& style);
};

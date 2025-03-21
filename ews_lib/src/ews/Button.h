#pragma once

#include "AnyButton.h"

class Button : public AnyButton {
public:
  static void Add_Button_Funcs(lua_State* L);

public:
  static int Lua_Create(lua_State* L);

private:
  void init(Window* parent, const std::string& label, const std::string& style);
};
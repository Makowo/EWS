
#pragma once

#include "Control.h"

#include "./TextEntry.h"

class ComboBox : public Control {
  COMPONENT_CHILD_CLASS("ComboBox");
public:
  static void AddLuaFunctions(lua_State* L);

public:
  static int Lua_Create(lua_State* L);

  static int Lua_Append(lua_State* L);

  // TextEntry functions, cant move into another sub-class because of errors, must be kept 1:1 with TextCtrl for feature parity
  TEXTENTRY_HEADER_FUNCS

private:
  void init(Window* parent, std::string style);
};

#pragma once

#include <wx/textctrl.h>

#include "./Control.h"

#include "./TextEntry.h"

class TextCtrl : public Control {
  COMPONENT_CHILD_CLASS(TextCtrl);
public:
  static void AddLuaFunctions(lua_State* L);

public:
  static int Lua_Create(lua_State* L);

public:
  static int Lua_ShowPosition(lua_State* L);

  // TextEntry functions, cant move into another sub-class because of errors, must be kept 1:1 with ComboBox for feature parity
  TEXTENTRY_HEADER_FUNCS

private:
  void init(Window* parent, const std::string& text, const std::string& style);
};
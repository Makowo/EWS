
#pragma once

#include "Control.h"

#include <wx/radiobut.h>

class RadioButton : public Control {
public:
  static void Add_RadioButton_Funcs(lua_State* L);

public:
  static int Lua_Create(lua_State* L);

private:
  void init(Window* parent, std::string name, std::string group, std::string style);
};

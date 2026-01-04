#pragma once

#include <wx/wx.h>

#include "Dialog.h"

#include "ews_utils.h"

class MessageDialog : public Dialog {
  COMPONENT_CHILD_CLASS(MessageDialog);
public:
  static void AddLuaFunctions(lua_State* L);

public:
  static int Lua_Create(lua_State* L);

  private:
    void init(Window *parent, std::string message, std::string caption, std::string style);
};
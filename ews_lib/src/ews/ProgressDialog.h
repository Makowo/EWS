#pragma once

#include "Dialog.h"

#include <wx/wx.h>
#include <wx/progdlg.h>

class ProgressDialog : public Dialog {
public:
  static void Add_ProgressDialog_Funcs(lua_State* L);

public:
  static int Lua_Create(lua_State* L);

  static int Lua_UpdateBar(lua_State* L);

private:
  void init(std::string title, std::string message, int maximum, Window* parent, std::string style);
};
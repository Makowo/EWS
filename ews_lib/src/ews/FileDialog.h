
#pragma once

#include "Dialog.h"

class FileDialog : public Dialog {
  COMPONENT_CHILD_CLASS(FileDialog);
public:
  static void AddLuaFunctions(lua_State* L);

public:
  static int Lua_Create(lua_State* L);

  static int Lua_GetPath(lua_State* L);
  static int Lua_GetDirectory(lua_State* L);

private:
  void init(Window* parent, std::string title, std::string start_path, std::string default_file, std::string type_selection_restrictions, std::string style);
};

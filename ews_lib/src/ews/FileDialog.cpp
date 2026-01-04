
#include "FileDialog.h"

void FileDialog::AddLuaFunctions(lua_State* L)
{
  Dialog::AddLuaFunctions(L);
  REGISTER_LUA_CLASS_FUNCTION(FileDialog::Lua_GetPath, "get_path");
  REGISTER_LUA_CLASS_FUNCTION(FileDialog::Lua_GetDirectory, "get_directory");
}

int FileDialog::Lua_Create(lua_State* L) {
  auto parent = get_ews_object_from_top<FileDialog>(L, 2);
  auto title = lua_tostring(L, 3);
  auto start_path = lua_tostring(L, 4);
  auto default_file = lua_tostring(L, 5);
  auto type_selection_restrictions = lua_tostring(L, 6);
  auto style = lua_tostring(L, 7);

  auto filedialog = create_new_ews_object<FileDialog>(L);

  filedialog->init(parent, title, start_path, default_file, type_selection_restrictions, style);

  return 1;
}

int FileDialog::Lua_GetPath(lua_State* L) {
  auto filedialog = get_ews_object_from_top<FileDialog>(L, 1);

  lua_pushstring(L, filedialog->get_internal_object_type<wxFileDialog>()->GetPath().c_str());

  return 1;
}

int FileDialog::Lua_GetDirectory(lua_State* L) {
  auto filedialog = get_ews_object_from_top<FileDialog>(L, 1);

  lua_pushstring(L, filedialog->get_internal_object_type<wxFileDialog>()->GetDirectory().c_str());

  return 1;
}

void FileDialog::init(Window* parent, std::string title, std::string start_path, std::string default_file, std::string type_selection_restrictions, std::string style) {
  this->set_internal_object_type(new wxFileDialog(parent->get_internal_object_type<wxWindow>(), title, start_path, default_file, type_selection_restrictions, get_style_from_merged_list(style)));
}

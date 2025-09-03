#include "ProgressDialog.h"

void ProgressDialog::AddLuaFunctions(lua_State* L)
{
  Dialog::AddLuaFunctions(L);
  REGISTER_LUA_CLASS_FUNCTION(ProgressDialog::Lua_UpdateBar, "update_bar");
}

int ProgressDialog::Lua_Create(lua_State* L) {
  auto parent = get_ews_object_from_top<Window>(L, 2);

  auto title = lua_tostring(L, 3);
  auto message = lua_tostring(L, 4);

  auto maximum = lua_tonumber(L, 5);
  auto style = lua_tostring(L, 6);

#define AHHHHHHHHHHHHHHHHH(a) std::cout << "[ProgressDialog] " #a  " " << a << std::endl;
  //AHHHHHHHHHHHHHHHHH(parent)
  //AHHHHHHHHHHHHHHHHH(title)
  //AHHHHHHHHHHHHHHHHH(message)
  //AHHHHHHHHHHHHHHHHH(maximum)
  //AHHHHHHHHHHHHHHHHH(style)

  auto progressdialog = create_new_ews_object<ProgressDialog>(L);
  //AHHHHHHHHHHHHHHHHH(progressdialog)

  progressdialog->init(title, message, maximum, parent, style);

  return 1;
}

int ProgressDialog::Lua_UpdateBar(lua_State* L) {
  auto progressdialog = get_ews_object_from_top<ProgressDialog>(L, 1);

  auto value = lua_tonumber(L, 2);

  wxString new_message = wxEmptyString;
  if (lua_type(L, 3) == LUA_TSTRING) { // optional parameter
    new_message = wxString(lua_tostring(L, 3));
  }

  if (progressdialog == nullptr) {
    __debugbreak();
    return 0;
  }

  progressdialog->get_internal_object_type<wxProgressDialog>()->Update(value, new_message);

  return 0;
}

void ProgressDialog::init(std::string title, std::string message, int maximum, Window* parent, std::string style) {
  this->set_internal_object_type(new wxProgressDialog(title, message, maximum, parent->get_internal_object_type<wxWindow>(), get_style_from_merged_list(style)));
}

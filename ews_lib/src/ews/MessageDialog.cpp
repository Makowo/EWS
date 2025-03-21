#include "MessageDialog.h"

#include <iostream>

static const std::vector<luaL_Reg> methods = {
};


void MessageDialog::Add_MessageDialog_Funcs(lua_State* L) {
  for (int i = 0; i < methods.size(); i++)
  {
    lua_register(L, methods[i].name, methods[i].func);
  }
}

int MessageDialog::Lua_Create(lua_State* L) {
  Window *parent = get_ews_object_from_top<Window>(L, 1);
  auto message = lua_tostring(L, 2);
  auto caption = lua_tostring(L, 3);
  auto style = lua_tostring(L, 4);

  auto dialog = create_new_ews_object<MessageDialog>(L);

  dialog->init(parent, message, caption, style);

  return 1;
}

void MessageDialog::init(Window *parent, std::string message, std::string caption, std::string style)
{
  this->set_internal_object_type(new wxMessageDialog(parent->get_internal_object_type<wxWindow>(), wxString(message), wxString(caption), get_style_from_merged_list(style)));
}
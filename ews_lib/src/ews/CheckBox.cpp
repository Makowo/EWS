
#include "CheckBox.h"

static const std::vector<luaL_Reg> methods = {
  {"EWS_CheckBox_GetValue", CheckBox::Lua_GetValue},
  {"EWS_CheckBox_SetValue", CheckBox::Lua_SetValue},
};

ADD_FUNCS_AUTOFILL(CheckBox::Add_CheckBox_Funcs)

int CheckBox::Lua_Create(lua_State* L) {
  auto parent = get_ews_object_from_top<Window>(L, 1);
  auto name = lua_tostring(L, 2);
  auto style = lua_tostring(L, 3);

  auto checkbox = create_new_ews_object<CheckBox>(L);

  checkbox->init(parent, name, style);

  return 1;
}

int CheckBox::Lua_GetValue(lua_State* L) {
  auto checkbox = get_ews_object_from_top<CheckBox>(L, 1);

  lua_pushboolean(L, checkbox->get_internal_object_type<wxCheckBox>()->GetValue());

  return 1;
}

int CheckBox::Lua_SetValue(lua_State* L) {
  auto checkbox = get_ews_object_from_top<CheckBox>(L, 1);
  auto new_value = lua_toboolean(L, 2);

  checkbox->get_internal_object_type<wxCheckBox>()->SetValue(new_value);

  return 0;
}

void CheckBox::init(Window* parent, std::string label, std::string style) {
  this->set_internal_object_type(new wxCheckBox(parent->get_internal_object_type<wxWindow>(), wxID_ANY, label, wxDefaultPosition, wxDefaultSize, get_style_from_merged_list(style)));
}


#include "CheckBox.h"

void CheckBox::AddLuaFunctions(lua_State* L)
{
  Control::AddLuaFunctions(L);
  REGISTER_LUA_CLASS_FUNCTION(CheckBox::Lua_GetValue, "get_value");
  REGISTER_LUA_CLASS_FUNCTION(CheckBox::Lua_SetValue, "set_value");
}

int CheckBox::Lua_Create(lua_State* L) {
  auto parent = get_ews_object_from_top<Window>(L, 2);
  auto name = lua_tostring(L, 3);
  auto style = lua_tostring(L, 4);

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


#include "ToggleButton.h"

void ToggleButton::AddLuaFunctions(lua_State* L)
{
  AnyButton::AddLuaFunctions(L);
  REGISTER_LUA_CLASS_FUNCTION(ToggleButton::Lua_SetValue, "set_value");
  REGISTER_LUA_CLASS_FUNCTION(ToggleButton::Lua_GetValue, "get_value");
}

int ToggleButton::Lua_Create(lua_State* L) {
  auto parent = get_ews_object_from_top<Window>(L, 2);
  auto label = lua_tostring(L, 3);
  auto style = lua_tostring(L, 4);

  auto id = "";

  auto togglebutton = create_new_ews_object<ToggleButton>(L);

  togglebutton->init(parent, id, label, style);

  return 1;
}

int ToggleButton::Lua_SetValue(lua_State* L) {
  auto tglbtn = get_ews_object_from_top<ToggleButton>(L, 1);
  auto new_value = lua_toboolean(L, 2);

  tglbtn->get_internal_object_type<wxToggleButton>()->SetValue(new_value);

  return 0;
}

int ToggleButton::Lua_GetValue(lua_State* L) {
  auto tglbtn = get_ews_object_from_top<ToggleButton>(L, 1);

  lua_pushboolean(L, tglbtn->get_internal_object_type<wxToggleButton>()->GetValue());

  return 1;
}

void ToggleButton::init(Window* parent, std::string id, std::string label, std::string style) {
  this->set_internal_object_type(new wxToggleButton(parent->get_internal_object_type<wxWindow>(), get_winid_from_string(id), label, wxDefaultPosition, wxDefaultSize, get_style_from_merged_list(style)));
}

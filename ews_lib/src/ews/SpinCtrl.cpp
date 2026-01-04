
#include "SpinCtrl.h"

void SpinCtrl::AddLuaFunctions(lua_State* L)
{
  Control::AddLuaFunctions(L);
  REGISTER_LUA_CLASS_FUNCTION(SpinCtrl::Lua_SetValue, "set_value");
  REGISTER_LUA_CLASS_FUNCTION(SpinCtrl::Lua_GetValue, "get_value");
  REGISTER_LUA_CLASS_FUNCTION(SpinCtrl::Lua_SetRange, "set_range");
}

int SpinCtrl::Lua_Create(lua_State* L) {
  auto parent = get_ews_object_from_top<Window>(L, 2);
  auto style = lua_tostring(L, 3);

  auto spinctrl = create_new_ews_object<SpinCtrl>(L);

  spinctrl->init(parent, style);

  return 1;
}

int SpinCtrl::Lua_SetValue(lua_State* L) {
  auto spinctrl = get_ews_object_from_top<SpinCtrl>(L, 1);

  switch (lua_type(L, 2)) {
  case LUA_TSTRING:
    spinctrl->get_internal_object_type<wxSpinCtrl>()->SetValue(lua_tostring(L, 2));
    break;
  case LUA_TNUMBER:
    spinctrl->get_internal_object_type<wxSpinCtrl>()->SetValue(lua_tonumber(L, 2));
    break;
  }
  

  return 0;
}

int SpinCtrl::Lua_GetValue(lua_State* L) {
  auto spinctrl = get_ews_object_from_top<SpinCtrl>(L, 1);

  lua_pushnumber(L, spinctrl->get_internal_object_type<wxSpinCtrl>()->GetValue());

  return 1;
}

int SpinCtrl::Lua_SetRange(lua_State* L) {
  auto spinctrl = get_ews_object_from_top<SpinCtrl>(L, 1);

  auto min = lua_tonumber(L, 2);
  auto max = lua_tonumber(L, 3);

  spinctrl->get_internal_object_type<wxSpinCtrl>()->SetRange(min, max);

  return 0;
}

void SpinCtrl::init(Window* parent, std::string style) {
  this->set_internal_object_type(new wxSpinCtrl(parent->get_internal_object_type<wxWindow>(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, get_style_from_merged_list(style)));
}

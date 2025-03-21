
#include "SpinCtrl.h"

static const std::vector<luaL_Reg> methods = {
  {"EWS_SpinCtrl_SetValue", SpinCtrl::Lua_SetValue},
  {"EWS_SpinCtrl_GetValue", SpinCtrl::Lua_GetValue},
  {"EWS_SpinCtrl_SetRange", SpinCtrl::Lua_SetRange},
};

ADD_FUNCS_AUTOFILL(SpinCtrl::Add_SpinCtrl_Funcs)

int SpinCtrl::Lua_Create(lua_State* L) {
  auto parent = get_ews_object_from_top<Window>(L, 1);
  auto style = lua_tostring(L, 2);

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

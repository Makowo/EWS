
#include "RadioButton.h"

static const std::vector<luaL_Reg> methods = {
};

ADD_FUNCS_AUTOFILL(RadioButton::Add_RadioButton_Funcs)

int RadioButton::Lua_Create(lua_State* L) {
  auto parent = get_ews_object_from_top<Window>(L, 1);
  auto name = lua_tostring(L, 2);
  auto group = lua_tostring(L, 3);
  auto style = lua_tostring(L, 4);

  auto radiobutton = create_new_ews_object<RadioButton>(L);

  radiobutton->init(parent, name, group, style);

  return 1;
}

void RadioButton::init(Window* parent, std::string name, std::string group, std::string style) {
  this->set_internal_object_type(new wxRadioButton(parent->get_internal_object_type<wxWindow>(), get_winid_from_string(name), name, wxDefaultPosition, wxDefaultSize, get_style_from_merged_list(style)));
}

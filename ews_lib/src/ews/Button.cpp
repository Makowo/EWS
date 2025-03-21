#include "Button.h"


static const std::vector<luaL_Reg> methods = {
};

ADD_FUNCS_AUTOFILL(Button::Add_Button_Funcs)

int Button::Lua_Create(lua_State* L) {
  auto parent = get_ews_object_from_top<Window>(L, 1);
  auto label = lua_tostring(L, 2);
  auto style = lua_tostring(L, 3);

  auto button = create_new_ews_object<Button>(L);

  button->init(parent, label, style);

  return 1;
}

void Button::init(Window* parent, const std::string& label, const std::string& style) {
  this->set_internal_object_type(new wxButton(parent->get_internal_object_type<wxWindow>(), wxID_ANY, label, wxDefaultPosition, wxDefaultSize, get_style_from_merged_list(style)));
}

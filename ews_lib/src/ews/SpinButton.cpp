
#include "SpinButton.h"

#include <wx/spinbutt.h>

static const std::vector<luaL_Reg> methods = {
};

ADD_FUNCS_AUTOFILL(SpinButton::Add_SpinButton_Funcs)

int SpinButton::Lua_Create(lua_State* L) {
  auto parent = get_ews_object_from_top<Window>(L, 1);
  auto id = lua_tostring(L, 2);
  auto style = lua_tostring(L, 3);

  auto spinbutton = create_new_ews_object<SpinButton>(L);

  spinbutton->init(parent, id, style);

  return 1;
}

void SpinButton::init(Window* parent, const std::string& id, const std::string& style) {
  this->set_internal_object_type(new wxSpinButton(parent->get_internal_object_type<wxWindow>(), get_winid_from_string(id), wxDefaultPosition, wxDefaultSize, get_style_from_merged_list(style)));
}

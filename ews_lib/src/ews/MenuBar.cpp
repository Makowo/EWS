#include "MenuBar.h"
#include "Menu.h"

#include "ews_utils.h"

static const std::vector<luaL_Reg> methods = {
  {"EWS_MenuBar_Append", MenuBar::Lua_Append}
};

ADD_FUNCS_AUTOFILL(MenuBar::Add_MenuBar_Funcs)

int MenuBar::Lua_Create(lua_State* L) {
  MenuBar* bar = create_new_ews_object<MenuBar>(L);
  bar->init();
  return 1;
}

int MenuBar::Lua_Append(lua_State *L) {
  auto menubar = get_ews_object_from_top<MenuBar>(L, 1);
  auto menu = get_ews_object_from_top<Menu>(L, 2);
  auto name = lua_tostring(L, 3);

  menubar->get_internal_object_type<wxMenuBar>()->Append(menu->get_internal_object_type<wxMenu>(), wxString(name));
  return 0;
}

void MenuBar::init() {
  this->set_internal_object_type(new wxMenuBar());
}
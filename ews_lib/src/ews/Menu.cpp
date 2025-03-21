#include "Menu.h"

static const std::vector<luaL_Reg> methods = {
  {"EWS_Menu_AppendItem", Menu::Lua_AppendItem},
  {"EWS_Menu_AppendSeparator", Menu::Lua_AppendSeparator},
  {"EWS_Menu_AppendMenu", Menu::Lua_AppendMenu},
  {"EWS_Menu_AppendCheckItem", Menu::Lua_AppendCheckItem},
  {"EWS_Menu_AppendRadioItem", Menu::Lua_AppendRadioItem},

  {"EWS_Menu_Clear", Menu::Lua_Clear},

  {"EWS_Menu_SetChecked", Menu::Lua_SetChecked},
  {"EWS_Menu_IsChecked", Menu::Lua_IsChecked},
};

ADD_FUNCS_AUTOFILL(Menu::Add_Menu_Funcs)

int Menu::Lua_Create(lua_State* L) {
  auto style = lua_tostring(L, 1);
  Menu* menu = create_new_ews_object<Menu>(L);
  menu->init(style);
  return 1;
}
int Menu::Lua_AppendItem(lua_State* L) {
  auto menu = get_ews_object_from_top<Menu>(L, 1);
  const char* id = lua_tostring(L, 2);
  if (id == nullptr) id = "";
  const char* name = lua_tostring(L, 3);
  if (name == nullptr) name = "";
  const char* description = lua_tostring(L, 4);
  if (description == nullptr) description = "";

  menu->get_internal_object_type<wxMenu>()->Append(get_winid_from_string(id), name, description);
  return 0;
}
int Menu::Lua_AppendMenu(lua_State* L) {
  auto menu1 = get_ews_object_from_top<Menu>(L, 1);
  auto id = lua_tostring(L, 2);
  auto name = lua_tostring(L, 3);
  auto menu2 = get_ews_object_from_top<Menu>(L, 4);
  auto description = lua_tostring(L, 5);

  menu1->get_internal_object_type<wxMenu>()->AppendSubMenu(menu2->get_internal_object_type<wxMenu>(), wxString(name), wxString(description));

  return 0;
}
int Menu::Lua_AppendSeparator(lua_State* L) {
  auto menu = get_ews_object_from_top<Menu>(L, 1);

  menu->get_internal_object_type<wxMenu>()->AppendSeparator();

  return 0;
}

int Menu::Lua_AppendCheckItem(lua_State* L) {
  Menu* menu = get_ews_object_from_top<Menu>(L, 1);
  const char* id = lua_tostring(L, 2);
  if (id == nullptr) id = "";
  const char* name = lua_tostring(L, 3);
  if (name == nullptr) name = "";
  const char* description = lua_tostring(L, 4);
  if (description == nullptr) description = "";

  menu->get_internal_object_type<wxMenu>()->AppendCheckItem(get_winid_from_string(id), wxString(name), wxString(description));
  return 0;
}

int Menu::Lua_AppendRadioItem(lua_State* L) {
  Menu *menu = get_ews_object_from_top<Menu>(L, 1);
  const char* id = lua_tostring(L, 2);
  if(id == nullptr) id = "";
  const char* name = lua_tostring(L, 3);
  if(name == nullptr) name = "";
  const char* description = lua_tostring(L, 4);
  if (description == nullptr) description = "";

  menu->get_internal_object_type<wxMenu>()->AppendRadioItem(get_winid_from_string(id), wxString(name), wxString(description));
  return 0;
}

int Menu::Lua_Clear(lua_State* L) {
  auto menu = get_ews_object_from_top<Menu>(L, 1);

  for (int i = menu->get_internal_object_type<wxMenu>()->GetMenuItemCount(); i > -1; i--) {
    menu->get_internal_object_type<wxMenu>()->Destroy(i);
  }

  return 0;
}

int Menu::Lua_SetChecked(lua_State* L) {
  auto menu = get_ews_object_from_top<Menu>(L, 1);
  auto item_id = lua_tostring(L, 2);
  auto checked = lua_toboolean(L, 3);

  menu->get_internal_object_type<wxMenu>()->Check(get_winid_from_string(item_id), checked);

  return 0;
}

int Menu::Lua_IsChecked(lua_State* L) {
  auto menu = get_ews_object_from_top<Menu>(L, 1);
  auto item_id = lua_tostring(L, 2);

  lua_pushboolean(L, menu->get_internal_object_type<wxMenu>()->IsChecked(get_winid_from_string(item_id)));

  return 1;
}

void Menu::init(std::string style) {
  this->set_internal_object_type(new wxMenu(get_style_from_merged_list(style)));
}
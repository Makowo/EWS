#include "StatusBar.h"

void StatusBar::AddLuaFunctions(lua_State* L)
{
  Control::AddLuaFunctions(L);
  REGISTER_LUA_CLASS_FUNCTION(StatusBar::Lua_SetStatusText, "set_status_text");
}

int StatusBar::Lua_Create(lua_State* L) {
  auto parent = get_ews_object_from_top<Window>(L, 2);

  const char* unk1 = "";
  if (lua_type(L, 3) != LUA_TNIL) {
    unk1 = lua_tostring(L, 3);
  }
  const char* unk2 = "";
  if (lua_type(L, 4) != LUA_TNIL) {
    unk2 = lua_tostring(L, 4);
  }

  auto status_bar = create_new_ews_object<StatusBar>(L);

  status_bar->init(parent, unk1, unk2);

  return 1;
}

int StatusBar::Lua_SetStatusText(lua_State* L) {
  auto status_bar = get_ews_object_from_top<StatusBar>(L, 1);
  auto text = lua_tostring(L, 2);

  status_bar->get_internal_object_type<wxStatusBar>()->SetStatusText(text);

  return 0;
}

void StatusBar::init(Window* parent, std::string unknown1, std::string unknown2) {
  this->set_internal_object_type(new wxStatusBar(parent->get_internal_object_type<wxWindow>()));
}

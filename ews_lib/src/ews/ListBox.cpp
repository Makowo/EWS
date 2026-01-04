
#include "ListBox.h"

int empty_str(lua_State* L) { lua_pushstring(L, ""); return 1; }
int return_zero(lua_State* L) { lua_pushnumber(L, 0); return 1; }
int return_one(lua_State* L) { lua_pushnumber(L, 1); return 1; }

void ListBox::AddLuaFunctions(lua_State* L)
{
  Control::AddLuaFunctions(L);

  REGISTER_LUA_CLASS_FUNCTION(do_nothing, "append");
  REGISTER_LUA_CLASS_FUNCTION(do_nothing, "clear");
  REGISTER_LUA_CLASS_FUNCTION(do_nothing, "select_index");
  REGISTER_LUA_CLASS_FUNCTION(return_one, "nr_items");
  REGISTER_LUA_CLASS_FUNCTION(return_zero, "selected_index");
  REGISTER_LUA_CLASS_FUNCTION(do_nothing, "deselect_index");
  REGISTER_LUA_CLASS_FUNCTION(empty_str, "get_string");
}

int ListBox::Lua_Create(lua_State* L) {
  Window* parent = get_ews_object_from_top<Window>(L, 2);

  auto listbox = create_new_ews_object<ListBox>(L);

  listbox->init(parent);

  return 1;
}

void ListBox::init(Window* parent) {
  this->set_internal_object_type(new wxListBox(parent->get_internal_object_type<wxWindow>(), -1));
}

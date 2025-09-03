
#include "ListCtrl.h"

#include <wx/listctrl.h>

void ListCtrl::AddLuaFunctions(lua_State* L)
{
  Control::AddLuaFunctions(L);
  REGISTER_LUA_CLASS_FUNCTION(do_nothing, "clear_all");
  REGISTER_LUA_CLASS_FUNCTION(do_nothing, "append_column");
  REGISTER_LUA_CLASS_FUNCTION(do_nothing, "append_item");
  REGISTER_LUA_CLASS_FUNCTION(do_nothing, "set_item_data");
  REGISTER_LUA_CLASS_FUNCTION(do_nothing, "autosize_column");
}

int ListCtrl::Lua_Create(lua_State* L) {
  auto parent = get_ews_object_from_top<Window>(L, 2);
  auto unknown = lua_tostring(L, 3);
  auto style = lua_tostring(L, 4);

  auto listctrl = create_new_ews_object<ListCtrl>(L);

  listctrl->init(parent, style);

  return 1;
}

void ListCtrl::init(Window* parent, const std::string& style) {
  this->set_internal_object_type(new wxListCtrl(parent->get_internal_object_type<wxWindow>(), -1, wxDefaultPosition, wxDefaultSize, get_style_from_merged_list(style)));
}

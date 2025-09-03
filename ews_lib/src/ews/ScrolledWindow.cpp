#include "ScrolledWindow.h"

void ScrolledWindow::AddLuaFunctions(lua_State* L)
{
  Panel::AddLuaFunctions(L);
  REGISTER_LUA_CLASS_FUNCTION(ScrolledWindow::Lua_SetScrollRate, "set_scroll_rate");
}

int ScrolledWindow::Lua_Create(lua_State* L) {
  auto parent = get_ews_object_from_top<Window>(L, 2);
  auto name = lua_tostring(L, 3);
  auto style = lua_tostring(L, 4);

  auto scrolled_window = create_new_ews_object<ScrolledWindow>(L);
  scrolled_window->init(parent, name, style);

  return 1;
}

int ScrolledWindow::Lua_SetScrollRate(lua_State* L) {
  auto window = get_ews_object_from_top<ScrolledWindow>(L, 1);
  auto scroll_rate = get_vec3_from_arg(L, 2);

  window->get_internal_object_type<wxScrolledWindow>()->SetScrollRate(scroll_rate->x, scroll_rate->y);

  return 0;
}

void ScrolledWindow::init(Window* parent, const std::string& name, const std::string& style) {
  this->set_internal_object_type(new wxScrolledWindow(parent->get_internal_object_type<wxWindow>(), wxID_ANY, wxDefaultPosition, wxDefaultSize, get_style_from_merged_list(style)));
}

#include "ScrolledWindow.h"


static const std::vector<luaL_Reg> methods = {
  {"EWS_ScrolledWindow_SetScrollRate", ScrolledWindow::Lua_SetScrollRate}
};

ADD_FUNCS_AUTOFILL(ScrolledWindow::Add_ScrolledWindow_Funcs)

int ScrolledWindow::Lua_Create(lua_State* L) {
  auto parent = get_ews_object_from_top<Window>(L, 1);
  auto name = lua_tostring(L, 2);
  auto style = lua_tostring(L, 3);

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

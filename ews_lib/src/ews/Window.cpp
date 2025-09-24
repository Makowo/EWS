#include "Window.h"

#include "Sizer.h"

void Window::AddLuaFunctions(lua_State* L)
{
  Component::AddLuaFunctions(L);
  REGISTER_LUA_CLASS_FUNCTION(Window::Lua_Set_Sizer, "set_sizer");
  REGISTER_LUA_CLASS_FUNCTION(Window::Lua_Set_Visible, "set_visible");
  REGISTER_LUA_CLASS_FUNCTION(Window::Lua_Layout, "layout");
  REGISTER_LUA_CLASS_FUNCTION(Window::Lua_Update, "update");
  REGISTER_LUA_CLASS_FUNCTION(Window::Lua_Refresh, "refresh");
  REGISTER_LUA_CLASS_FUNCTION(Window::Lua_Fit, "fit");
  REGISTER_LUA_CLASS_FUNCTION(Window::Lua_FitInside, "fit_inside");
  REGISTER_LUA_CLASS_FUNCTION(Window::Lua_SetSize, "set_size");
  REGISTER_LUA_CLASS_FUNCTION(Window::Lua_GetSize, "get_size");
  REGISTER_LUA_CLASS_FUNCTION(Window::Lua_SetMinSize, "set_min_size");
  REGISTER_LUA_CLASS_FUNCTION(Window::Lua_GetMinSize, "get_min_size");
  REGISTER_LUA_CLASS_FUNCTION(Window::Lua_SetMaxSize, "set_max_size");
  REGISTER_LUA_CLASS_FUNCTION(Window::Lua_Destroy, "destroy");
  REGISTER_LUA_CLASS_FUNCTION(Window::Lua_DestroyChildren, "destroy_children");
  REGISTER_LUA_CLASS_FUNCTION(Window::Lua_Freeze, "freeze");
  REGISTER_LUA_CLASS_FUNCTION(Window::Lua_Thaw, "thaw");
  REGISTER_LUA_CLASS_FUNCTION(Window::Lua_SetEnabled, "set_enabled");
  REGISTER_LUA_CLASS_FUNCTION(Window::Lua_Enabled, "enabled");
  REGISTER_LUA_CLASS_FUNCTION(Window::Lua_SetFocus, "set_focus");
  REGISTER_LUA_CLASS_FUNCTION(Window::Lua_InFocus, "in_focus");
  REGISTER_LUA_CLASS_FUNCTION(Window::Lua_GetPosition, "get_position");
  REGISTER_LUA_CLASS_FUNCTION(Window::Lua_TopLevelWindow_SetTitle, "set_title");
  REGISTER_LUA_CLASS_FUNCTION(Window::Lua_TopLevelWindow_IsActive, "is_active");
  REGISTER_LUA_CLASS_FUNCTION(Window::Lua_TopLevelWindow_IsMaximized, "is_maximized");

  

  REGISTER_LUA_CLASS_FUNCTION(do_nothing, "set_background_colour");
  REGISTER_LUA_CLASS_FUNCTION(do_nothing, "get_best_size");
  REGISTER_LUA_CLASS_FUNCTION(do_nothing, "get_effective_min_size");
  REGISTER_LUA_CLASS_FUNCTION(do_nothing, "set_position");
  REGISTER_LUA_CLASS_FUNCTION(do_nothing, "center");
  REGISTER_LUA_CLASS_FUNCTION(do_nothing, "set_font_size");
  REGISTER_LUA_CLASS_FUNCTION(do_nothing, "set_font_family");
  REGISTER_LUA_CLASS_FUNCTION(do_nothing, "set_font_style");
  REGISTER_LUA_CLASS_FUNCTION(do_nothing, "set_font_weight");
  REGISTER_LUA_CLASS_FUNCTION(do_nothing, "set_font_underline");
  REGISTER_LUA_CLASS_FUNCTION(do_nothing, "set_font_face");
  REGISTER_LUA_CLASS_FUNCTION(do_nothing, "set_own_font_size");
  REGISTER_LUA_CLASS_FUNCTION(do_nothing, "set_own_font_family");
  REGISTER_LUA_CLASS_FUNCTION(do_nothing, "set_own_font_style");
  REGISTER_LUA_CLASS_FUNCTION(do_nothing, "set_own_font_weight");
  REGISTER_LUA_CLASS_FUNCTION(do_nothing, "set_own_font_underline");
  REGISTER_LUA_CLASS_FUNCTION(do_nothing, "set_own_font_face");
  REGISTER_LUA_CLASS_FUNCTION(do_nothing, "set_tool_tip");
  REGISTER_LUA_CLASS_FUNCTION(do_nothing, "popup_menu");
  REGISTER_LUA_CLASS_FUNCTION(do_nothing, "set_virtual_size");
  REGISTER_LUA_CLASS_FUNCTION(do_nothing, "set_virtual_size_hints");
  REGISTER_LUA_CLASS_FUNCTION(do_nothing, "toggle_style");
}

int Window::Lua_Set_Sizer(lua_State *L) {
  auto window = get_ews_object_from_top<Window>(L, 1);
  auto sizer = (Sizer *)lua_touserdata(L, 2);

  window->get_internal_object_type<wxWindow>()->SetSizer(sizer->get_internal_object_type<wxSizer>());

  return 0;
}

int Window::Lua_Set_Visible(lua_State* L) {
  auto window = get_ews_object_from_top<Window>(L, 1);
  auto new_visible = lua_toboolean(L, 2);

  window->get_internal_object_type<wxWindow>()->Show(new_visible);

  return 0;
}

int Window::Lua_Layout(lua_State* L) {
  auto window = get_ews_object_from_top<Window>(L, 1);

  window->get_internal_object_type<wxWindow>()->Layout();

  return 0;
}

int Window::Lua_Update(lua_State* L) {
  auto window = get_ews_object_from_top<Window>(L, 1);

  window->get_internal_object_type<wxWindow>()->Update();

  return 0;
}

int Window::Lua_Refresh(lua_State* L) {
  auto window = get_ews_object_from_top<Window>(L, 1);

  window->get_internal_object_type<wxWindow>()->Refresh();

  return 0;
}

int Window::Lua_Fit(lua_State* L) {
  auto window = get_ews_object_from_top<Window>(L, 1);

  window->get_internal_object_type<wxWindow>()->Fit();

  return 0;
}

int Window::Lua_FitInside(lua_State* L) {
  auto window = get_ews_object_from_top<Window>(L, 1);

  window->get_internal_object_type<wxWindow>()->FitInside();

  return 0;
}

int Window::Lua_SetSize(lua_State* L) {
  auto window = get_ews_object_from_top<Window>(L, 1);

  auto size = get_vec3_from_arg(L, 2);

  window->get_internal_object_type<wxWindow>()->SetSize(wxSize(size->x, size->y));

  return 0;
}

int Window::Lua_GetSize(lua_State* L) {
  auto window = get_ews_object_from_top<Window>(L, 1);

  wxSize size = window->get_internal_object_type<wxWindow>()->GetSize();

  dsl::push_lua_Vector3(L, Vector3(size.x, size.y, 0));

  return 1;
}

int Window::Lua_SetMinSize(lua_State* L) {
  auto window = get_ews_object_from_top<Window>(L, 1);

  auto size = get_vec3_from_arg(L, 2);

  window->get_internal_object_type<wxWindow>()->SetMinSize(wxSize(size->x, size->y));

  return 0;
}

int Window::Lua_GetMinSize(lua_State* L) {
  auto window = get_ews_object_from_top<Window>(L, 1);

  wxSize minSize = window->get_internal_object_type<wxWindow>()->GetMinSize();

  dsl::push_lua_Vector3(L, Vector3(minSize.x, minSize.y, 0));

  return 1;
}

int Window::Lua_SetMaxSize(lua_State* L) {
  auto window = get_ews_object_from_top<Window>(L, 1);

  auto size = get_vec3_from_arg(L, 2);

  window->get_internal_object_type<wxWindow>()->SetMaxSize(wxSize(size->x, size->y));

  return 0;
}

int Window::Lua_Destroy(lua_State* L) {
  auto window = get_ews_object_from_top<Window>(L, 1);

  window->get_internal_object_type<wxWindow>()->Destroy();

  return 0;
}

int Window::Lua_DestroyChildren(lua_State* L) {
  auto window = get_ews_object_from_top<Window>(L, 1);

  window->get_internal_object_type<wxWindow>()->DestroyChildren();

  return 0;
}

int Window::Lua_Freeze(lua_State* L) {
  auto window = get_ews_object_from_top<Window>(L, 1);

  window->get_internal_object_type<wxWindow>()->Freeze();

  return 0;
}

int Window::Lua_Thaw(lua_State* L) {
  auto window = get_ews_object_from_top<Window>(L, 1);

  window->get_internal_object_type<wxWindow>()->Thaw();

  return 0;
}

int Window::Lua_SetEnabled(lua_State* L) {
  auto window = get_ews_object_from_top<Window>(L, 1);
  auto enabled = lua_toboolean(L, 2);

  window->get_internal_object_type<wxWindow>()->Enable(enabled);

  return 0;
}

int Window::Lua_Enabled(lua_State* L) {
  auto window = get_ews_object_from_top<Window>(L, 1);

  lua_pushboolean(L, window->get_internal_object_type<wxWindow>()->IsEnabled());

  return 1;
}

int Window::Lua_SetFocus(lua_State* L) {
  auto window = get_ews_object_from_top<Window>(L, 1);

  window->get_internal_object_type<wxWindow>()->SetFocus();

  return 0;
}

int Window::Lua_InFocus(lua_State* L) {
  auto window = get_ews_object_from_top<Window>(L, 1);

  lua_pushboolean(L, window->get_internal_object_type<wxWindow>()->HasFocus());

  return 1;
}

int Window::Lua_GetPosition(lua_State* L) {
  auto window = get_ews_object_from_top<Window>(L, 1);

  lua_pushnumber(L, window->get_internal_object_type<wxWindow>()->GetPosition().x);
  lua_pushnumber(L, window->get_internal_object_type<wxWindow>()->GetPosition().y);

  return 2;
}

int Window::Lua_TopLevelWindow_SetTitle(lua_State* L) {
  auto frame = get_ews_object_from_top<Window>(L, 1);
  auto title = lua_tostring(L, 2);

  if (dynamic_cast<wxTopLevelWindow*>(frame->get_internal_object_type<wxWindow>()) == nullptr) {
    std::cout << "Attempt to call a TopLevelWindow function on a non-TopLevelWindow!!" << std::endl;
    return 0;
  }

  ((wxTopLevelWindow*)frame->get_internal_object_type<wxWindow>())->SetTitle(wxString(title));

  return 0;
}

int Window::Lua_TopLevelWindow_IsActive(lua_State* L) {
  auto frame = get_ews_object_from_top<Window>(L, 1);

  if (dynamic_cast<wxTopLevelWindow*>(frame->get_internal_object_type<wxWindow>()) == nullptr) {
    std::cout << "Attempt to call a TopLevelWindow function on a non-TopLevelWindow!!" << std::endl;
    lua_pushboolean(L, false);
    return 1;
  }

  lua_pushboolean(L, ((wxTopLevelWindow*)frame->get_internal_object_type<wxWindow>())->IsActive());
  return 1;
}

int Window::Lua_TopLevelWindow_IsMaximized(lua_State* L) {
  auto frame = get_ews_object_from_top<Window>(L, 1);

  if (dynamic_cast<wxTopLevelWindow*>(frame->get_internal_object_type<wxWindow>()) == nullptr) {
    std::cout << "Attempt to call a TopLevelWindow function on a non-TopLevelWindow!!" << std::endl;
    lua_pushboolean(L, false);
    return 1;
  }

  lua_pushboolean(L, ((wxTopLevelWindow*)frame->get_internal_object_type<wxWindow>())->IsMaximized());
  return 1;
}


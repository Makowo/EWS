#include "Window.h"

#include "Sizer.h"

static const std::vector<luaL_Reg> methods = {
  {"EWS_Window_Set_Sizer", Window::Lua_Set_Sizer},
  {"EWS_Window_Set_Visible", Window::Lua_Set_Visible},

  {"EWS_Window_Layout", Window::Lua_Layout},
  {"EWS_Window_Update", Window::Lua_Update},
  {"EWS_Window_Refresh", Window::Lua_Refresh},

  {"EWS_Window_Fit", Window::Lua_Fit},
  {"EWS_Window_FitInside", Window::Lua_FitInside},

  {"EWS_Window_SetSize", Window::Lua_SetSize},
  {"EWS_Window_GetSize", Window::Lua_GetSize},

  {"EWS_Window_SetMinSize", Window::Lua_SetMinSize},
  {"EWS_Window_GetMinSize", Window::Lua_GetMinSize},

  {"EWS_Window_SetMaxSize", Window::Lua_SetMaxSize},

  {"EWS_Window_Destroy", Window::Lua_Destroy},
  {"EWS_Window_DestroyChildren", Window::Lua_DestroyChildren},
  {"EWS_Window_Freeze", Window::Lua_Freeze},
  {"EWS_Window_Thaw", Window::Lua_Thaw},

  {"EWS_Window_SetEnabled", Window::Lua_SetEnabled},
  {"EWS_Window_Enabled", Window::Lua_Enabled},

  {"EWS_Window_SetFocus", Window::Lua_SetFocus},
  {"EWS_Window_InFocus", Window::Lua_InFocus},

  {"EWS_Window_GetPosition", Window::Lua_GetPosition},

  {"EWS_TopLevelWindow_SetTitle", Window::Lua_TopLevelWindow_SetTitle},
  {"EWS_TopLevelWindow_IsActive", Window::Lua_TopLevelWindow_IsActive},
  {"EWS_TopLevelWindow_IsMaximized", Window::Lua_TopLevelWindow_IsMaximized},
};

ADD_FUNCS_AUTOFILL(Window::Add_Window_Funcs)

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

  lua_pushnumber(L, window->get_internal_object_type<wxWindow>()->GetSize().x);
  lua_pushnumber(L, window->get_internal_object_type<wxWindow>()->GetSize().y);

  return 2;
}

int Window::Lua_SetMinSize(lua_State* L) {
  auto window = get_ews_object_from_top<Window>(L, 1);

  auto size = get_vec3_from_arg(L, 2);

  window->get_internal_object_type<wxWindow>()->SetMinSize(wxSize(size->x, size->y));

  return 0;
}

int Window::Lua_GetMinSize(lua_State* L) {
  auto window = get_ews_object_from_top<Window>(L, 1);

  lua_pushnumber(L, window->get_internal_object_type<wxWindow>()->GetMinSize().x);
  lua_pushnumber(L, window->get_internal_object_type<wxWindow>()->GetMinSize().y);

  return 2;
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


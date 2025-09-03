#include "ToolBar.h"

#include "EventHandler.h"

#include "Control.h"

void ToolBar::AddLuaFunctions(lua_State* L)
{
  Window::AddLuaFunctions(L);
  REGISTER_LUA_CLASS_FUNCTION(ToolBar::Lua_AddTool, "add_tool");
  REGISTER_LUA_CLASS_FUNCTION(ToolBar::Lua_AddCheckTool, "add_check_tool");
  REGISTER_LUA_CLASS_FUNCTION(ToolBar::Lua_AddRadioTool, "add_radio_tool");
  REGISTER_LUA_CLASS_FUNCTION(ToolBar::Lua_AddSeparator, "add_separator");
  REGISTER_LUA_CLASS_FUNCTION(ToolBar::Lua_AddControl, "add_control");
  REGISTER_LUA_CLASS_FUNCTION(ToolBar::Lua_GetToolState, "tool_state");
  REGISTER_LUA_CLASS_FUNCTION(ToolBar::Lua_SetToolState, "set_tool_state");
  REGISTER_LUA_CLASS_FUNCTION(ToolBar::Lua_SetToolEnabled, "set_tool_enabled");
  REGISTER_LUA_CLASS_FUNCTION(ToolBar::Lua_Realize, "realize");
}

int ToolBar::Lua_Create(lua_State* L) {
  Window* parent = get_ews_object_from_top<Window>(L, 2);
  std::string id = lua_tostring(L, 3);
  std::string style = lua_tostring(L, 4);

  
  auto toolbar = create_new_ews_object<ToolBar>(L);
  toolbar->init(parent, id, style);

  return 1;
}

int ToolBar::Lua_AddSeparator(lua_State* L) {
  ToolBar* tb = get_ews_object_from_top<ToolBar>(L, 1);

  tb->get_internal_object_type<wxToolBar>()->AddSeparator();
  tb->get_internal_object_type<wxToolBar>()->Realize();
  return 0;
}

int ToolBar::Lua_AddTool(lua_State* L) {
  ToolBar* tb = get_ews_object_from_top<ToolBar>(L, 1);
  std::string id = lua_tostring(L, 2);
  std::string name = lua_tostring(L, 3);
  std::string image_path = lua_tostring(L, 4);
  std::string description;
  if (lua_type(L, 5) == LUA_TSTRING)
    description = lua_tostring(L, 5);

  DISABLE_IMAGE_LOAD_WARNING
  wxImage icon = get_wximage_from_path(resolve_icon_path(image_path));

  // TODO: add id to tool
  tb->get_internal_object_type<wxToolBar>()->AddTool(get_winid_from_string(id), wxString(name), icon, wxString(description));
  //tb->internal_toolbar->InsertTool(tb->internal_toolbar->GetToolsCount(), -1, wxString(name), wxBitmap(icon), wxBitmap(icon), wxITEM_NORMAL, wxString(description), wxString(description));

  tb->get_internal_object_type<wxToolBar>()->Realize();

  //tb->internal_toolbar->AddTool()

  return 0;
}

int ToolBar::Lua_AddCheckTool(lua_State* L) {
  ToolBar *tb = get_ews_object_from_top<ToolBar>(L, 1);
  std::string id = lua_tostring(L, 2);
  std::string name = lua_tostring(L, 3);
  std::string image_path = lua_tostring(L, 4);
  std::string description;
  if(lua_type(L, 5) == LUA_TSTRING)
    description = lua_tostring(L, 5);


  DISABLE_IMAGE_LOAD_WARNING
  wxImage icon = get_wximage_from_path(resolve_icon_path(image_path));

  tb->get_internal_object_type<wxToolBar>()->AddCheckTool(get_winid_from_string(id), wxString(name), wxBitmap(icon), wxBitmap(icon), wxString(description), wxString(description));
  tb->get_internal_object_type<wxToolBar>()->Realize();
  return 0;
}

int ToolBar::Lua_AddRadioTool(lua_State *L) {
  ToolBar *tb = get_ews_object_from_top<ToolBar>(L, 1);
  std::string id = lua_tostring(L, 2);
  std::string name = lua_tostring(L, 3);
  std::string image_path = lua_tostring(L, 4);
  std::string description;
  if (lua_type(L, 5) == LUA_TSTRING)
    description = lua_tostring(L, 5);

  DISABLE_IMAGE_LOAD_WARNING
  wxImage icon = get_wximage_from_path(resolve_icon_path(image_path));

  tb->get_internal_object_type<wxToolBar>()->AddRadioTool(get_winid_from_string(id), wxString(name), wxBitmap(icon), wxBitmap(icon), wxString(description), wxString(description));
  tb->get_internal_object_type<wxToolBar>()->Realize();
  return 0;
}

int ToolBar::Lua_AddControl(lua_State* L) {
  auto toolbar = get_ews_object_from_top<ToolBar>(L, 1);
  auto control = get_ews_object_from_top<Control>(L, 2);

  toolbar->get_internal_object_type<wxToolBar>()->AddControl(control->get_internal_object_type<wxControl>());

  return 0;
}

int ToolBar::Lua_Realize(lua_State* L) {
  ToolBar* tb = get_ews_object_from_top<ToolBar>(L, 1);

  tb->get_internal_object_type<wxToolBar>()->Realize();

  return 0;
}

int ToolBar::Lua_GetToolState(lua_State* L) {
  auto toolbar = get_ews_object_from_top<ToolBar>(L, 1);
  auto id = lua_tostring(L, 2);

  lua_pushboolean(L, toolbar->get_internal_object_type<wxToolBar>()->GetToolState(get_winid_from_string(id)));

  return 1;
}

int ToolBar::Lua_SetToolState(lua_State* L) {
  auto toolbar = get_ews_object_from_top<ToolBar>(L, 1);
  auto id = lua_tostring(L, 2);
  auto state = lua_toboolean(L, 3);

  toolbar->get_internal_object_type<wxToolBar>()->EnableTool(get_winid_from_string(id), state);

  return 0;
}

int ToolBar::Lua_SetToolEnabled(lua_State* L)
{
  auto toolbar = get_ews_object_from_top<ToolBar>(L, 1);
  auto id = lua_tostring(L, 2);
  auto enabled = lua_toboolean(L, 3);

  toolbar->get_internal_object_type<wxToolBar>()->EnableTool(get_winid_from_string(id), enabled);

  return 0;
}

void ToolBar::init(Window* parent, std::string name, std::string style) {
  auto style_value = get_style_from_merged_list(style);
  this->set_internal_object_type(new wxToolBar(parent->get_internal_object_type<wxWindow>(), wxID_ANY, wxDefaultPosition, wxDefaultSize, style_value, wxString(name)));
}
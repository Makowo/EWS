#include "Frame.h"
#include "MenuBar.h"
#include "ToolBar.h"
#include "EventHandler.h"
#include "StatusBar.h"

#include <rendering.h>

void Frame::AddLuaFunctions(lua_State* L)
{
  Window::AddLuaFunctions(L);
  REGISTER_LUA_CLASS_FUNCTION(Frame::Lua_Set_Icon, "set_icon");
  REGISTER_LUA_CLASS_FUNCTION(Frame::Lua_Set_MenuBar, "set_menu_bar");
  REGISTER_LUA_CLASS_FUNCTION(Frame::Lua_Set_ToolBar, "set_tool_bar");
  REGISTER_LUA_CLASS_FUNCTION(Frame::Lua_Set_StatusBar, "set_status_bar");
  REGISTER_LUA_CLASS_FUNCTION(Frame::Lua_IsIconized, "is_iconized");
}


int Frame::Lua_Create(lua_State *L)
{
  auto name = std::string(lua_tostring(L, 2));
  //std::cout << "[Frame::Lua_Create] name" << std::endl;
  //auto start_pos = get_vec3_from_arg(L, 3);
  auto start_pos = Vector3(0, 0, 0);
  //std::cout << "[Frame::Lua_Create] startpos: x: " << start_pos->x << " y: " << start_pos->y << " z: " << start_pos->z << std::endl;
  //auto size = get_vec3_from_arg(L, 4);
  auto size = Vector3(0, 0, 0);
  //std::cout << "[Frame::Lua_Create] size: x: " << start_pos->x << " y: " << start_pos->y << " z: " << start_pos->z << std::endl;
  auto style = std::string(lua_tostring(L, 5));
  //std::cout << "[Frame::Lua_Create] style" << std::endl;

  auto parent = (Frame *)nullptr;
  if (lua_type(L, 6) != LUA_TNIL)
  {
    parent = get_ews_object_from_top<Frame>(L, 6);
  }
  //std::cout << "[Frame::Lua_Create] lua_type" << std::endl;

  auto final_frame = create_new_ews_object<Frame>(L);
  //auto final_frame = new Frame();
  //std::cout << "[Frame::Lua_Create] lua_newuserdata: " << final_frame << std::endl;

  final_frame->init(name, &start_pos, &size, style, parent);
  //std::cout << "[Frame::Lua_Create] init" << std::endl;

  return 1;
}

int Frame::Lua_Set_ToolBar(lua_State* L) {
  Frame* frame = get_ews_object_from_top<Frame>(L, 1);
  ToolBar* toolbar = get_ews_object_from_top<ToolBar>(L, 2);

  frame->get_internal_object_type<wxFrame>()->SetToolBar(toolbar->get_internal_object_type<wxToolBar>());
  return 0;
}

int Frame::Lua_Set_Icon(lua_State* L) {
  auto frame = get_ews_object_from_top<Frame>(L, 1);
  auto icon = lua_tostring(L, 2);

  wxLogNull stopiCCPWarning;
  wxIcon icon2;
  icon2.LoadFile(wxString(resolve_icon_path(icon)), wxBITMAP_TYPE_PNG);
  frame->get_internal_object_type<wxFrame>()->SetIcon(icon2);
  return 0;
}

int Frame::Lua_Set_MenuBar(lua_State* L) {
  auto frame = get_ews_object_from_top<Frame>(L, 1);
  auto menubar = get_ews_object_from_top<MenuBar>(L, 2);

  frame->get_internal_object_type<wxFrame>()->SetMenuBar(menubar->get_internal_object_type<wxMenuBar>());
  return 0;
}

int Frame::Lua_Set_StatusBar(lua_State* L) {
  auto frame = get_ews_object_from_top<Frame>(L, 1);
  auto status_bar = get_ews_object_from_top<StatusBar>(L, 2);

  frame->get_internal_object_type<wxFrame>()->SetStatusBar(status_bar->get_internal_object_type<wxStatusBar>());

  return 0;
}

int Frame::Lua_IsIconized(lua_State* L) {
  auto frame = get_ews_object_from_top<Frame>(L, 1);

  lua_pushboolean(L, frame->get_internal_object_type<wxFrame>()->IsIconized());

  return 1;
}

dsl::Win32Window* get_dsl_window_from_hwnd(HWND window);
extern HWND real_raid_window;

void Frame::init(std::string name, Vector3* start_pos, Vector3* size, std::string style, Frame* parent) {
  std::cout << "Creating frame: " + name << std::endl;
  auto parent2 = (wxWindow*)nullptr;
  if(parent != nullptr)
    parent2 = parent->get_internal_object_type<wxWindow>();

  this->set_internal_object_type(new wxFrame(parent2, wxID_ANY, wxString(name.c_str()), wxPoint(start_pos->x, start_pos->y), wxSize(size->x, size->y), get_style_from_merged_list(style)));
  if (name == "World Editor") {
    wxTheApp->SetTopWindow(this->get_internal_object_type<wxFrame>());
    this->get_internal_object_type<wxFrame>()->Bind(wxEVT_ACTIVATE, [this](wxActivateEvent& evt) {

      if (get_dsl_window_from_hwnd(real_raid_window)) {
        get_dsl_window_from_hwnd(real_raid_window)->_activate_state = evt.GetActive();
        get_dsl_window_from_hwnd(real_raid_window)->_activation_altered = true;
      }
      evt.Skip();
      });
  }
}
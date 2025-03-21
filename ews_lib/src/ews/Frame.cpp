#include "Frame.h"
#include "MenuBar.h"
#include "ToolBar.h"
#include "EventHandler.h"
#include "StatusBar.h"

#include <rendering.h>

static const std::vector<luaL_Reg> Frame_methods = {
  {"EWS_Frame_Set_Icon", Frame::Lua_Set_Icon},
  {"EWS_Frame_Set_MenuBar", Frame::Lua_Set_MenuBar},
  {"EWS_Frame_Set_ToolBar", Frame::Lua_Set_ToolBar},

  {"EWS_Frame_Set_StatusBar", Frame::Lua_Set_StatusBar},

  {"EWS_Frame_IsIconized", Frame::Lua_IsIconized }
};

void Frame::Add_Frame_Funcs(lua_State* L)
{
  for (int i = 0; i < Frame_methods.size(); i++)
  {
    lua_register(L, Frame_methods[i].name, Frame_methods[i].func);
  }
}

int Frame::Lua_Create(lua_State *L)
{
  auto name = std::string(lua_tostring(L, 1));
  //std::cout << "[Frame::Lua_Create] name" << std::endl;
  auto start_pos = get_vec3_from_arg(L, 2);
  //std::cout << "[Frame::Lua_Create] startpos: x: " << start_pos->x << " y: " << start_pos->y << " z: " << start_pos->z << std::endl;
  auto size = get_vec3_from_arg(L, 3);
  //std::cout << "[Frame::Lua_Create] size: x: " << start_pos->x << " y: " << start_pos->y << " z: " << start_pos->z << std::endl;
  auto style = std::string(lua_tostring(L, 4));
  //std::cout << "[Frame::Lua_Create] style" << std::endl;

  auto parent = (Frame *)nullptr;
  if (lua_type(L, 5) != LUA_TNIL)
  {
    parent = get_ews_object_from_top<Frame>(L, 5);
  }
  //std::cout << "[Frame::Lua_Create] lua_type" << std::endl;

  auto final_frame = create_new_ews_object<Frame>(L);
  //auto final_frame = new Frame();
  //std::cout << "[Frame::Lua_Create] lua_newuserdata: " << final_frame << std::endl;

  final_frame->init(name, start_pos, size, style, parent);
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
      get_dsl_window_from_hwnd(real_raid_window)->_activate_state = evt.GetActive();
      get_dsl_window_from_hwnd(real_raid_window)->_activation_altered = true;
      evt.Skip();
      });
  }
}
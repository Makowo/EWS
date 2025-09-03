#include <wx/wx.h>

#include "ews_utils.h"
#include "component.h"
#include "EventHandler.h"
#include "Window.h"

#include <unordered_map>
#include <vector>

#include <wx/tglbtn.h>

#include <unordered_map>

std::unordered_map<std::string, wxWindowID> winids;
int get_winid_from_string(const char* str) {
  if (std::string(str) == "") return wxID_ANY;
  if (winids.contains(str))
    return winids[str];

  auto id = wxWindow::NewControlId();

  winids.insert({ str, id });

  return id;
  //return wxWindow::NewControlId
}

int get_winid_from_string(const std::string& str) {
  return get_winid_from_string(str.c_str());
}

struct EWSRegisteredEventData final : public wxObject {
  lua_State* L;
  Component* ews_window;
  std::string window_id;
  std::string event;
  int lua_callback_ref;

  EWSRegisteredEventData(lua_State* L, Component* ews_window, const std::string& window_id, const std::string& event, int lua_callback_ref) : L(L), ews_window(ews_window), window_id(window_id), event(event), lua_callback_ref(lua_callback_ref) {}
};

void print_stack(lua_State* L) {
  lua_getglobal(L, "EWS_LuaSided_CHelper_Print_Stack");
  lua_pcall(L, 0, 0, 0);
}

//std::cout << "Pushing callback with ref id: " << event_data->lua_callback_ref << std::endl;
//std::cout << "Pushing string: " << event_data->window_id << std::endl;
#define DEFINE_EVENT_FUNCTION(diesel_event_type, wxEvtArgumentType, skipAtEnd) \
void EWS_##diesel_event_type(wxEvtArgumentType& evt) {\
  EWSRegisteredEventData* event_data = (EWSRegisteredEventData*)evt.GetEventUserData();\
  auto winid = get_winid_from_string(event_data->window_id);\
  \
  auto wnd = wxWindow::FindWindowById(evt.GetId());\
\
  bool menu = dynamic_cast<wxMenu*>(evt.GetEventObject()) != nullptr; /* if the event object is a wxMenu, then skip checking to see if the event object is the same as the EWS object that the ui was created with, as wxMenu events propagate to the wxFrame that contains them.*/ \
\
  if ((evt.GetId() == winid || wnd == event_data->ews_window->get_internal_object_type<wxWindow>()) && (evt.GetEventObject() == event_data->ews_window->get_internal_object_type<wxWindow>() || menu)) { \
    int pre_pcall_stack = lua_gettop(event_data->L); \
    lua_rawgeti(event_data->L, LUA_REGISTRYINDEX, event_data->lua_callback_ref); \
    lua_pushstring(event_data->L, event_data->window_id.c_str()); \
    if (lua_pcall(event_data->L, 1, LUA_MULTRET, 0) != 0) { \
      std::cout << "Error occurred while processing event " << event_data->event << " for EWS object with id " << event_data->window_id << std::endl; \
      std::cout << lua_tostring(event_data->L, -1) << std::endl; \
      print_stack(event_data->L); \
    } \
    int returned_values = lua_gettop(event_data->L) - pre_pcall_stack; \
    lua_pop(event_data->L, returned_values); /* dispose of any returned values from functions, as they are not used */\
  } \
  if (skipAtEnd) evt.Skip(); \
}

DEFINE_EVENT_FUNCTION(EVT_CLOSE_WINDOW, wxCloseEvent, false)
DEFINE_EVENT_FUNCTION(EVT_LEAVE_WINDOW, wxMouseEvent, true)
DEFINE_EVENT_FUNCTION(EVT_ENTER_WINDOW, wxMouseEvent, true)

DEFINE_EVENT_FUNCTION(EVT_KILL_FOCUS, wxFocusEvent, false)
DEFINE_EVENT_FUNCTION(EVT_ACTIVATE, wxActivateEvent, true)

void EWS_EVT_COMMAND_MENU_SELECTED(wxCommandEvent& evt) {
  EWSRegisteredEventData* event_data = (EWSRegisteredEventData*)evt.GetEventUserData();
  auto winid = get_winid_from_string(event_data->window_id);
  auto wnd = wxWindow::FindWindowById(evt.GetId());
  bool menu = dynamic_cast<wxMenu*>(evt.GetEventObject()) != nullptr;
  if ((evt.GetId() == winid || wnd == event_data->ews_window->get_internal_object_type<wxWindow>()) && (evt.GetEventObject() == event_data->ews_window->get_internal_object_type<wxWindow>() || menu)) {
    int pre_pcall_stack = lua_gettop(event_data->L);
    lua_rawgeti(event_data->L, (-10000), event_data->lua_callback_ref);
    //lua_pushvalue(event_data->L, -1);
    lua_pushstring(event_data->L, event_data->window_id.c_str());
    //lua_settop(event_data->L, 2);
    if (lua_pcall(event_data->L, 1, (-1), 0) != 0) {
      std::cout << "Error occurred while processing event " << event_data->event << " for EWS object with id " << event_data->window_id << std::endl; std::cout << lua_tolstring(event_data->L, (-1), 0) << std::endl; print_stack(event_data->L);
    }
    int returned_values = lua_gettop(event_data->L) - pre_pcall_stack;
    lua_settop(event_data->L, -(returned_values)-1);
  }
}
DEFINE_EVENT_FUNCTION(EVT_COMMAND_COMBOBOX_SELECTED, wxCommandEvent, false)
DEFINE_EVENT_FUNCTION(EVT_COMMAND_BUTTON_CLICKED, wxCommandEvent, false)
DEFINE_EVENT_FUNCTION(EVT_MENU, wxCommandEvent, false)
DEFINE_EVENT_FUNCTION(EVT_TOGGLEBUTTON, wxCommandEvent, false)
DEFINE_EVENT_FUNCTION(EVT_KEY_DOWN, wxKeyEvent, false)


std::vector<EWSRegisteredEventData*> registered_events;
int EWS_EventHandler_Connect(lua_State* L) {
  auto wxobject = get_ews_object_from_top<Component>(L, 1);

  const char* id = nullptr;
  const char* event_name = nullptr;
  int ref = 0;
  void* extra_data = nullptr;

  if (lua_type(L, 3) == LUA_TSTRING) {
    id = lua_tostring(L, 2);
    event_name = lua_tostring(L, 3);
    
    lua_pushvalue(L, 4);
    ref = luaL_ref(L, LUA_REGISTRYINDEX);
  }
  else {
    id = "";
    event_name = lua_tostring(L, 2);
    lua_pushvalue(L, 3);
    ref = luaL_ref(L, LUA_REGISTRYINDEX);


  }

  //lua_pop(L, 4);

  //auto eventdata = new RegisteredEventData{ L, wxobject, ref };
  //
  //wxobject->internal_object->Bind(wxEVT_CLOSE_WINDOW, EWS_EVT_CLOSE_WINDOW, -1, -1, (wxObject*)eventdata);

  EWSRegisteredEventData* event_data = new EWSRegisteredEventData(L, wxobject, id, event_name, ref);

  registered_events.push_back(event_data);

  auto evt_handler = wxobject->get_internal_object_type<wxEvtHandler>();

//#define DECLARE_EVENT_BIND(diesel_event_type) \
//if(!strcmp(event_name, #diesel_event_type)) \
//  evt_handler->Bind(wx##diesel_event_type, EWS_##diesel_event_type, get_winid_from_string(id), get_winid_from_string(id), event_data);

#define DECLARE_EVENT_BIND(diesel_event_type) \
if(!strcmp(event_name, #diesel_event_type)) \
  evt_handler->Bind(wx##diesel_event_type, [](auto& evt) {\
    EWS_##diesel_event_type(evt);\
    }, get_winid_from_string(id), get_winid_from_string(id), event_data);
#define DECLARE_EVENT_BIND_DEPRECATED(diesel_event_type, new_name) \
if(!strcmp(event_name, #diesel_event_type)) \
  evt_handler->Bind(wx##new_name, [](auto& evt) {\
    EWS_##new_name(evt);\
    }, get_winid_from_string(id), get_winid_from_string(id), event_data);

  DECLARE_EVENT_BIND(EVT_COMMAND_MENU_SELECTED)
  DECLARE_EVENT_BIND(EVT_CLOSE_WINDOW)

  DECLARE_EVENT_BIND(EVT_LEAVE_WINDOW)
  DECLARE_EVENT_BIND(EVT_ENTER_WINDOW)

  DECLARE_EVENT_BIND(EVT_KILL_FOCUS)
  DECLARE_EVENT_BIND(EVT_ACTIVATE)

  DECLARE_EVENT_BIND(EVT_MENU)
  DECLARE_EVENT_BIND(EVT_COMMAND_COMBOBOX_SELECTED)
  DECLARE_EVENT_BIND(EVT_COMMAND_BUTTON_CLICKED)
  DECLARE_EVENT_BIND_DEPRECATED(EVT_COMMAND_TOGGLEBUTTON_CLICKED, EVT_TOGGLEBUTTON)
  DECLARE_EVENT_BIND(EVT_KEY_DOWN)

  return 0;
}

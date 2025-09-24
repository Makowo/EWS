#include "EWS.h"

#include <vector>
#include <wx/wx.h>
#include <HashlistLib.h>

#include "ews/EventHandler.h"

#include "ews/Window.h"
#include "ews/Frame.h"
#include "ews/Panel.h"

#include "ews/Sizer.h"
#include "ews/BoxSizer.h"
#include "ews/StaticBoxSizer.h"

#include "ews/Notebook.h"
#include "ews/SplitterWindow.h"
#include "ews/StaticText.h"
#include "ews/AppWindow.h"
#include "ews/ScrolledWindow.h"

#include "ews/Dialog.h"
#include "ews/MessageDialog.h"
#include "ews/ProgressDialog.h"
#include "ews/FileDialog.h"

#include "ews/Menu.h"
#include "ews/MenuBar.h"
#include "ews/ToolBar.h"

#include "ews/Control.h"
#include "ews/StatusBar.h"

#include "ews/TextCtrl.h"
#include "ews/SpinCtrl.h"
#include "ews/ListCtrl.h"
#include "ews/TreeCtrl.h"

#include "ews/ComboBox.h"
#include "ews/ListBox.h"
#include "ews/CheckBox.h"

#include "ews/Slider.h"

#include "ews/AnyButton.h"
#include "ews/Button.h"
#include "ews/ToggleButton.h"
#include "ews/BitmapButton.h"
#include "ews/SpinButton.h"
#include "ews/RadioButton.h"

#include "rendering.h"

#include <filesystem>

#include <shellapi.h>

int EWS_Lua_SystemFileExists(lua_State* L);
int EWS_Lua_LaunchURL(lua_State* L);
int EWS_Lua_MessageBox(lua_State* L);
int EWS_Lua_NameToKeyCode(lua_State* L);
int EWS_Lua_GetTextFromUser(lua_State* L);

static const std::vector<luaL_Reg> EWS_methods = {
  {"Frame", Frame::Lua_Create},
  {"BoxSizer", BoxSizer::Lua_Create},
  {"StaticBoxSizer", StaticBoxSizer::Lua_Create},
  {"Dialog", Dialog::Lua_Create},
  {"MessageDialog", MessageDialog::Lua_Create},
  {"MenuBar", MenuBar::Lua_Create},
  {"Menu", Menu::Lua_Create},
  {"Panel", Panel::Lua_Create},
  {"ToolBar", ToolBar::Lua_Create},
  {"Notebook", Notebook::Lua_Create},
  {"SplitterWindow", SplitterWindow::Lua_Create},
  {"StaticText", StaticText::Lua_Create},
  {"AppWindow", AppWindow::Lua_Create},
  {"TextCtrl", TextCtrl::Lua_Create},
  {"Button", Button::Lua_Create},
  {"ToggleButton", ToggleButton::Lua_Create},
  {"ScrolledWindow", ScrolledWindow::Lua_Create},
  {"StatusBar", StatusBar::Lua_Create},
  {"ProgressDialog", ProgressDialog::Lua_Create},
  {"FileDialog", FileDialog::Lua_Create},
  {"ComboBox", ComboBox::Lua_Create},
  {"CheckBox", CheckBox::Lua_Create},
  {"SpinCtrl", SpinCtrl::Lua_Create},
  {"BitmapButton", BitmapButton::Lua_Create},
  {"ListBox", ListBox::Lua_Create},
  {"ListCtrl", ListCtrl::Lua_Create},
  {"TreeCtrl", TreeCtrl::Lua_Create},
  {"Slider", Slider::Lua_Create},
  {"SpinButton", SpinButton::Lua_Create},
  {"RadioButton", RadioButton::Lua_Create},

  {"system_file_exists", EWS_Lua_SystemFileExists},
  {"launch_url", EWS_Lua_LaunchURL},
  {"message_box", EWS_Lua_MessageBox},
  {"name_to_key_code", EWS_Lua_NameToKeyCode},
  {"get_text_from_user", EWS_Lua_GetTextFromUser}
};

int EWS_Lua_SystemFileExists(lua_State* L) {
  auto path = lua_tostring(L, 1);
  if (path == nullptr) {
    //std::cout << "File: " << path << " does not exist." << std::endl;
    lua_pushboolean(L, false);
    return 1;
  }
  lua_pushboolean(L, std::filesystem::exists(path));
  return 1;
}

int EWS_EWSModExistsCookie(lua_State* L) { return 0; }

int EWS_Lua_GetTextFromUser(lua_State* L) {
  auto parent = get_ews_object_from_top<Window>(L, 1);

  auto message = lua_tostring(L, 2);
  auto caption = lua_tostring(L, 3);
  auto default_value = lua_tostring(L, 4);

  auto start_pos = get_vec3_from_arg(L, 5);

  auto centre = lua_toboolean(L, 6);


  wxWindow* parent2 = nullptr;
  if (parent) parent2 = parent->get_internal_object_type<wxWindow>();

  auto text = wxGetTextFromUser(message, caption, default_value, parent2, start_pos->x, start_pos->y, centre);

  lua_pushstring(L, text);

  return 1;
}

int EWS_Lua_SetEWSWindow(lua_State* L) {
  auto appwindow = get_ews_object_from_top<AppWindow>(L, 1);

  set_appwindow(appwindow);

  return 0;
}

int EWS_AddStringToHashlist(lua_State* L) {
  auto str = lua_tostring(L, 1);

  add_string_to_hashlist(str);

  return 0;
}

LSTATUS GetStringValueOfRegistryKey(HKEY key, const wchar_t* keyPath, const wchar_t* keyValueName, WCHAR* valueOut, DWORD valueOutSize) {
  LSTATUS status = RegGetValueW(key, keyPath, keyValueName, RRF_RT_REG_SZ, nullptr, valueOut, &valueOutSize);

  return status;
}

int EWS_Lua_LaunchURL(lua_State* L) {
  const char* url = lua_tostring(L, 1);
  if (!url)
    return 0;


  /*WCHAR progID[256]{};
  auto result = GetStringValueOfRegistryKey(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\Shell\\Associations\\UrlAssociations\\https\\UserChoice", L"ProgID", progID, sizeof(progID));

  if (result == ERROR_FILE_NOT_FOUND)
    return 0;

  WCHAR browserShellCmd[256]{};
  std::wstring progIDStr = std::wstring(progID, wcslen(progID));
  std::wstring browserShellCmdPath = progIDStr + std::wstring(L"\\shell\\open\\command");
  result = GetStringValueOfRegistryKey(HKEY_CLASSES_ROOT, browserShellCmdPath.c_str(), L"", browserShellCmd, sizeof(browserShellCmd));

  if (result == ERROR_FILE_NOT_FOUND)
    return 0;

  std::wstring browserCommand(browserShellCmd, sizeof(browserShellCmd));
  browserCommand.replace(browserCommand.find(L"%1", 0), 2, std::wstring(url, url + strnlen_s(url, 256)));*/

  //ShellExecuteW(0, 0, browserCommand.c_str(), 0, 0, SW_HIDE);

  ShellExecuteA(0, "open", url, 0, 0, SW_HIDE);

  return 0;
}

int EWS_Lua_MessageBox(lua_State* L) {
  // parent
  // message
  // caption
  // style
  // start_pos

  Window* parent = get_ews_object_from_top<Window>(L, 2);
  const char* message = lua_tostring(L, 3);
  const char* caption = lua_tostring(L, 4);
  const char* style = lua_tostring(L, 5);
  Vector3* position = get_vec3_from_arg(L, 6);


  int code = wxMessageBox(message, caption, get_style_from_merged_list(style), parent->get_internal_object_type<wxWindow>(), position->x, position->y);

  int returnargs = 0;

  if (code == wxCANCEL) {
    lua_pushstring(L, "CANCEL");
    returnargs++;
  }
  else if (code == wxNO) {
    lua_pushstring(L, "NO");
    returnargs++;
  }
  else if (code == wxYES) {
    lua_pushstring(L, "YES");
    returnargs++;
  }

  return returnargs;
}
int EWS_Lua_NameToKeyCode(lua_State* L) {
  const char* name = lua_tostring(L, 2);

  lua_pushnumber(L, 0);

  return 1;
}

void EWS::add_members(lua_State* L) {
  lua_pushlightuserdata(L, nullptr);
  lua_createtable(L, 0, 0);
  lua_pushvalue(L, -1);

  lua_setmetatable(L, -3);

  lua_getglobal(L, "__classes");
  lua_pushvalue(L, -2);
  lua_setfield(L, -2, "EWS");
  lua_pop(L, 1);

  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");

  for (auto& method : EWS_methods) {
    lua_pushcclosure(L, method.func, 0);
    lua_setfield(L, -2, method.name);
  }

  lua_pop(L, 1);

  lua_setglobal(L, "EWS");

  lua_settop(L, 0);
  
  return;
  // EWS functions, added in order of when they're called

  lua_register(L, "EWS_EWSModExistsCookie", EWS_EWSModExistsCookie);

  lua_register(L, "EWS_SetEWSWindow", EWS_Lua_SetEWSWindow);

  lua_register(L, "EWS_AddStringToHashlist", EWS_AddStringToHashlist);

}
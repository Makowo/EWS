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

static const std::vector<luaL_Reg> EWS_methods = {
  {"EWS_New_Frame", Frame::Lua_Create},
  {"EWS_New_BoxSizer", BoxSizer::Lua_Create},
  {"EWS_New_StaticBoxSizer", StaticBoxSizer::Lua_Create},
  {"EWS_New_Dialog_Simple", Dialog::Lua_Create_Simple},
  {"EWS_New_Dialog_Complex", Dialog::Lua_Create_Complex},
  {"EWS_New_MessageDialog", MessageDialog::Lua_Create},
  {"EWS_New_MenuBar", MenuBar::Lua_Create},
  {"EWS_New_Menu", Menu::Lua_Create},
  {"EWS_New_Panel", Panel::Lua_Create},
  {"EWS_New_ToolBar", ToolBar::Lua_Create},
  {"EWS_New_Notebook", Notebook::Lua_Create},
  {"EWS_New_SplitterWindow", SplitterWindow::Lua_Create},
  {"EWS_New_StaticText", StaticText::Lua_Create},
  {"EWS_New_AppWindow", AppWindow::Lua_Create},
  {"EWS_New_TextCtrl", TextCtrl::Lua_Create},
  {"EWS_New_Button", Button::Lua_Create},
  {"EWS_New_ToggleButton", ToggleButton::Lua_Create},
  {"EWS_New_ScrolledWindow", ScrolledWindow::Lua_Create},
  {"EWS_New_StatusBar", StatusBar::Lua_Create},
  {"EWS_New_ProgressDialog", ProgressDialog::Lua_Create},
  {"EWS_New_FileDialog", FileDialog::Lua_Create},
  {"EWS_New_ComboBox", ComboBox::Lua_Create},
  {"EWS_New_CheckBox", CheckBox::Lua_Create},
  {"EWS_New_SpinCtrl", SpinCtrl::Lua_Create},

  {"EWS_New_BitmapButton", BitmapButton::Lua_Create},
  {"EWS_New_ListBox", ListBox::Lua_Create},
  {"EWS_New_ListCtrl", ListCtrl::Lua_Create},
  {"EWS_New_TreeCtrl", TreeCtrl::Lua_Create},
  {"EWS_New_Slider", Slider::Lua_Create},
  {"EWS_New_SpinButton", SpinButton::Lua_Create},
  {"EWS_New_RadioButton", RadioButton::Lua_Create},
};

int EWS_Lua_GetDataFolder(lua_State* L) {
  //#ifndef GAME
  lua_pushstring(L, "X:/Projects/EWS/ews_lib/lua_scripts/ews");
  /*#else
  char folder[MAX_PATH];
  getcwd(folder, MAX_PATH);
  std::cout << "CWD: " << folder << std::endl;
  lua_pushstring(L, folder);
  #endif*/
  return 1;
}

int EWS_Lua_SystemFileExists(lua_State* L) {
  auto path = lua_tostring(L, 1);
  if (path == nullptr) {
    std::cout << "File: " << path << " does not exist." << std::endl;
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

int EWS_Lua_Delete_Object(lua_State* L) {
  if (lua_type(L, 1) != LUA_TUSERDATA)
    return 0;
  auto obj = get_ews_object_from_top<Component>(L, 1);

  if (obj && obj->get_internal_object_type<wxObject>())
    delete obj->get_internal_object_type<wxObject>();

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

void EWS::add_members(lua_State* L) {
  // EWS functions, added in order of when they're called

  lua_register(L, "EWS_EWSModExistsCookie", EWS_EWSModExistsCookie);

  lua_register(L, "EWS_Lua_GetDataFolder", EWS_Lua_GetDataFolder);
  lua_register(L, "EWS_Lua_SystemFileExists", EWS_Lua_SystemFileExists);
  lua_register(L, "EWS_Lua_GetTextFromUser", EWS_Lua_GetTextFromUser);
  lua_register(L, "EWS_Lua_LaunchURL", EWS_Lua_LaunchURL);

  lua_register(L, "EWS_SetEWSWindow", EWS_Lua_SetEWSWindow);
  lua_register(L, "EWS_Delete_Object", EWS_Lua_Delete_Object);

  lua_register(L, "EWS_AddStringToHashlist", EWS_AddStringToHashlist);

  for(int i = 0; i < EWS_methods.size(); i++) {
    lua_register(L, EWS_methods[i].name, EWS_methods[i].func);
  }

  Register_EventHandling_Functions(L);

  Frame::Add_Frame_Funcs(L);
  BoxSizer::Add_BoxSizer_Funcs(L);
  StaticBoxSizer::Add_StaticBoxSizer_Funcs(L);
  Dialog::Add_Dialog_Funcs(L);
  MessageDialog::Add_MessageDialog_Funcs(L);
  MenuBar::Add_MenuBar_Funcs(L);
  Menu::Add_Menu_Funcs(L);
  Panel::Add_Panel_Funcs(L);
  ToolBar::Add_ToolBar_Funcs(L);
  Sizer::Add_Sizer_Funcs(L);
  Notebook::Add_Notebook_Funcs(L);
  SplitterWindow::Add_SplitterWindow_Funcs(L);
  StaticText::Add_StaticText_Funcs(L);
  Window::Add_Window_Funcs(L);
  AppWindow::Add_AppWindow_Funcs(L);

  ProgressDialog::Add_ProgressDialog_Funcs(L);
  FileDialog::Add_FileDialog_Funcs(L);

  ScrolledWindow::Add_ScrolledWindow_Funcs(L);
  
  StatusBar::Add_StatusBar_Funcs(L);

  TextCtrl::Add_TextCtrl_Funcs(L);
  SpinCtrl::Add_SpinCtrl_Funcs(L);
  ListCtrl::Add_ListCtrl_Funcs(L);
  TreeCtrl::Add_TreeCtrl_Funcs(L);

  ComboBox::Add_ComboBox_Funcs(L);
  ListBox::Add_ListBox_Funcs(L);
  CheckBox::Add_CheckBox_Funcs(L);

  Slider::Add_Slider_Funcs(L);

  AnyButton::Add_AnyButton_Funcs(L);
  Button::Add_Button_Funcs(L);
  ToggleButton::Add_ToggleButton_Funcs(L);
  BitmapButton::Add_BitmapButton_Funcs(L);
  SpinButton::Add_SpinButton_Funcs(L);
}
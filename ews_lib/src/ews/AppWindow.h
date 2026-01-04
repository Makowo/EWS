#pragma once

#include <wx/wx.h>
#include <wx/glcanvas.h>

#include "Window.h"

class wxAppWindow : public wxGLCanvas {
public:
  wxAppWindow(wxWindow* parent, const wxSize& size, int style);
  ~wxAppWindow();

  void Keyboard_OnKeyDown(wxKeyEvent& evt);
  void Keyboard_OnKeyUp(wxKeyEvent& evt);
  void Keyboard_OnChar(wxKeyEvent& evt);

  void Mouse_Move(wxMouseEvent& evt);

  void Mouse_LeftButtonDown(wxMouseEvent& evt);
  void Mouse_LeftButtonUp(wxMouseEvent& evt);
  void Mouse_RightButtonDown(wxMouseEvent& evt);
  void Mouse_RightButtonUp(wxMouseEvent& evt);

  void Mouse_EnterWindow(wxMouseEvent& evt);
  void Mouse_LeaveWindow(wxMouseEvent& evt);

  void Event_Activate(wxActivateEvent& evt);

public:
  void set_real_raid_window(HWND raid);
private:
  HWND raid_window;
  char* directinput_keyboard_source;
};

class AppWindow : public Window {
  COMPONENT_CHILD_CLASS(AppWindow);
public:
  static void AddLuaFunctions(lua_State* L);

public:
  static int Lua_Create(lua_State* L);

private:
  void init(Window* parent, Vector3 resolution, const std::string& style);
};
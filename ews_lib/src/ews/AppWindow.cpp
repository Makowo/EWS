#include "AppWindow.h"

// rendering.h
#include <rendering.h>
#include <input.h>

#include <dinput.h>

static const std::vector<luaL_Reg> methods = {
};

ADD_FUNCS_AUTOFILL(AppWindow::Add_AppWindow_Funcs)

int AppWindow::Lua_Create(lua_State* L) {
  Window* parent = get_ews_object_from_top<Window>(L, 1);
  Vector3* size = get_vec3_from_arg(L, 2);
  std::string style = lua_tostring(L, 3);

  AppWindow* appwindow = create_new_ews_object<AppWindow>(L);

  appwindow->init(parent, *size, style);

  return 1;
}

void AppWindow::init(Window* parent, Vector3 resolution, const std::string& style) {
  this->set_internal_object_type(new wxAppWindow(parent->get_internal_object_type<wxWindow>(), wxSize(resolution.x, resolution.y), get_style_from_merged_list(style)));
}

wxAppWindow::wxAppWindow(wxWindow* parent, const wxSize& size, int style) : wxGLCanvas(parent, wxID_ANY, 0, wxDefaultPosition, size, style, "Diesel AppWindow") {
  // genius way of creating a canvas to render DX to from:
  // https://github.com/adamajammary/simple-3d-engine-directx-opengl/blob/master/src/render/DXContext.cpp#L1568
  // https://github.com/adamajammary/simple-3d-engine-directx-opengl/blob/b65e1b81aec68de1b7dbb59b60bb10f1152b27d3/src/render/RenderEngine.cpp#L738

  this->Bind(wxEVT_MOTION, &wxAppWindow::Mouse_Move, this);

  this->Bind(wxEVT_ENTER_WINDOW, &wxAppWindow::Mouse_EnterWindow, this);
  this->Bind(wxEVT_LEAVE_WINDOW, &wxAppWindow::Mouse_LeaveWindow, this);

  this->Bind(wxEVT_ACTIVATE, &wxAppWindow::Event_Activate, this);

  this->Bind(wxEVT_LEFT_DOWN, &wxAppWindow::Mouse_LeftButtonDown, this);
  this->Bind(wxEVT_LEFT_UP, &wxAppWindow::Mouse_LeftButtonUp, this);
  this->Bind(wxEVT_RIGHT_DOWN, &wxAppWindow::Mouse_RightButtonDown, this);
  this->Bind(wxEVT_RIGHT_UP, &wxAppWindow::Mouse_RightButtonUp, this);

  this->Bind(wxEVT_KEY_DOWN, &wxAppWindow::Keyboard_OnKeyDown, this);
  this->Bind(wxEVT_KEY_UP, &wxAppWindow::Keyboard_OnKeyUp, this);

  this->directinput_keyboard_source = new char[256] {};

  set_keyboard_input_source(this->directinput_keyboard_source);
}

wxAppWindow::~wxAppWindow() {
  set_keyboard_input_source(nullptr);
  delete[] this->directinput_keyboard_source;
}

dsl::Win32Window* get_dsl_window_from_hwnd(HWND window) {
  return (dsl::Win32Window*)GetWindowLong(window, -21);
}

WNDPROC get_wndproc_from_hwnd(HWND window) {
  return (WNDPROC)GetWindowLongPtr(window, GWLP_WNDPROC);
}

constexpr char lower_constexpr(char c);

#define KEY_LIST() \
KEY('w', DIK_W); \
KEY('a', DIK_A); \
KEY('s', DIK_S); \
KEY('d', DIK_D); \
KEY(VK_SPACE, DIK_SPACE);

// TODO: implement these events
#define KEY(wxKeyCode_, dinputKeyCode) case wxKeyCode_: this->directinput_keyboard_source[dinputKeyCode] = 0x80; std::cout << #wxKeyCode_ << " pressed" << std::endl; break;
void wxAppWindow::Keyboard_OnKeyDown(wxKeyEvent& evt) {
  switch (lower_constexpr(evt.GetRawKeyCode())) { // only ASCII support, sorry users of international keyboards :(
    KEY_LIST();
  }
  evt.Skip();
}
#undef KEY

#define KEY(wxKeyCode_, dinputKeyCode) case wxKeyCode_: this->directinput_keyboard_source[dinputKeyCode] = 0x0; break;
void wxAppWindow::Keyboard_OnKeyUp(wxKeyEvent& evt) {
  switch (lower_constexpr(evt.GetRawKeyCode())) {
  //switch (0x00) {
    KEY_LIST();
  }
  evt.Skip();
}
#undef KEY

void wxAppWindow::Keyboard_OnChar(wxKeyEvent& evt) {
}

void wxAppWindow::Mouse_Move(wxMouseEvent& evt) {
  if (get_dsl_window_from_hwnd(this->raid_window)) {
    get_dsl_window_from_hwnd(this->raid_window)->_mouse_x = evt.GetX();
    get_dsl_window_from_hwnd(this->raid_window)->_mouse_y = evt.GetY();
  }
}

void wxAppWindow::Mouse_LeftButtonDown(wxMouseEvent& evt) { if(get_dsl_window_from_hwnd(this->raid_window)) get_dsl_window_from_hwnd(this->raid_window)->_mouse_l_button = true; evt.Skip(); }
void wxAppWindow::Mouse_LeftButtonUp(wxMouseEvent& evt) { if(get_dsl_window_from_hwnd(this->raid_window)) get_dsl_window_from_hwnd(this->raid_window)->_mouse_l_button = false; }
void wxAppWindow::Mouse_RightButtonDown(wxMouseEvent& evt) { if(get_dsl_window_from_hwnd(this->raid_window)) get_dsl_window_from_hwnd(this->raid_window)->_mouse_r_button = true; }
void wxAppWindow::Mouse_RightButtonUp(wxMouseEvent& evt) { if(get_dsl_window_from_hwnd(this->raid_window)) get_dsl_window_from_hwnd(this->raid_window)->_mouse_r_button = false; }

void wxAppWindow::Mouse_EnterWindow(wxMouseEvent& evt) {
  if(get_dsl_window_from_hwnd(this->raid_window))
    get_dsl_window_from_hwnd(this->raid_window)->_mouse_in_window = true;
  //get_dsl_window_from_hwnd(this->raid_window)->_activate_state = true;
}

void wxAppWindow::Mouse_LeaveWindow(wxMouseEvent& evt) {
  if(get_dsl_window_from_hwnd(this->raid_window))
    get_dsl_window_from_hwnd(this->raid_window)->_mouse_in_window = false;
  //get_dsl_window_from_hwnd(this->raid_window)->_activate_state = false;
}

void wxAppWindow::Event_Activate(wxActivateEvent& evt) {
  std::cout << "Activate" << std::endl;
  if (get_dsl_window_from_hwnd(this->raid_window)) {
    get_dsl_window_from_hwnd(this->raid_window)->_activate_state = evt.GetActive();
    get_dsl_window_from_hwnd(this->raid_window)->_activation_altered = true;
  }
}

void wxAppWindow::set_real_raid_window(HWND raid) {
  this->raid_window = raid;
}

constexpr char lower_constexpr(char c) {
  switch (c) {
  default: return c;
  case 'A': return 'a';
  case 'B': return 'b';
  case 'C': return 'c';
  case 'D': return 'd';
  case 'E': return 'e';
  case 'F': return 'f';
  case 'G': return 'g';
  case 'H': return 'h';
  case 'I': return 'i';
  case 'J': return 'j';
  case 'K': return 'k';
  case 'L': return 'l';
  case 'M': return 'm';
  case 'N': return 'n';
  case 'O': return 'o';
  case 'P': return 'p';
  case 'Q': return 'q';
  case 'R': return 'r';
  case 'S': return 's';
  case 'T': return 't';
  case 'U': return 'u';
  case 'V': return 'v';
  case 'W': return 'w';
  case 'X': return 'x';
  case 'Y': return 'y';
  case 'Z': return 'z';
  }
}
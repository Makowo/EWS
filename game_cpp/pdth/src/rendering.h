#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

namespace dsl {
  class Win32Window {
  public:
    HWND _hwnd;
    int _x;
    int _y;
    int _width;
    int _height;
    bool _close;
    bool _hide_mouse_on_activate;

    unsigned int _style;
    unsigned int _style_ex;

    char pad1[44]; // _input_buffer (std::string) and _control_key_buffer (std::vector<dsl::idstring>)

    float _mouse_x;
    float _mouse_y;
    float _mouse_z;
    bool _mouse_l_button;
    bool _mouse_m_button;
    bool _mouse_r_button;
    bool _mouse_x1_button;
    bool _mouse_x2_button;
    bool _mouse_in_window;

    bool _deactivated;
    bool _activation_altered;
    bool _activate_state;
  };
}

class AppWindow;
void set_appwindow(AppWindow* new_appwindow);
void setup_rendering_hooks();


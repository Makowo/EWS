#include "rendering.h"

#include <cstddef>

static_assert(offsetof(dsl::Win32Window, _mouse_in_window) == 0x5D);

static_assert(sizeof(dsl::Win32Window) == 0x64);

AppWindow* appwindow;
void set_appwindow(AppWindow* new_appwindow) {
  appwindow = new_appwindow;
}

void setup_rendering_hooks() {}

HWND real_raid_window;
#pragma once

#include <vector>
#include <dinput.h>

void setup_input_hooks();

void set_keyboard_input_source(char* source);
std::vector<LPDIRECTINPUTDEVICE8W>& get_directinput_devices();
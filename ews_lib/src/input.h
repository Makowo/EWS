#pragma once
#define WIN32_LEAN_AND_MEAN 1
#define NOMINMAX

#include <vector>
#include <dinput.h>

void setup_input_hooks();

void set_keyboard_input_source(char* source);
std::vector<LPDIRECTINPUTDEVICE8W>& get_directinput_devices();
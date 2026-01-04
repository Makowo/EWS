#pragma once

#include "component.h"

int get_winid_from_string(const char* str);
int get_winid_from_string(const std::string& str);

int EWS_EventHandler_Connect(lua_State* L);
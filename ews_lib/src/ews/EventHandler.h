#pragma once

#include "component.h"

int get_winid_from_string(const char* str);
int get_winid_from_string(const std::string& str);

void Register_EventHandling_Functions(lua_State* L);
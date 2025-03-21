#pragma once

#ifdef GAME
#include <lua_funcs.h>
#else
extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}
#endif
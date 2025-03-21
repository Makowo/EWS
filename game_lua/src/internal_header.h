#pragma once

#include "lua_funcs.h"

#ifdef INTERNAL_USE
#define FUNC_DEFINE(name, return_, function_args, ...) \
  typedef return_ (__cdecl* lua_funcs_types_##name##_t)(function_args); \
  extern lua_funcs_types_##name##_t funcptr_##name;
#else
#define FUNC_DEFINE(name, return_, function_args) \
  typedef return_ (__cdecl*lua_funcs_types_##name##_t)(function_args); \
  lua_funcs_types_##name##_t funcptr_##name;
#endif


#undef FUNC_DEFINE
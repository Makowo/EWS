#pragma once

#define METHODSLIST_METHOD(name, dest) {name, dest},
#define TEXTENTRY_METHODS_LIST(clazz)\
METHODSLIST_METHOD("EWS_" #clazz "_GetLastPosition", clazz##::Lua_GetLastPosition) \
METHODSLIST_METHOD("EWS_" #clazz "_GetValue", clazz##::Lua_GetValue) \
METHODSLIST_METHOD("EWS_" #clazz "_SetValue", clazz##::Lua_SetValue) \
METHODSLIST_METHOD("EWS_" #clazz "_AppendText", clazz##::Lua_AppendText) \
METHODSLIST_METHOD("EWS_" #clazz "_Clear", clazz##::Lua_Clear)

#define TEXTENTRY_HEADER_FUNCS \
static int Lua_GetLastPosition(lua_State* L);\
static int Lua_GetValue(lua_State* L);\
static int Lua_SetValue(lua_State* L);\
static int Lua_Clear(lua_State* L); \
static int Lua_AppendText(lua_State* L);

#define TEXTENTRY_FUNCTIONS_IMPL(clazz, internal_obj_type)\
int clazz::Lua_GetLastPosition(lua_State* L) {\
  auto ctrl = get_ews_object_from_top<clazz>(L, 1);\
\
  lua_pushnumber(L, ctrl->get_internal_object_type<internal_obj_type>()->GetLastPosition());\
\
  return 1;\
}\
\
int clazz::Lua_GetValue(lua_State* L) {\
  auto ctrl = get_ews_object_from_top<clazz>(L, 1);\
\
  lua_pushstring(L, ctrl->get_internal_object_type<internal_obj_type>()->GetValue());\
\
  return 1;\
}\
int clazz::Lua_SetValue(lua_State* L) {\
  auto ctrl = get_ews_object_from_top<clazz>(L, 1);\
  auto value = lua_tostring(L, 2);\
\
  ctrl->get_internal_object_type<internal_obj_type>()->SetValue(value);\
\
  return 0;\
}\
\
int clazz::Lua_AppendText(lua_State* L) {\
  auto ctrl = get_ews_object_from_top<clazz>(L, 1);\
  auto append = lua_tostring(L, 2);\
\
  ctrl->get_internal_object_type<internal_obj_type>()->AppendText(append);\
\
  return 0;\
}\
int clazz::Lua_Clear(lua_State* L) {\
  auto ctrl = get_ews_object_from_top<clazz>(L, 1);\
\
  ctrl->get_internal_object_type<internal_obj_type>()->Clear();\
\
  return 0;\
}
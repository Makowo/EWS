#pragma once

#include "ews_lua.hpp"

#include "diesel.h"

#include <string>
#include <vector>

#include <sstream>
#include <wx/colour.h>
#include <wx/image.h>


#if 1
#define DISABLE_IMAGE_LOAD_WARNING wxLogNull stopImageLoadWarning;
#else
#define DISABLE_IMAGE_LOAD_WARNING
#endif

template<typename T> T* get_ews_object_from_top(lua_State* L, int top) {
  if (top == 0)
    __debugbreak();
  return (T*)lua_touserdata(L, top);
}

template<typename T> T* create_new_ews_object(lua_State* L) {
  auto instance = (T*)lua_newuserdata(L, sizeof(T));
  new (instance) T();
  return instance;
}

Vector3* get_vec3_from_arg(lua_State*L, int top);

const wxColour* get_wxcolour_from_name(std::string name);

int get_style_from_merged_list(const std::string& merged_list);
int constexpr get_style_from_list(const std::vector<std::string>& styles);

static int get_event_by_name(const char* name);

std::string resolve_icon_path(std::string path);
wxImage get_wximage_from_path(std::string path);

#define ADD_FUNCS_AUTOFILL(function)            \
  void function(lua_State *L) { \
    for (int i = 0; i < methods.size(); i++) { \
      lua_register(L, methods[i].name, methods[i].func); \
    } \
  }
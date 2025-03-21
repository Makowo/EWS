#include "BoxSizer.h"

#include "component.h"

static const std::vector<luaL_Reg> BoxSizer_methods = {
  {"EWS_BoxSizer_Add", BoxSizer::Lua_Add}
};

void BoxSizer::Add_BoxSizer_Funcs(lua_State* L) {
  for (int i = 0; i < BoxSizer_methods.size(); i++) {
    lua_register(L, BoxSizer_methods[i].name, BoxSizer_methods[i].func);
  }
}

int BoxSizer::Lua_Create(lua_State* L) {
  auto orient = lua_tostring(L, 1);

  auto sizer = create_new_ews_object<BoxSizer>(L);

  sizer->init(orient);

  return 1;
}

int BoxSizer::Lua_Add(lua_State* L) {
  auto sizer = get_ews_object_from_top<BoxSizer>(L, 1);
  auto to_add = get_ews_object_from_top<Component>(L, 2)->get_internal_object_type<wxSizerItem>();

  sizer->get_internal_object_type<wxBoxSizer>()->Add(to_add);
  return 0;
}

void BoxSizer::init(std::string orient) {
  auto orientation = 0;
  if (orient == "HORIZONTAL") {
    orientation = wxHORIZONTAL;
  } else if (orient == "VERTICAL") {
    orientation = wxVERTICAL;
  }
  this->set_internal_object_type(new wxBoxSizer(orientation));
}

#include "StaticBoxSizer.h"

#include "./Window.h"

static const std::vector<luaL_Reg> methods = {
};

ADD_FUNCS_AUTOFILL(StaticBoxSizer::Add_StaticBoxSizer_Funcs)

int StaticBoxSizer::Lua_Create(lua_State* L) {
  auto win = get_ews_object_from_top<Window>(L, 1);
  auto orient = lua_tostring(L, 2);
  auto title = lua_tostring(L, 3);

  auto staticboxsizer = create_new_ews_object<StaticBoxSizer>(L);

  staticboxsizer->init(win, orient, title);

  return 1;
}

void StaticBoxSizer::init(Window* win, std::string orient, std::string title) {
  auto orientation = 0;
  if (orient == "HORIZONTAL") {
    orientation = wxHORIZONTAL;
  }
  else if (orient == "VERTICAL") {
    orientation = wxVERTICAL;
  }
  this->set_internal_object_type(new wxStaticBoxSizer(orientation, win->get_internal_object_type<wxWindow>(), title));
}

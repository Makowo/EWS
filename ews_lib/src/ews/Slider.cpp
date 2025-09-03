
#include "Slider.h"

void Slider::AddLuaFunctions(lua_State* L)
{
  Control::AddLuaFunctions(L);
  REGISTER_LUA_CLASS_FUNCTION(do_nothing, "set_value");
  REGISTER_LUA_CLASS_FUNCTION(do_nothing, "get_value");
}

int Slider::Lua_Create(lua_State* L) {
  Window* parent = get_ews_object_from_top<Window>(L, 2);
  float value = lua_tonumber(L, 3);
  float min_value = lua_tonumber(L, 4);
  float max_value = lua_tonumber(L, 5);

  auto slider = create_new_ews_object<Slider>(L);

  slider->init(parent, (int)value, (int)min_value, (int)max_value);

  return 1;
}

void Slider::init(Window* parent, int value, int min_value, int max_value) {
  this->set_internal_object_type(new wxSlider(parent->get_internal_object_type<wxWindow>(), -1, value, min_value, max_value));
}

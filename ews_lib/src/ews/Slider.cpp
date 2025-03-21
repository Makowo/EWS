
#include "Slider.h"

static const std::vector<luaL_Reg> methods = {
};

ADD_FUNCS_AUTOFILL(Slider::Add_Slider_Funcs)

int Slider::Lua_Create(lua_State* L) {

  auto slider = create_new_ews_object<Slider>(L);

  slider->init();

  return 1;
}

void Slider::init() {
}


#include "ListCtrl.h"

static const std::vector<luaL_Reg> methods = {
};

ADD_FUNCS_AUTOFILL(ListCtrl::Add_ListCtrl_Funcs)

int ListCtrl::Lua_Create(lua_State* L) {

  auto listctrl = create_new_ews_object<ListCtrl>(L);

  listctrl->init();

  return 1;
}

void ListCtrl::init() {
}

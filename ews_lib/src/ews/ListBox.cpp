
#include "ListBox.h"

static const std::vector<luaL_Reg> methods = {
};

ADD_FUNCS_AUTOFILL(ListBox::Add_ListBox_Funcs)

int ListBox::Lua_Create(lua_State* L) {

  auto listbox = create_new_ews_object<ListBox>(L);

  listbox->init();

  return 1;
}

void ListBox::init() {
}

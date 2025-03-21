
#include "TreeCtrl.h"

#include <wx/treectrl.h>

static const std::vector<luaL_Reg> methods = {
};

ADD_FUNCS_AUTOFILL(TreeCtrl::Add_TreeCtrl_Funcs)

int TreeCtrl::Lua_Create(lua_State* L) {
  auto parent = get_ews_object_from_top<Window>(L, 1);
  auto unk1 = lua_tostring(L, 2);
  auto style = lua_tostring(L, 3);

  auto treectrl = create_new_ews_object<TreeCtrl>(L);

  treectrl->init(parent, style);

  return 1;
}

void TreeCtrl::init(Window* parent, const std::string& style) {
  this->set_internal_object_type(new wxTreeCtrl(parent->get_internal_object_type<wxWindow>(), wxID_ANY, wxDefaultPosition, wxDefaultSize, get_style_from_merged_list(style)));
}

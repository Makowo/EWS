#include "TextCtrl.h"

#include "ews_utils.h"

static const std::vector<luaL_Reg> methods = {
  {"EWS_TextCtrl_ShowPosition", TextCtrl::Lua_ShowPosition},


  TEXTENTRY_METHODS_LIST(TextCtrl)
};

ADD_FUNCS_AUTOFILL(TextCtrl::Add_TextCtrl_Funcs)

int TextCtrl::Lua_Create(lua_State* L) {
  auto parent = get_ews_object_from_top<Window>(L, 1);
  auto text = lua_tostring(L, 2);
  auto style = lua_tostring(L, 3);

  auto textctrl = create_new_ews_object<TextCtrl>(L);

  textctrl->init(parent, text, style);

  return 1;
}

int TextCtrl::Lua_ShowPosition(lua_State* L) {
  auto ctrl = get_ews_object_from_top<TextCtrl>(L, 1);

  auto position = lua_tonumber(L, 2);

  ctrl->get_internal_object_type<wxTextCtrl>()->ShowPosition(position);

  return 0;
}

TEXTENTRY_FUNCTIONS_IMPL(TextCtrl, wxTextCtrl)

void TextCtrl::init(Window* parent, const std::string& text, const std::string& style) {
  auto parent2 = (wxWindow*)nullptr;
  if (parent != nullptr)
    parent2 = parent->get_internal_object_type<wxWindow>();
  this->set_internal_object_type(new wxTextCtrl(parent2, wxID_ANY, wxString(text), wxDefaultPosition, wxDefaultSize, get_style_from_merged_list(style)));
}

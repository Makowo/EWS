
#include "ComboBox.h"

void ComboBox::AddLuaFunctions(lua_State* L)
{
  Control::AddLuaFunctions(L);
  TEXTENTRY_METHODS_LIST(ComboBox);
  REGISTER_LUA_CLASS_FUNCTION(ComboBox::Lua_Append, "append");
}

int ComboBox::Lua_Create(lua_State* L) {
  auto parent = get_ews_object_from_top<Window>(L, 2);
  auto unk1 = lua_tostring(L, 3);
  auto unk2 = lua_tostring(L, 4);
  auto style = lua_tostring(L, 5);

  auto combobox = create_new_ews_object<ComboBox>(L);

  combobox->init(parent, style);

  return 1;
}

int ComboBox::Lua_Append(lua_State* L) {
  auto combobox = get_ews_object_from_top<ComboBox>(L, 1);
  auto append_text = lua_tostring(L, 2);

  combobox->get_internal_object_type<wxComboBox>()->Append(append_text);

  return 0;
}

TEXTENTRY_FUNCTIONS_IMPL(ComboBox, wxComboBox)


void ComboBox::init(Window* parent, std::string style) {
  this->set_internal_object_type(new wxComboBox(parent->get_internal_object_type<wxWindow>(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, {}, get_style_from_merged_list(style)));
}

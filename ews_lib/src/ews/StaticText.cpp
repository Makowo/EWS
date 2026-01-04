#include "StaticText.h"

void StaticText::AddLuaFunctions(lua_State* L)
{
  Window::AddLuaFunctions(L);
  REGISTER_LUA_CLASS_FUNCTION(StaticText::Lua_SetValue, "set_value");
  REGISTER_LUA_CLASS_FUNCTION(StaticText::Lua_SetLabel, "set_label");
}

int StaticText::Lua_Create(lua_State* L) {
  Window* parent = get_ews_object_from_top<Window>(L, 2);
  std::string text = lua_tostring(L, 3);
  int unknown = lua_tonumber(L, 4);
  std::string style = lua_tostring(L, 5);

  StaticText* statictext = create_new_ews_object<StaticText>(L);

  statictext->init(parent, text, unknown, style);

  return 1;
}

int StaticText::Lua_SetLabel(lua_State* L) {
  auto text = get_ews_object_from_top<StaticText>(L, 1);

  text->get_internal_object_type<wxStaticText>()->SetLabel(lua_tostring(L, 2));

  return 0;
}

int StaticText::Lua_SetValue(lua_State* L) {
  auto text = get_ews_object_from_top<StaticText>(L, 1);

  text->get_internal_object_type<wxStaticText>()->SetLabel(lua_tostring(L, 2));

  return 0;
}

void StaticText::init(Window *parent, std::string text, int unknown, std::string style) {
  this->set_internal_object_type(new wxStaticText(parent->get_internal_object_type<wxWindow>(), wxID_ANY, wxString(text), wxDefaultPosition, wxDefaultSize, get_style_from_merged_list(style)));
}
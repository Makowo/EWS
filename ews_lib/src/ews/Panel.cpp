#include "Panel.h"

#include "Sizer.h"

#include "EventHandler.h"

static const std::vector<luaL_Reg> methods = {
};

ADD_FUNCS_AUTOFILL(Panel::Add_Panel_Funcs)

GCobj a;

int Panel::Lua_Create(lua_State* L) {
  auto parent = get_ews_object_from_top<Window>(L, 1);
  auto id = std::string("");
  auto style = std::string("");
  if(lua_gettop(L) > 1) {
  id = lua_tostring(L, 2);
  style = lua_tostring(L, 3);
  }

  auto panel = create_new_ews_object<Panel>(L);

  panel->init(parent, id, style);

  return 1;
}

void Panel::init(Window *parent, std::string id, std::string style)
{
  auto parent2 = (wxWindow*)nullptr;
  if (parent != nullptr)
    parent2 = parent->get_internal_object_type<wxWindow>();
  this->set_internal_object_type(new wxPanel(parent2, wxID_ANY, wxDefaultPosition, wxDefaultSize, get_style_from_merged_list(style)));
}

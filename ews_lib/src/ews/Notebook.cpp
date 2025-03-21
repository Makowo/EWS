#include "Notebook.h"

#include "Panel.h"

static const std::vector<luaL_Reg> methods = {
  {"EWS_Notebook_Add_Page", Notebook::Lua_Add_Page},

  {"EWS_Notebook_GetPageCount", Notebook::Lua_GetPageCount},
  {"EWS_Notebook_SetPageText", Notebook::Lua_SetPageText},
  {"EWS_Notebook_GetPageText", Notebook::Lua_GetPageText},
  {"EWS_Notebook_Fit", Notebook::Lua_Fit},
  {"EWS_Notebook_SetPage", Notebook::Lua_SetPage},
  {"EWS_Notebook_GetCurrentPage", Notebook::Lua_GetCurrentPage},
};

ADD_FUNCS_AUTOFILL(Notebook::Add_Notebook_Funcs)

int Notebook::Lua_Create(lua_State* L) {
  Window* parent = get_ews_object_from_top<Window>(L, 1);
  std::string name = lua_tostring(L, 2);
  std::string style = lua_tostring(L, 3);

  Notebook* notebook = create_new_ews_object<Notebook>(L);

  notebook->init(parent, name, style);

  return 1;
}

int Notebook::Lua_Add_Page(lua_State* L) {
  Notebook* notebook = get_ews_object_from_top<Notebook>(L, 1);
  Window* panel = get_ews_object_from_top<Window>(L, 2);
  std::string name = lua_tostring(L, 3);
  bool select = lua_toboolean(L, 4);

  notebook->get_internal_object_type<wxNotebook>()->AddPage(panel->get_internal_object_type<wxWindow>(), wxString(name), select);

  return 0;
}

int Notebook::Lua_GetPageCount(lua_State* L) {
  auto notebook = get_ews_object_from_top<Notebook>(L, 1);

  lua_pushnumber(L, notebook->get_internal_object_type<wxNotebook>()->GetPageCount());

  return 1;
}

int Notebook::Lua_SetPageText(lua_State* L) {
  auto notebook = get_ews_object_from_top<Notebook>(L, 1);
  auto index = lua_tonumber(L, 2);
  auto text = lua_tostring(L, 3);

  notebook->get_internal_object_type<wxNotebook>()->SetPageText(index, text);

  return 0;
}

int Notebook::Lua_GetPageText(lua_State* L) {
  auto notebook = get_ews_object_from_top<Notebook>(L, 1);
  auto index = lua_tonumber(L, 2);

  lua_pushstring(L, notebook->get_internal_object_type<wxNotebook>()->GetPageText(index).c_str());
  return 1;
}

int Notebook::Lua_Fit(lua_State* L) {
  auto notebook = get_ews_object_from_top<Notebook>(L, 1);

  notebook->get_internal_object_type<wxNotebook>()->Fit();

  return 0;
}

int Notebook::Lua_SetPage(lua_State* L) {
  auto notebook = get_ews_object_from_top<Notebook>(L, 1);
  auto index = lua_tonumber(L, 2);

  notebook->get_internal_object_type<wxNotebook>()->SetSelection(index);

  return 0;
}

// hacky way to avoid doing a ton of work in the lua side, returns the **index** of the current page (indexed at 1, same as lua), and the lua side will index into an internal self.pages table
int Notebook::Lua_GetCurrentPage(lua_State* L) {
  auto notebook = get_ews_object_from_top<Notebook>(L, 1);

  auto current_page = notebook->get_internal_object_type<wxNotebook>()->GetCurrentPage();
  auto index = 0;
  for (int i = 0; i < notebook->get_internal_object_type<wxNotebook>()->GetPageCount(); i++) {
    if (notebook->get_internal_object_type<wxNotebook>()->GetPage(i) == current_page) {
      index = i;
      break;
    }
  }

  // shift index into lua array indexing
  lua_pushnumber(L, index + 1);

  return 1;
}

void Notebook::init(Window* parent, std::string name, std::string style) {
  this->set_internal_object_type(new wxNotebook(parent->get_internal_object_type<wxWindow>(), wxID_ANY, wxDefaultPosition, wxDefaultSize, get_style_from_merged_list(style), wxString(name)));
}
#include "Dialog.h"

static const std::vector<luaL_Reg> methods = {
  {"EWS_Dialog_ShowModal", Dialog::Lua_ShowModal},
  {"EWS_Dialog_EndModal", Dialog::Lua_EndModal},
  {"EWS_Dialog_SetIcon", Dialog::Lua_SetIcon},
  {"EWS_Dialog_Visible", Dialog::Lua_Visible}
};

void Dialog::Add_Dialog_Funcs(lua_State* L) {
  for (int i = 0; i < methods.size(); i++) {
    lua_register(L, methods[i].name, methods[i].func);
  }
}

int Dialog::Lua_Create_Simple(lua_State* L) {
  Window* parent = get_ews_object_from_top<Window>(L, 1);
  auto title = lua_tostring(L, 2);
  auto style = lua_tostring(L, 3);

  auto dialog = create_new_ews_object<Dialog>(L);

  dialog->init(parent, title, style);

  return 1;
}

int Dialog::Lua_Create_Complex(lua_State* L) {
  Window* parent = get_ews_object_from_top<Window>(L, 1);
  auto title = lua_tostring(L, 2);
  auto position = get_vec3_from_arg(L, 3);
  auto size = get_vec3_from_arg(L, 4);
  auto style = lua_tostring(L, 5);

  auto dialog = create_new_ews_object<Dialog>(L);

  dialog->init(parent, title, style, wxPoint(position->x, position->y), wxSize(size->x, size->y));

  return 1;
}

int Dialog::Lua_ShowModal(lua_State* L) {
  Dialog* dialog = get_ews_object_from_top<Dialog>(L, 1);

  auto code = dialog->get_internal_object_type<wxDialog>()->ShowModal();

  if (code == wxID_CANCEL) {
    lua_pushboolean(L, false); // fix for file dialogs, possibly replace with lua_pushnil
  }
  else {
    lua_pushnumber(L, code);
  }

  return 1;
}

int Dialog::Lua_EndModal(lua_State* L) {
  Dialog* dialog = get_ews_object_from_top<Dialog>(L, 1);

  int code = 0;

  if (lua_type(L, 2) == LUA_TNUMBER) {
    code = lua_tonumber(L, 2);
  }
  else if (lua_type(L, 2) == LUA_TSTRING) {
    auto code_str = lua_tostring(L, 2);

#define ENDMODAL_CODE(code_, val) if (!strcmp(code_str, code_)) code = val;

    ENDMODAL_CODE("OK", wxID_OK);
    ENDMODAL_CODE("ID_OK", wxID_OK);

    ENDMODAL_CODE("CANCEL", wxID_CANCEL);
    ENDMODAL_CODE("ID_CANCEL", wxID_CANCEL);
    ENDMODAL_CODE("NEW", wxID_NEW);
    ENDMODAL_CODE("ID_NEW", wxID_NEW);
  }

  dialog->get_internal_object_type<wxDialog>()->EndModal(code);

  return 0;
}

int Dialog::Lua_SetIcon(lua_State* L) {
  Dialog* dialog = get_ews_object_from_top<Dialog>(L, 1);
  auto icon_path = lua_tostring(L, 2);

  wxLogNull stopiCCPWarning;
  wxIcon icon2;
  icon2.LoadFile(wxString(resolve_icon_path(icon_path)), wxBITMAP_TYPE_PNG);
  dialog->get_internal_object_type<wxDialog>()->SetIcon(icon2);
  return 0;
}

int Dialog::Lua_Visible(lua_State* L) {
  Dialog* dialog = get_ews_object_from_top<Dialog>(L, 1);

  lua_pushboolean(L, dialog->get_internal_object_type<wxDialog>()->IsVisible());

  return 1;
}

void Dialog::init(Window* parent, std::string title, std::string style, const wxPoint& pos, const wxSize& size) {
  wxWindow* parent_obj = nullptr;
  if (parent != nullptr) {
    parent_obj = parent->get_internal_object_type<wxWindow>();
  }
  this->set_internal_object_type(new wxDialog(parent_obj, -1, wxString(title), pos, size, get_style_from_merged_list(style)));
}

#include "Sizer.h"
#include "Window.h"

static const std::vector<luaL_Reg> methods = {
  {"EWS_Sizer_Add_WindowObj", Sizer::Lua_Add_WindowObj},
  {"EWS_Sizer_Add_SizerObj", Sizer::Lua_Add_SizerObj},
  {"EWS_Sizer_AddStretchSpacer", Sizer::Lua_AddStrechSpacer},
};

ADD_FUNCS_AUTOFILL(Sizer::Add_Sizer_Funcs)

int Sizer::Lua_Add_WindowObj(lua_State* L) {
  Sizer* sizer = get_ews_object_from_top<Sizer>(L, 1);
  Window* to_add = get_ews_object_from_top<Window>(L, 2);
  auto proportion = lua_tonumber(L, 3);
  auto border = lua_tonumber(L, 4);
  std::string flags = lua_tostring(L, 5);

  int flags2 = wxsizer_flags_to_int(flags);


  if (to_add == nullptr) {
    std::cout << "Add_SizerObj to_add was nullptr" << std::endl;
    return 0;
  }

  sizer->get_internal_object_type<wxSizer>()->Add(to_add->get_internal_object_type<wxWindow>(), proportion, flags2, border);
  return 0;
}
int Sizer::Lua_Add_SizerObj(lua_State *L)
{
  Sizer* sizer = get_ews_object_from_top<Sizer>(L, 1);
  Sizer* to_add = get_ews_object_from_top<Sizer>(L, 2);
  auto proportion = lua_tonumber(L, 3);
  auto border = lua_tonumber(L, 4);
  std::string flags = lua_tostring(L, 5);

  int flags2 = wxsizer_flags_to_int(flags);

  if (to_add == nullptr) {
    std::cout << "Add_SizerObj to_add was nullptr" << std::endl;
    return 0;
  }

  sizer->get_internal_object_type<wxSizer>()->Add(to_add->get_internal_object_type<wxSizer>(), proportion, flags2, border);
  return 0;
}

int Sizer::Lua_AddStrechSpacer(lua_State* L) {
  auto sizer = get_ews_object_from_top<Sizer>(L, 1);
  auto prop = lua_tonumber(L, 2);

  sizer->get_internal_object_type<wxSizer>()->AddStretchSpacer(prop);

  return 0;
}

int wxsizer_flags_to_int(std::string flags) {
  int out = 0;
  std::stringstream sstream(flags);

  std::string s;
  while(getline(sstream, s, ',')) {
    if (s == std::string("TOP"))
    {
      out |= wxTOP;
    }
    if (s == std::string("BOTTOM"))
    {
      out |= wxBOTTOM;
    }
    if (s == std::string("LEFT"))
    {
      out |= wxLEFT;
    }
    if (s == std::string("RIGHT"))
    {
      out |= wxRIGHT;
    }
    if (s == std::string("ALL"))
    {
      out |= wxALL;
    }

    if (s == std::string("EXPAND"))
    {
      out |= wxEXPAND;
    }

    if (s == std::string("SHAPED"))
    {
      out |= wxSHAPED;
    }

    if (s == std::string("FIXED_MINSIZE"))
    {
      out |= wxFIXED_MINSIZE;
    }

    if (s == std::string("RESERVE_SPACE_EVEN_IF_HIDDEN"))
    {
      out |= wxRESERVE_SPACE_EVEN_IF_HIDDEN;
    }

    if (s == std::string("ALIGN_CENTER"))
    {
      out |= wxALIGN_CENTER;
    }
    if (s == std::string("ALIGN_CENTRE"))
    {
      out |= wxALIGN_CENTRE;
    }
    if (s == std::string("ALIGN_LEFT"))
    {
      out |= wxALIGN_LEFT;
    }
    if (s == std::string("ALIGN_RIGHT"))
    {
      out |= wxALIGN_RIGHT;
    }
    if (s == std::string("ALIGN_TOP"))
    {
      out |= wxALIGN_TOP;
    }
    if (s == std::string("ALIGN_BOTTOM"))
    {
      out |= wxALIGN_BOTTOM;
    }
    if (s == std::string("ALIGN_CENTER_VERTICAL"))
    {
      out |= wxALIGN_CENTER_VERTICAL;
    }
    if (s == std::string("ALIGN_CENTRE_VERTICAL"))
    {
      out |= wxALIGN_CENTRE_VERTICAL;
    }
    if (s == std::string("ALIGN_CENTER_HORIZONTAL"))
    {
      out |= wxALIGN_CENTER_HORIZONTAL;
    }
    if (s == std::string("ALIGN_CENTRE_HORIZONTAL"))
    {
      out |= wxALIGN_CENTRE_HORIZONTAL;
    }
  }

  return out;
}
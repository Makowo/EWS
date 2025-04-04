
#include "BitmapButton.h"

static const std::vector<luaL_Reg> methods = {
};

ADD_FUNCS_AUTOFILL(BitmapButton::Add_BitmapButton_Funcs)

int BitmapButton::Lua_Create(lua_State* L) {
  auto parent = get_ews_object_from_top<Window>(L, 1);
  auto image_path = lua_tostring(L, 2);
  auto style = lua_tostring(L, 3);

  DISABLE_IMAGE_LOAD_WARNING

  auto bitmap = get_wximage_from_path(resolve_icon_path(image_path));


  auto bitmapbutton = create_new_ews_object<BitmapButton>(L);

  bitmapbutton->init(parent, bitmap, style);

  return 1;
}

void BitmapButton::init(Window* parent, const wxBitmapBundle& bitmap, const std::string& style) {
  this->set_internal_object_type(new wxBitmapButton(parent->get_internal_object_type<wxWindow>(), wxID_ANY, bitmap, wxDefaultPosition, wxDefaultSize, get_style_from_merged_list(style)));
}

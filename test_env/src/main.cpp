#include <EWS.h>

#include <iostream>
#include <wx/wx.h>

#include <ews_utils.h>
#include <diesel.h>

#pragma comment(lib, "User32.lib")
#pragma comment(lib, "Gdi32.lib")
#pragma comment(lib, "Ole32.lib")
#pragma comment(lib, "Winspool.lib")
#pragma comment(lib, "Shell32.lib")
#pragma comment(lib, "Rpcrt4.lib")
#pragma comment(lib, "Comdlg32.lib")
#pragma comment(lib, "Comctl32.lib")
#pragma comment(lib, "OleAut32.lib")
#pragma comment(lib, "Advapi32.lib")
#pragma comment(lib, "Version.lib")

class AppWindow;
void set_appwindow(AppWindow*) {}

int main2() {
  lua_State* L = luaL_newstate();

  luaL_openlibs(L);

  // diesel
  mvector(L);
  lua_register(L, "Vector3", vec3);

  wxImage::AddHandler(new wxPNGHandler);

  EWS::add_members(L);

  if (luaL_dofile(L, "X:/Projects/EWS/ews_lib/lua_scripts/ews/diesel.lua")) {
    std::cout << "Error " << lua_tostring(L, -1) << std::endl;
  }
  if(luaL_dofile(L, "X:/Projects/EWS/EWSLib.lua")) {
    std::cout << "Error " << lua_tostring(L, -1) << std::endl;
  }
  /*if (luaL_dofile(L, "X:/Projects/EWS/ews_lib/lua_scripts/ews/window.lua")) {
    luaL_error(L, "Error %s\n", lua_tostring(L, -1));
  }*/
  if(luaL_dofile(L, "X:/Projects/EWS/ews_lib/lua_scripts/ews/ews.lua")) {
    std::cout << "Error " << lua_tostring(L, -1) << std::endl;
  }
  if (luaL_dofile(L, "X:/Projects/EWS/test_env/lua_scripts/menu_bar.lua"))
  {
    std::cout << "Error " << lua_tostring(L, -1) << std::endl;
  }
  if (luaL_dofile(L, "X:/Projects/EWS/test_env/lua_scripts/tool_bar.lua"))
  {
    std::cout << "Error " << lua_tostring(L, -1) << std::endl;
  }
  if (luaL_dofile(L, "X:/Projects/EWS/test_env/lua_scripts/test.lua"))
  {
    std::cout << "Error " << lua_tostring(L, -1) << std::endl;
  }

  return 0;
}

class app : public wxApp {
  public:
  bool OnInit() override;
};

bool app::OnInit() {
  main2();
  return true;
}

wxIMPLEMENT_APP_NO_MAIN(app);

int main() {
  wxEntry(NULL);
  //main2();
  return 0;
}
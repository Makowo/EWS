--add_requires("wxwidgets 3.2.0")
add_requires("minhook")

--add_requires("wxwidgets 3.2.4", {configs = {shared = false}})

includes("ews_lib")
includes("game_lua")
includes("test_env")
includes("version_proxy")

includes("tools/exepatcher")

includes("wxWidgetsPackage.lua")

includes("Dependencies/HashlistLib/xmake.lua")

includes("../DieselEngineExplorer/dieselformats")


local arch = "x86"

x86_games = {
  "PDTH",
  "PD2"
}
x64_games = {
  "RAIDWW2"
}

set_runtimes("MD")

games = {}
if arch == "x64" then
  games = x64_games
else
  games = x86_games
end

games = {
  "PDTH",
  "PD2",
  "RAIDWW2"
}
for _, game in ipairs(games) do
target("EWS-"..game)
  set_kind("shared")
  set_languages("cxx20")
  set_exceptions("cxx")

  set_symbols("debug")

  --add_packages("wxwidgets")



  add_packages("minhook")
  --add_defines("__WXMSW__", "WXUSINGDLL")

  add_defines("GAME")
  add_files(get_ews_source_files())
  add_headerfiles(get_ews_header_files())
  add_includedirs(get_ews_include_path())

  add_headerfiles(path.join(os.projectdir(), "EWS.natvis"), path.join(os.projectdir(), ".editorconfig"))

  add_includedirs(path.join(os.projectdir(), "Dependencies/HashlistLib/include"))
  add_deps("HashlistLib")

  if game == "RAIDWW2" then
    add_files("./game_cpp/raidww2/src/*.cpp")
    add_headerfiles("./game_cpp/raidww2/src/*.h")
    add_includedirs("./game_cpp/raidww2/src")
    add_deps("game_lua_raid")
    add_defines("WIN64")
    add_wxwidgets("x64")
    set_arch("x64")
  end
  if game == "PDTH" then
    add_files("./game_cpp/pdth/src/*.cpp")
    add_headerfiles("./game_cpp/pdth/src/*.h")
    add_includedirs("./game_cpp/pdth/src")
    add_deps("game_lua_pdth")
    add_wxwidgets("x86")
    set_arch("x86")
  end
  if game == "PD2" then
    add_files("./game_cpp/pd2/src/*.cpp")
    add_headerfiles("./game_cpp/pd2/src/*.h")
    add_includedirs("./game_cpp/pd2/src")
    add_deps("game_lua_pd2")
    add_wxwidgets("x86")
    set_arch("x86")
  end

  add_files("./Dependencies/wxWidgets/include/wx/msw/wx.rc")
  add_includedirs(path.join(os.projectdir(), "game_lua/include"))
  add_includedirs(path.join(os.projectdir(), "ews_lib/src"))

  add_ldflags("/SUBSYSTEM:WINDOWS", {force = true})

target_end()
end
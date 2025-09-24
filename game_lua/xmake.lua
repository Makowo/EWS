target("game_lua_raid")
  set_languages("cxx20")
  set_kind("static")
  add_files("./src/gamelua.cpp")
  add_files("./src/raidww2.cpp")

  add_defines("WIN64")
  
  add_defines("GAME_RAIDWW2=1")
  add_headerfiles("./include/*.h")
  add_includedirs("./include")

  set_symbols("debug")

target_end()
target("game_lua_pdth")
  set_languages("cxx20")
  set_kind("static")
  add_files("./src/gamelua.cpp")
  add_files("./src/pdth.cpp")

  add_defines("GAME_PDTH=1")
  add_headerfiles("./include/*.h")
  add_includedirs("./include")

  set_symbols("debug")

target_end()
target("game_lua_pd2")
  set_languages("cxx20")
  set_kind("static")
  add_files("./src/gamelua.cpp")
  add_files("./src/pd2.cpp")
  
  add_defines("GAME_PD2=1")
  add_headerfiles("./include/*.h")
  add_includedirs("./include")

  set_symbols("debug")

target_end()
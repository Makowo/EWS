-- disable due to it taking up too much time in the GitHub Actions runner
--add_requires("wxwidgets 3.2.0")
--add_requires("lua v5.1.1")


includes(path.join(os.projectdir(),"ews_lib"))
includes(path.join(os.projectdir(),"wxWidgetsPackage.lua"))

target("test_env")
  set_languages("cxx20")
  add_files("./src/main.cpp")

  add_defines("WIN64")
  add_wxwidgets()
  add_packages("lua")
  

  add_files(path.join(os.projectdir(), "Dependencies/wxWidgets/include/wx/msw/wx.rc"))
  add_files(get_ews_source_files())
  add_includedirs(get_ews_include_path())
  add_includedirs(path.join(os.scriptdir(), ""))

target_end()
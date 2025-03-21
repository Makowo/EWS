function add_wxwidgets(arch)

  add_defines("__WXMSW__")
  
  add_links(
    "wxmsw32u_core", "wxbase32u", "wxpng", "wxzlib", "wxmsw32u_gl"
  )
  if arch == "x86" then
    add_linkdirs(path.join(os.projectdir(), "Dependencies/wxWidgets/lib/vc_lib"))
    add_includedirs(path.join(os.projectdir(), "Dependencies/wxWidgets/lib/vc_lib/mswu"))
  else
    add_linkdirs(path.join(os.projectdir(), "Dependencies/wxWidgets/lib/vc_x64_lib"))
    add_includedirs(path.join(os.projectdir(), "Dependencies/wxWidgets/lib/vc_x64_lib/mswu"))
  end
  add_includedirs(path.join(os.projectdir(), "Dependencies/wxWidgets/include"))
  add_includedirs(path.join(os.projectdir(), "Dependencies/wxWidgets/interface"))
end
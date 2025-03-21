function get_ews_source_files()
  return path.join(os.projectdir(), "ews_lib/src/*.cpp"), path.join(os.projectdir(), "ews_lib/src/ews/*.cpp")
end
function get_ews_header_files()
  return path.join(os.projectdir(), "ews_lib/src/*.h"), path.join(os.projectdir(), "ews_lib/src/ews/*.h")
end
function get_ews_include_path()
  return path.join(os.projectdir(), "ews_lib/include"), path.join(os.projectdir(), "ews_lib")
end
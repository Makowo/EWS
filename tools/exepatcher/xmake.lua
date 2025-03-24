add_requires("lief 0.16.4")

target("exepatcher")
  add_files("./src/*.cpp")
  set_languages("cxx20")

  add_packages("lief")

  after_build(function()
    os.cp("$(scriptdir)/fixed_manifest.manifest", "$(buildir)/windows/" .. os.arch() .. "/" .. (is_mode("debug") and "debug" or "release") .. "/fixed_manifest.manifest")
  end)
target_end()
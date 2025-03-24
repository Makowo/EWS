///
/// Command line tool to patch executables to target Windows 10, use a DPI aware manifest and switch the subsystem to CONSOLE.
/// 

#include <LIEF/LIEF.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <filesystem>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

int main(int argc, char* argv[])
{
  if (argc == 1) {
    std::cout << "Please provide an executable to patch" << std::endl;
    system("pause");
    return -1;
  }

  std::filesystem::path in_path = std::filesystem::path(argv[1]);

  WCHAR executable_path[256];
  GetModuleFileNameW(NULL, executable_path, sizeof(executable_path) / sizeof(executable_path[0]));
  std::filesystem::path base_path = std::filesystem::path(executable_path).parent_path();


  std::cout << "Opening " << in_path << "\n";

  auto pe = LIEF::PE::Parser::parse(in_path.string());

  if (!pe->has_resources()) {
    std::cout << "Provided file has no resources\n";
    system("pause");
    return 0;
  }

  auto resource_manager = pe->resources_manager();

  std::ifstream new_manifest((base_path / "fixed_manifest.manifest"));

  if (!new_manifest.good()) {
    std::cout << "Manifest file not present in the same directory as exepatcher (fixed_manifest.manifest)\n";
    system("pause");
    return 0;
  }

  std::stringstream new_manifest_str;
  new_manifest_str << new_manifest.rdbuf();
  new_manifest.close();

  resource_manager->manifest(new_manifest_str.str());
  std::cout << "Assigned new manifest (fixed_manifest.manifest)\n";


  pe->optional_header().major_operating_system_version(8); // windows 8, windows 10 targetting is set in the manifest
  pe->optional_header().minor_operating_system_version(0);
  std::cout << "Assigned new windows version (8.1)\n";

  pe->optional_header().subsystem(LIEF::PE::OptionalHeader::SUBSYSTEM::WINDOWS_CUI);
  std::cout << "Assigned new windows subsystem (CONSOLE)\n";

  pe->optional_header().checksum(pe->compute_checksum());

  LIEF::PE::Builder builder = LIEF::PE::Builder(*pe.get());
  builder.build_resources(true);
  builder.build();

  std::cout << "Writing patched executable to: " << (base_path / "raid_win64_release_patched.exe").string() << "\n";
  builder.write((base_path / "raid_win64_release_patched.exe").string());

  system("pause");
}
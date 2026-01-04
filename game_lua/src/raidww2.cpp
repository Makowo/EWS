#ifdef WIN64
#include "lua_funcs.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <psapi.h>

#include <string>

#include <iostream>
#include <vector>

// Signature scanning code is from RAID BLT https://github.com/Luffyyy/Raid-BLT/blob/master/src/signatures/signatures.cpp
//

// https://github.com/Luffyyy/Raid-BLT/blob/master/src/signatures/signatures.cpp#L9
MODULEINFO GetModuleInfo(std::string szModule)
{
  MODULEINFO modinfo = {0};
  HMODULE hModule = GetModuleHandle(szModule.c_str());
  if (hModule == 0)
    return modinfo;
  GetModuleInformation(GetCurrentProcess(), hModule, &modinfo, sizeof(MODULEINFO));
  return modinfo;
}

// https://github.com/Luffyyy/Raid-BLT/blob/master/src/signatures/signatures.cpp#L20
void FindPatternMultiple(const char* module, const char* funcname, const char* pattern, const char* mask, std::vector<unsigned __int64>& matches, bool stop_after_first) {
  MODULEINFO mInfo = GetModuleInfo(module);
  DWORDLONG base = (DWORDLONG)mInfo.lpBaseOfDll;
  DWORDLONG size = (DWORDLONG)mInfo.SizeOfImage;
  DWORDLONG patternLength = (DWORDLONG)strlen(mask);
  for(DWORDLONG i = 0; i < size - patternLength; i++) {
    bool found = true;
    for(DWORDLONG j = 0; j < patternLength; j++) {
      found &= mask[j] == '?' || pattern[j] == *(char*)(base + i + j);
    }
    if(found) {
      std::cout << "Found function " << funcname << " at " << (void*)(base + i) << std::endl;
      //return base + i;
      matches.push_back(base + i);
      if (stop_after_first)
        return;
    }
  }
  //std::cout << "Could not find function " << funcname << std::endl;
  return;
}

unsigned __int64 FindPattern(const char* module, const char* funcname, const char* pattern, const char* mask)
{
  std::vector<unsigned __int64> matches;
  FindPatternMultiple(module, funcname, pattern, mask, matches, true);
  if(matches.size() == 0)
    return NULL;
  return matches[0];
}

void FindPatternHack(unsigned long long* out_addr, const char* module, const char* funcname, const char* pattern, const char* mask) {
  auto addr = FindPattern(module, funcname, pattern, mask);
  if (out_addr)
    *out_addr = addr;
}


#define FUNCTION_SIGNATURE(name, pattern, mask) \
  FindPatternHack((unsigned long long*)&funcptr_##name, "raid_win64_release.exe", #name, pattern, mask)
  //std::cout << #func_name << " : " << funcptr_##func_name << std::endl;

#define CONCAT_IMPL(...) __VA_ARGS__
#define CONCAT_ARGS(...) CONCAT_IMPL(__VA_ARGS__)

#define RAID_VERSION_U_24_4 0x012404

#define RAID_VERSION RAID_VERSION_U_24_4

void raidww2_lua_setup() {

  FUNCTION_SIGNATURE(lua_createtable, "\x48\x89\x5C\x24\x00\x48\x89\x74\x24\x00\x57\x48\x83\xEC\x20\x4C\x8B\x49\x00\x41\x8B\xF8", "xxxx?xxxx?xxxxxxxx?xxx");
  FUNCTION_SIGNATURE(lua_getfield, "\x48\x89\x5C\x24\x00\x57\x48\x83\xEC\x20\x4D\x8B\xD0\x48\x8B\xF9", "xxxx?xxxxxxxxxxx");
  FUNCTION_SIGNATURE(lua_getmetatable, "\x48\x83\xEC\x28\x4C\x8B\xD1\xE8\x00\x00\x00\x00\x48\x8B\x08\x48\x8B\xC1\x48\xC1\xF8\x2F\x83\xF8\xF4", "xxxxxxxx????xxxxxxxxxxxxx");
  FUNCTION_SIGNATURE(lua_pcall, "\x48\x89\x5C\x24\x00\x48\x89\x74\x24\x00\x57\x48\x83\xEC\x20\x48\x8B\x59\x00\x41\x8B\xF0", "xxxx?xxxx?xxxxxxxx?xxx");
  FUNCTION_SIGNATURE(lua_pushboolean, "\x48\x8B\x41\x00\x45\x33\xC0\x85\xD2", "xxx?xxxxx");
  FUNCTION_SIGNATURE(lua_pushcclosure, "\x48\x89\x5C\x24\x00\x48\x89\x74\x24\x00\x57\x48\x83\xEC\x20\x48\x8B\xD9\x49\x63\xF8", "xxxx?xxxx?xxxxxxxxxxx");
  FUNCTION_SIGNATURE(lua_pushlstring, "\x48\x89\x5C\x24\x00\x48\x89\x74\x24\x00\x57\x48\x83\xEC\x20\x4C\x8B\x49\x00\x49\x8B\xF8\x48\x8B\xF2\x48\x8B\xD9\x49\x8B\x41\x00\x49\x39\x41\x00\x72\x00\xE8\x00\x00\x00\x00\x4C\x8B\xC7\x48\x8B\xD6\x48\x8B\xCB\xE8", "xxxx?xxxx?xxxxxxxx?xxxxxxxxxxxx?xxx?x?x????xxxxxxxxxx");

  FUNCTION_SIGNATURE(lua_pushlightuserdata, "\x40\x53\x48\x83\xEC\x00\x48\x8B\xD9\xE8\x00\x00\x00\x00\x48\x8B\x53\x00\x48\xB9", "xxxxx?xxxx????xxx?xx");
  FUNCTION_SIGNATURE(lua_setmetatable, "\x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x48\x89\x74\x24\x00\x57\x48\x83\xEC\x00\x8B\xEA\x48\x8B\xF9\xE8", "xxxx?xxxx?xxxx?xxxx?xxxxxx");

#if RAID_VERSION < RAID_VERSION_U_24_4
  // PRE u24.4
  FUNCTION_SIGNATURE(lua_pushstring, "\x48\x89\x5C\x24\x00\x57\x48\x83\xEC\x20\x48\x8B\xFA\x48\x8B\xD9\x48\x85\xD2", "xxxx?xxxxxxxxxxxxxx");
#else
  // POST u24.4
  FUNCTION_SIGNATURE(lua_pushstring, "\x48\x89\x5C\x24\x00\x57\x48\x83\xEC\x00\x48\x8B\xFA\x48\x8B\xD9\x48\x85\xD2\x75", "xxxx?xxxx?xxxxxxxxxx");
#endif

  FUNCTION_SIGNATURE(lua_setfield, "\x48\x89\x5C\x24\x00\x57\x48\x83\xEC\x20\x4D\x8B\xD0\x48\x8B\xD9", "xxxx?xxxxxxxxxxx");
  FUNCTION_SIGNATURE(lua_toboolean, "\x48\x83\xEC\x28\xE8\x00\x00\x00\x00\x48\x8B\x08\x33\xC0", "xxxxx????xxxxx");
  FUNCTION_SIGNATURE(lua_tolstring, "\x48\x89\x5C\x24\x00\x48\x89\x74\x24\x00\x57\x48\x83\xEC\x20\x49\x8B\xF8\x8B\xDA\x48\x8B\xF1\xE8", "xxxx?xxxx?xxxxxxxxxxxxxx");
  FUNCTION_SIGNATURE(lua_tonumber, "\x48\x83\xEC\x28\xE8\x00\x00\x00\x00\x48\x8B\x08\x48\x8B\xD1\x48\xC1\xFA\x2F\x83\xFA\xF2\x77", "xxxxx????xxxxxxxxxxxxxx");
  FUNCTION_SIGNATURE(lua_touserdata, "\x48\x83\xEC\x28\x4C\x8B\xD1\xE8\x00\x00\x00\x00\x48\x8B\x10", "xxxxxxxx????xxx");
  FUNCTION_SIGNATURE(lua_type, "\x48\x83\xEC\x28\x4C\x8B\xD1\xE8\x00\x00\x00\x00\x4C\x8B\xD8", "xxxxxxxx????xxx");
  FUNCTION_SIGNATURE(luaL_loadstring, "\x48\x89\x5C\x24\x00\x57\x48\x81\xEC\xF0\x00\x00\x00\x48\x8B\xD9", "xxxx?xxxxxxxxxxx");
  FUNCTION_SIGNATURE(lua_loadx, "\x4C\x8B\xDC\x49\x89\x5B\x00\x57\x48\x81\xEC\xE0\x00\x00\x00\x4D\x85\xC9\x49\x89\x53\x00\x4D\x89\x43\x00\x48\x8D\x05\x00\x00\x00\x00\x49\x0F\x45\xC1\x49\x89\x4B\x00\x49\x89\x43\x00\x4C\x8D\x0D\x00\x00\x00\x00\x48\x8B\x84\x24", "xxxxxx?xxxxxxxxxxxxxx?xxx?xxx????xxxxxxx?xxx?xxx????xxxx");


  FUNCTION_SIGNATURE(lua_newuserdata, "\x48\x89\x5C\x24\x00\x57\x48\x83\xEC\x00\x4C\x8B\x41\x00\x48\x8B\xFA\x48\x8B\xD9\x49\x8B\x40", "xxxx?xxxx?xxx?xxxxxxxxx");
  FUNCTION_SIGNATURE(luaL_loadfile, "\x48\x89\x5C\x24\x00\x55\x56\x57\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x84\x24\x00\x00\x00\x00\x33\xF6", "xxxx?xxxxxx????xxx????xxxxxxx????xx");

  FUNCTION_SIGNATURE(lua_pushnumber, "\x48\x8B\x41\x00\xF2\x0F\x11\x08", "xxx?xxxx");

  FUNCTION_SIGNATURE(lua_settop, "\x48\x89\x5C\x24\x00\x57\x48\x83\xEC\x00\x48\x63\xFA", "xxxx?xxxx?xxx");
  FUNCTION_SIGNATURE(luaL_ref, "\x48\x89\x5C\x24\x00\x57\x48\x83\xEC\x00\x8D\x82", "xxxx?xxxx?xx");
  FUNCTION_SIGNATURE(lua_rawgeti, "\x40\x53\x48\x83\xEC\x00\x4D\x63\xD0", "xxxxx?xxx");

  FUNCTION_SIGNATURE(lua_pushvalue, "\x48\x83\xEC\x00\x4C\x8B\xD1\xE8\x00\x00\x00\x00\x49\x8B\x52", "xxx?xxxx????xxx");
}

#undef FUNCTION_SIGNATURE

#endif
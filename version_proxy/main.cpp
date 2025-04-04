// https://github.com/BitCrackers/version-proxy/blob/main/src/version.cpp

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

static HMODULE ews_dll = nullptr;

HMODULE version_dll;

#define WRAPPER_FUNC(name) o##name = GetProcAddress(version_dll, ## #name)

#ifdef _M_AMD64
#pragma warning(disable : 4081)
#define STRINGIFY(name) #name
#define EXPORT_FUNCTION comment(linker, "/EXPORT:" __FUNCTION__ "=" __FUNCDNAME__)
#define WRAPPER_GENFUNC(name)                 \
  FARPROC o##name;                            \
  __declspec(dllexport) void WINAPI _##name() \
  {                                           \
    __pragma(STRINGIFY(EXPORT_FUNCTION));     \
    o##name();                                \
  }
#else
#define WRAPPER_GENFUNC(name)      \
  FARPROC o##name;                 \
  __declspec(naked) void _##name() \
  {                                \
    __asm jmp[o##name]             \
  }
#endif

WRAPPER_GENFUNC(GetFileVersionInfoA);
WRAPPER_GENFUNC(GetFileVersionInfoByHandle);
WRAPPER_GENFUNC(GetFileVersionInfoExW);
WRAPPER_GENFUNC(GetFileVersionInfoExA);
WRAPPER_GENFUNC(GetFileVersionInfoSizeA);
WRAPPER_GENFUNC(GetFileVersionInfoSizeExA);
WRAPPER_GENFUNC(GetFileVersionInfoSizeExW);
WRAPPER_GENFUNC(GetFileVersionInfoSizeW);
WRAPPER_GENFUNC(GetFileVersionInfoW);
WRAPPER_GENFUNC(VerFindFileA);
WRAPPER_GENFUNC(VerFindFileW);
WRAPPER_GENFUNC(VerInstallFileA);
WRAPPER_GENFUNC(VerInstallFileW);
WRAPPER_GENFUNC(VerLanguageNameA);
WRAPPER_GENFUNC(VerLanguageNameW);
WRAPPER_GENFUNC(VerQueryValueA);
WRAPPER_GENFUNC(VerQueryValueW);

void load_version()
{
  char systemPath[MAX_PATH];
  GetSystemDirectoryA(systemPath, MAX_PATH);
  strcat_s(systemPath, "\\version.dll");
  version_dll = LoadLibraryA(systemPath);

  if (!version_dll)
    return;

  WRAPPER_FUNC(GetFileVersionInfoA);
  WRAPPER_FUNC(GetFileVersionInfoByHandle);
  WRAPPER_FUNC(GetFileVersionInfoExW);
  WRAPPER_FUNC(GetFileVersionInfoExA);
  WRAPPER_FUNC(GetFileVersionInfoSizeA);
  WRAPPER_FUNC(GetFileVersionInfoSizeExW);
  WRAPPER_FUNC(GetFileVersionInfoSizeExA);
  WRAPPER_FUNC(GetFileVersionInfoSizeW);
  WRAPPER_FUNC(GetFileVersionInfoW);
  WRAPPER_FUNC(VerFindFileA);
  WRAPPER_FUNC(VerFindFileW);
  WRAPPER_FUNC(VerInstallFileA);
  WRAPPER_FUNC(VerInstallFileW);
  WRAPPER_FUNC(VerLanguageNameA);
  WRAPPER_FUNC(VerLanguageNameW);
  WRAPPER_FUNC(VerQueryValueA);
  WRAPPER_FUNC(VerQueryValueW);
}
BOOL APIENTRY DllMain(HMODULE hmodule, DWORD reason, LPVOID reserved)
{
  if (reason == DLL_PROCESS_ATTACH)
  {
    DisableThreadLibraryCalls(hmodule);
    load_version();
    if (!version_dll)
      return 0;

    // #ifdef _DEBUG
    //   MessageBox(NULL, L"It's working!", L"version proxy", MB_OK | MB_ICONINFORMATION | MB_SYSTEMMODAL);
    // #endif

    // your payload goes here
    ews_dll = LoadLibrary("EWS-RAIDWW2.dll");
    //if (GetFileAttributes("./EWS_RAIDWW2.dll") != INVALID_FILE_ATTRIBUTES && GetLastError() != ERROR_FILE_NOT_FOUND) {ews_dll = LoadLibrary("EWS_RAIDWW2.dll");} else
    //if (GetFileAttributes("./EWS_PDTH.dll") != INVALID_FILE_ATTRIBUTES && GetLastError() != ERROR_FILE_NOT_FOUND) { ews_dll = LoadLibrary("EWS_PDTH.dll"); } else
    //if (GetFileAttributes("./EWS_PD2.dll") != INVALID_FILE_ATTRIBUTES && GetLastError() != ERROR_FILE_NOT_FOUND) { ews_dll = LoadLibrary("EWS_PD2.dll"); }
  }
  if (reason == DLL_PROCESS_DETACH)
  {
    FreeLibrary(version_dll);
    FreeLibrary(ews_dll);
  }
  return TRUE;
}
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <rendering.h>
#include <input.h>

#include <lua_funcs.h>

#include <EWS.h>
#include <HashlistLib.h>

#include <MinHook.h>
#include <wx/wx.h>
#include <wx/evtloop.h>
#include <wx/sysopt.h>

#include <iostream>
#include <sstream>
#include <streambuf>

#include <locale>
#include <codecvt>
#include <string>

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
#pragma comment(lib, "Version.lib") // unfortunately wxWidgets happens to require the DLL used to inject EWS into RAID. Anybody trying to compile EWS themselves should build the VERSION project first, delete the created .lib file, and try to avoid building it again.

class DieselApp : public wxApp
{
public:
  bool OnInit() override;

  wxEventLoopBase* GetDieselMainLoop() {
    //wxEventLoopBaseTiedPtr mainLoop(&m_mainLoop, CreateMainLoop());
    if (m_mainLoop == nullptr) {
      m_mainLoop = CreateMainLoop();
      wxEventLoopBase::SetActive(m_mainLoop);
    }

    return m_mainLoop;
  }
};

bool DieselApp::OnInit() {
  return true;
}

extern unsigned __int64 FindPattern(const char* module, const char* funcname, const char* pattern, const char* mask);

extern void FindPatternMultiple(const char* module, const char* funcname, const char* pattern, const char* mask, std::vector<unsigned __int64>& matches, bool stop_after_first);

int EWS_DieselApp_ProcessEvents(lua_State *L) {
  return 0;
}

int EWS_Log(lua_State* L) {
  if (lua_type(L, 1) == LUA_TSTRING)
    std::cout << lua_tostring(L, 1) << std::endl;
  return 0;
}

#pragma region Lua Setup and Errors
class RAID_CXX14_runtime_error {};
namespace dsl {
  class ErrorWithCStack : public RAID_CXX14_runtime_error {
  public:
    virtual void unneeded_virtual(); // exception dtor
    virtual const char* what();
  };
  class LuaError : public dsl::ErrorWithCStack {};
  class LuaRuntimeError : public dsl::LuaError {};
  class LuaInterpreter {
  public:
    lua_State* L;
  };
}

typedef __int64(__thiscall* dsl__LuaInterpreter_t)(__int64 this_, int thread, bool libs, bool classes, bool main, unsigned int a6);
dsl__LuaInterpreter_t o_dsl__LuaInterpreter_constructor;
__int64 __fastcall h_dsl__LuaInterpreter_constructor(__int64 this_, int thread, bool libs, bool classes, bool main, unsigned int a6) {
  __int64 ret = o_dsl__LuaInterpreter_constructor(this_, thread, libs, classes, main, a6);

  if (main && libs) {
    lua_State* state = ((dsl::LuaInterpreter*)this_)->L;
    std::cout << "lua state created" << std::endl;
    EWS::add_members(state);
    lua_register(state, "EWS_DieselApp_ProcessEvents", EWS_DieselApp_ProcessEvents);
    lua_register(state, "EWS_Log", EWS_Log);

    typedef void(__cdecl* setup_lrdb_on_state_t)(lua_State* L); // setup_lrdb_on_state
    auto lrdb_setup = (setup_lrdb_on_state_t)GetProcAddress(GetModuleHandleA("DieselLuaDebugger.dll"), "?setup_lrdb_on_state@@YAXPEAUlua_State@@@Z");
    if (lrdb_setup) {
      std::cout << "Initialising LRDB" << std::endl;
      lrdb_setup(state);
    }
    else {
      std::cout << "setup_lrdb_on_state is null, if this is unintentional, make sure DieselLuaDebugger.dll is present in the game folder." << std::endl;
    }
  }
  return ret;
}

class RAID_CXX14_string {
public:
  union {
    char* _Ptr;
    char _Buf[16];
  };
  size_t size;
  size_t reserved_size;

public:
  std::string get_string() {
    if (size < 16) {
      return std::string(this->_Buf);
    }
    else {
      return std::string(this->_Ptr);
    }
  }
};

typedef __int64(__fastcall* dsl__LuaRuntimeError__LuaRuntimeError_t)(dsl::LuaRuntimeError* this_, lua_State* L, RAID_CXX14_string* what);
dsl__LuaRuntimeError__LuaRuntimeError_t o_dsl__LuaRuntimeError__LuaRuntimeError;

__int64 __fastcall h_dsl__LuaRuntimeError__LuaRuntimeError(dsl::LuaRuntimeError* this_, lua_State* L, RAID_CXX14_string* what) {
  auto ret = o_dsl__LuaRuntimeError__LuaRuntimeError(this_, L, what);

  std::cout << "LuaRuntimeError thrown:" << std::endl;
  //std::cout << what->data << std::endl;
  std::cout << this_->what() << std::endl;

  __debugbreak();
  return ret;
}
#pragma endregion

namespace dsl {
  class ThreadManager__Thread {
  public:
    char pad[8];
    RAID_CXX14_string name;
  };
}

std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

__int64 (__fastcall*o_dsl__ThreadManager__ThreadManagerThreadProc)(dsl::ThreadManager__Thread* thiz);

__int64 __fastcall h_dsl__ThreadManager__ThreadManagerThreadProc(dsl::ThreadManager__Thread* thiz) {
  std::wstring thread_name = converter.from_bytes(thiz->name.get_string());
  SetThreadDescription(GetCurrentThread(), thread_name.c_str());
  return o_dsl__ThreadManager__ThreadManagerThreadProc(thiz);
}

#define MINHOOK_ERROR_CHECKING(onerror, expression) \
  {MH_STATUS status = expression; if(status != MH_OK) {std::cout << onerror << " " << MH_StatusToString(status);}}

extern unsigned __int64 FindPattern(char* module, const char* funcname, const char* pattern, const char* mask);

class Application;
char(__fastcall* Application__update_o)(Application* this_);

char __fastcall Application__update_h(Application* this_) {
  auto ret = Application__update_o(this_);

  lua_State* L = (*(dsl::LuaInterpreter**)(((__int64)this_) + 704))->L;
  if (wxTheApp) {
    lua_settop(L, 0);
    wxTheApp->ProcessPendingEvents();
    ((DieselApp*)wxTheApp)->GetDieselMainLoop()->DispatchTimeout(1);
    lua_settop(L, 0);
  }

  return ret;
}

#pragma region Idstring:s()
typedef __int64(__fastcall* dsl__ScriptIdstring__add_members_t)(lua_State* L);
dsl__ScriptIdstring__add_members_t dsl__ScriptIdstring__add_members_o;

dsl::idstring dsl__ScriptValue__get_object__idstring(lua_State* L, int index) {
  auto _id = (dsl::idstring*)lua_touserdata(L, index);
  return *_id;
}

// diesel production build only, get the string representation of an idstring
int dsl__ScriptIdstring__s(lua_State* L) {
  auto id = dsl__ScriptValue__get_object__idstring(L, 1);

  auto str = attempt_to_find_source_idstring_from_hashlist(id, true);

  lua_pushstring(L, str.c_str());
  return 1;
}

__int64 __fastcall dsl__ScriptIdstring__add_members_h(lua_State* L) {
  auto ret = dsl__ScriptIdstring__add_members_o(L);

  lua_pushcclosure(L, dsl__ScriptIdstring__s, 0);
  lua_setfield(L, (int)4294967294LL, "s");

  return ret;
}
#pragma endregion

WXDLLIMPEXP_BASE extern void wxSetInstance(HINSTANCE hInst);
extern "C" HINSTANCE wxGetInstance();

void setup_EWS(HINSTANCE instance)
{
  //wxSetInstance(instance); // attempt to fix icon transparency, which is caused by raid_win64_release.exe not having the wxWidgets rc file
  raidww2_lua_setup();

  wxApp::SetInstance(new DieselApp());

  wxEntryStart(instance, NULL);

  wxTheApp->CallOnInit();


  wxSizerFlags::DisableConsistencyChecks(); // diesel used an older version of wxWidgets, meaning many sizer flags conflict now.
  wxSystemOptions::SetOption("msw.remap", 2);

  wxImage::AddHandler(new wxPNGHandler);
  wxImage::AddHandler(new wxBMPHandler);

  ((DieselApp*)wxTheApp)->GetDieselMainLoop();

  load_hashlist(); // TODO: add custom idstring string server. maybe?

  

  //std::cout << attempt_to_find_source_idstring_from_hashlist(0x99e60fa5d3f23c2d) << std::endl;
  //std::cout << attempt_to_find_source_idstring_from_hashlist(_byteswap_uint64(0x99e60fa5d3f23c2d)) << std::endl;
  //app_entry = (app_entry_t)FindPattern((char*)"raid_win64_release.exe", "app_entry", "\x48\x83\xEC\x00\xE8\x00\x00\x00\x00\x90\xB1", "xxx?x????xx");
  //app_entry = (app_entry_t)(((uint8_t*)GetModuleHandle("raid_win64_release.exe")) + 0x00000000006BE3D0);


auto luainterpreter_addr = FindPattern(("raid_win64_release.exe"), "dsl::LuaInterpreter::ctor", "\x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x48\x89\x74\x24\x00\x48\x89\x4C\x24\x00\x57\x48\x83\xEC\x00\x41\x0F\xB6\xE8", "xxxx?xxxx?xxxx?xxxx?xxxx?xxxx");


auto dsl__ScriptIdstring__add_members = FindPattern("raid_win64_release.exe", "ScriptIdstring::add_members", "\x48\x89\x5C\x24\x00\x48\x89\x74\x24\x00\x57\x48\x83\xEC\x00\x8B\x44\x24\x00\x48\x8D\x3D", "xxxx?xxxx?xxxx?xxx?xxx");

auto Application__update = FindPattern("raid_win64_release.exe", "Application::update", "\x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x48\x89\x74\x24\x00\x57\x41\x56\x41\x57\x48\x83\xEC\x00\x0F\x29\x74\x24\x00\x0F\x29\x7C\x24\x00\x48\x8B\xF9", "xxxx?xxxx?xxxx?xxxxxxxx?xxxx?xxxx?xxx");
auto dsl__ThreadManager__ThreadManagerThreadProc = FindPattern("raid_win64_release.exe", "dsl::ThreadManager::ThreadManagerThreadProc", "\x40\x55\x48\x83\xEC\x00\x48\x8D\x6C\x24\x00\x48\x89\x5D\x00\x48\x8D\x41", "xxxxx?xxxx?xxx?xxx");

MINHOOK_ERROR_CHECKING("Failed to create luainterpreter_constructor hook:", MH_CreateHook((LPVOID)luainterpreter_addr, &h_dsl__LuaInterpreter_constructor, reinterpret_cast<LPVOID*>(&o_dsl__LuaInterpreter_constructor)));
MINHOOK_ERROR_CHECKING("Failed to enable luainterpreter_constructor hook:", MH_EnableHook((LPVOID)luainterpreter_addr));

auto dsl__LuaRuntimeError__LuaRuntimeError = FindPattern("raid_win64_release.exe", "dsl::LuaRuntimeError::ctor", "\x48\x89\x5C\x24\x00\x48\x89\x4C\x24\x00\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xAC\x24\x00\x00\x00\x00\x48\x81\xEC\x00\x00\x00\x00\x49\x8B\xF0", "xxxx?xxxx?xxxxxxxxxxxxxxx????xxx????xxx");
MINHOOK_ERROR_CHECKING("Failed to create dsl__LuaRuntimeError__LuaRuntimeError hook:", MH_CreateHook((LPVOID)dsl__LuaRuntimeError__LuaRuntimeError, &h_dsl__LuaRuntimeError__LuaRuntimeError, reinterpret_cast<LPVOID*>(&o_dsl__LuaRuntimeError__LuaRuntimeError)));
MINHOOK_ERROR_CHECKING("Failed to enable dsl__LuaRuntimeError__LuaRuntimeError hook:", MH_EnableHook((LPVOID)dsl__LuaRuntimeError__LuaRuntimeError));

MH_CreateHook((LPVOID)dsl__ScriptIdstring__add_members, &dsl__ScriptIdstring__add_members_h, (LPVOID*)&dsl__ScriptIdstring__add_members_o);
MH_EnableHook((LPVOID)dsl__ScriptIdstring__add_members);

MH_CreateHook((LPVOID)dsl__ThreadManager__ThreadManagerThreadProc, &h_dsl__ThreadManager__ThreadManagerThreadProc, (LPVOID*)&o_dsl__ThreadManager__ThreadManagerThreadProc);
MH_EnableHook((LPVOID)dsl__ThreadManager__ThreadManagerThreadProc);
MH_CreateHook((LPVOID)Application__update, &Application__update_h, (LPVOID*)&Application__update_o);
MH_EnableHook((LPVOID)Application__update);

setup_rendering_hooks();
setup_input_hooks();

LoadLibrary("DieselLuaDebugger.dll");
}

class EWSRegisteredEventData;
std::vector<EWSRegisteredEventData*> registered_events;

dsl::idstring last_type;
dsl::idstring last_name;

__int64(__fastcall* o_dsl__DB__try_open)(__int64 thiz, __int64 result, dsl::idstring* type, dsl::idstring* name, __int64 a5, __int64 transport);
__int64(__fastcall* o_dsl__DB__try_open_LanguageResolver)(__int64 thiz, __int64 result, dsl::idstring* type, dsl::idstring* name, __int64 a5, __int64 transport);
__int64(__fastcall* o_dsl__DB__try_open_unk1)(__int64 thiz, __int64 result, dsl::idstring* type, dsl::idstring* name, __int64 a5, __int64 transport);
__int64 __fastcall h_dsl__DB__try_open(__int64 thiz, __int64 result, dsl::idstring* type, dsl::idstring* name, __int64 a5, __int64 transport) {
  if (type)
    last_type = *type;
  if (name)
    last_name = *name;
  std::cout << "[dsl::DB::try_open (Default Resolver)] File: " << attempt_to_find_source_idstring_from_hashlist(last_name, false) << "." << attempt_to_find_source_idstring_from_hashlist(last_type, false) << "\n";
  auto ret = o_dsl__DB__try_open(thiz, result, type, name, a5, transport);
  return ret;
}
__int64 __fastcall h_dsl__DB__try_open_LanguageResolver(__int64 thiz, __int64 result, dsl::idstring* type, dsl::idstring* name, __int64 a5, __int64 transport) {
  if (type)
    last_type = *type;
  if (name)
    last_name = *name;
  std::cout << "[dsl::DB::try_open (LanguageResolver)] File: " << attempt_to_find_source_idstring_from_hashlist(last_name, false) << "." << attempt_to_find_source_idstring_from_hashlist(last_type, false) << "\n";
  auto ret = o_dsl__DB__try_open_LanguageResolver(thiz, result, type, name, a5, transport);
  return ret;
}
__int64 __fastcall h_dsl__DB__try_open_unk1(__int64 thiz, __int64 result, dsl::idstring* type, dsl::idstring* name, __int64 a5, __int64 transport) {
  if (type)
    last_type = *type;
  if (name)
    last_name = *name;
  std::cout << "[dsl::DB::try_open (Unk1)] File: " << attempt_to_find_source_idstring_from_hashlist(last_name, false) << "." << attempt_to_find_source_idstring_from_hashlist(last_type, false) << "\n";
  auto ret = o_dsl__DB__try_open_unk1(thiz, result, type, name, a5, transport);
  return ret;
}

__int64(__fastcall* o_MultiFileTransport__open)(__int64* thiz, __int64* result, unsigned int key);
__int64 __fastcall h_MultiFileTransport__open(__int64* thiz, __int64* result, unsigned int key) {
  auto ret = o_MultiFileTransport__open(thiz, result, key);

  PWSTR desc;
  GetThreadDescription(GetCurrentThread(), &desc);

  /*std::cout << "Attempting to open: all" << (key % 512) << "/" << key << "\n";// << " file: " << attempt_to_find_source_idstring_from_hashlist(last_name, false) << "." << attempt_to_find_source_idstring_from_hashlist(last_type, false) << "\n";
  if (desc)
    std::wcout << desc << L"\n";
  else
    std::cout << "Unknown Thread Name\n";*/

  return ret;
}

HANDLE(__stdcall* o_CreateFileA)(LPCSTR lpFileName, DWORD desiredAccess, DWORD shareMode, LPSECURITY_ATTRIBUTES securityAttributes, DWORD creationDisposition, DWORD flagsAndAttributes, HANDLE templateFile);
HANDLE(__stdcall* o_CreateFileW)(LPCWSTR lpFileName, DWORD desiredAccess, DWORD shareMode, LPSECURITY_ATTRIBUTES securityAttributes, DWORD creationDisposition, DWORD flagsAndAttributes, HANDLE templateFile);
HANDLE __stdcall h_CreateFileA(LPCSTR lpFileName, DWORD desiredAccess, DWORD shareMode, LPSECURITY_ATTRIBUTES securityAttributes, DWORD creationDisposition, DWORD flagsAndAttributes, HANDLE templateFile) {
  if (!std::string(lpFileName).starts_with("\\"))
    std::cout << "CreateFileA: " << lpFileName << "\n";
  auto ret = o_CreateFileA(lpFileName, desiredAccess, shareMode, securityAttributes, creationDisposition, flagsAndAttributes, templateFile);
  return ret;
}
HANDLE __stdcall h_CreateFileW(LPCWSTR lpFileName, DWORD desiredAccess, DWORD shareMode, LPSECURITY_ATTRIBUTES securityAttributes, DWORD creationDisposition, DWORD flagsAndAttributes, HANDLE templateFile) {
  auto ret = o_CreateFileW(lpFileName, desiredAccess, shareMode, securityAttributes, creationDisposition, flagsAndAttributes, templateFile);
  if (!std::wstring(lpFileName).starts_with(L"\\")) {
    if (ret == INVALID_HANDLE_VALUE)
      std::wcout << L"CreateFileW (INVALID_HANDLE_VALUE): " << lpFileName << L"\n";
    /*else
      std::wcout << L"CreateFileW (!INVALID_HANDLE_VALUE): " << lpFileName << L"\n";*/
  }
  return ret;
}

struct ResourceID {
  dsl::idstring type;
  dsl::idstring name;
};
void* (__fastcall* o_PackageManager__resource)(__int64* thiz, ResourceID* r);
void* __fastcall h_PackageManager__resource(__int64* thiz, ResourceID* r) {
  auto ret = o_PackageManager__resource(thiz, r);

  if(ret == nullptr)
    std::cout << "[PackageManager::resource] File: " << attempt_to_find_source_idstring_from_hashlist(r->name) << "." << attempt_to_find_source_idstring_from_hashlist(r->type) << " - " << ret << "\n";

  /*if (r->type == dsl::idstring("font"))
    __debugbreak();*/
  return ret;
}

__int64 (__fastcall* o_Package__preload)(__int64 thiz, ResourceID* a2, char a3, char a4);
__int64 __fastcall h_Package__preload(__int64 thiz, ResourceID* a2, char a3, char a4) {

  auto ret = o_Package__preload(thiz, a2, a3, a4);
  if(!ret)
    std::cout << "[Package::preload] File: " << attempt_to_find_source_idstring_from_hashlist(a2->name) << "." << attempt_to_find_source_idstring_from_hashlist(a2->type) << " - " << ret << "\n";

  return ret;
}

void MultiFileTransportTesting() {
  //AllocConsole();
  //freopen("CONOUT$", "w", stdout);
  //load_hashlist();

  auto MultiFileTransport__open = FindPattern("raid_win64_release.exe", "MultiFileTransport::open", "\x48\x8B\xC4\x48\x89\x58\x00\x57\x48\x81\xEC\x00\x00\x00\x00\x0F\x29\x70\x00\x48\x8B\xFA\x48\x8B\xD9", "xxxxxx?xxxx????xxx?xxxxxx");
  auto dsl__DB__try_open = FindPattern("raid_win64_release.exe", "dsl::DB::try_open", "\x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x48\x89\x74\x24\x00\x48\x89\x4C\x24\x00\x57\x41\x56\x41\x57\x48\x83\xEC\x00\x49\x8B\xF9", "xxxx?xxxx?xxxx?xxxx?xxxxxxxx?xxx");
  //auto dsl__DB__try_open_LanguageResolver = (unsigned long long)((uint64_t)GetModuleHandle("raid_win64_release.exe") + 0x1636A0);//*(unsigned long long*)FindPattern("raid_win64_release.exe", "dsl::DB::try_open_LanguageResolver_xref", "\xE8\x00\x00\x00\x00\xC7\x44\x24\x00\x00\x00\x00\x00\x48\x83\x7F", "x????xxx?????xxx");
  //auto dsl__DB__try_open_unk1 = (unsigned long long)((uint64_t)GetModuleHandle("raid_win64_release.exe") + 0x466E0);
  //auto PackageManager__resource = FindPattern("raid_win64_release.exe", "db try_open", "", "");
  //auto dsl__DB__try_open_LanguageResolver = FindPattern("raid_win64_release.exe", "db try_open unk_language_resolver", "", "");
  //auto dsl__DB__try_open_unk1 = FindPattern("raid_win64_release.exe", "db try_open unk1", "", "");
  auto PackageManager__resource = FindPattern("raid_win64_release.exe", "PackageManager::resource", "\x40\x55\x41\x56\x41\x57\x48\x83\xEC\x00\x4C\x8B\x49", "xxxxxxxxx?xxx");
  auto Package__preload = FindPattern("raid_win64_release.exe", "Package::preload", "\x48\x89\x5C\x24\x00\x48\x89\x54\x24\x00\x48\x89\x4C\x24\x00\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xAC\x24\x00\x00\x00\x00\x48\x81\xEC\x00\x00\x00\x00\x45\x0F\xB6\xE1", "xxxx?xxxx?xxxx?xxxxxxxxxxxxxxx????xxx????xxxx");

  std::vector<unsigned __int64> db_try_open;
  FindPatternMultiple("raid_win64_release.exe", "dsl::DB::try_open", "\x48\x89\x54\x24\x00\x55\x53\x56\x57\x41\x54\x41\x56\x41\x57\x48\x8D\x6C\x24\x00\x48\x81\xEC\x00\x00\x00\x00\x49\x8B\xF1\x4D\x8B\xF0\x48\x8B\xDA\x48\x8B\xF9\x45\x33\xE4\x4D\x8B\x11\x49\x8B\x00\x48\x89\x05\x00\x00\x00\x00\x4C\x89\x15\x00\x00\x00\x00\x4C\x39\xA1\x00\x00\x00\x00\x0F\x84\x00\x00\x00\x00\x48\x8D\x54\x24\x00\xE8\x00\x00\x00\x00\x90\x4C\x39\x65\x00\x0F\x84\x00\x00\x00\x00\x0F\x57\xC0\x0F\x11\x03\x4C\x89\x63\x00\x48\xC7\x43\x00\x00\x00\x00\x00\x44\x88\x23\x48\x8D\x4B\x00\xBA\x00\x00\x00\x00\xFF\x15", "xxxx?xxxxxxxxxxxxxx?xxx????xxxxxxxxxxxxxxxxxxxxxxxx????xxx????xxx????xx????xxxx?x????xxxx?xx????xxxxxxxxx?xxx?????xxxxxx?x????xx", db_try_open, false);

  //auto dsl__DB__try_open_LanguageResolver = db_try_open[2];
  //auto dsl__DB__try_open_unk1 = db_try_open[1];

  MH_CreateHook((LPVOID)MultiFileTransport__open, &h_MultiFileTransport__open, reinterpret_cast<LPVOID*>(&o_MultiFileTransport__open));
  MH_EnableHook((LPVOID)MultiFileTransport__open);
  //MH_CreateHook((LPVOID)dsl__DB__try_open, &h_dsl__DB__try_open, reinterpret_cast<LPVOID*>(&o_dsl__DB__try_open));
  //MH_EnableHook((LPVOID)dsl__DB__try_open);
  //MH_CreateHook((LPVOID)dsl__DB__try_open_LanguageResolver, &h_dsl__DB__try_open_LanguageResolver, reinterpret_cast<LPVOID*>(&o_dsl__DB__try_open_LanguageResolver));
  //MH_EnableHook((LPVOID)dsl__DB__try_open_LanguageResolver);
  //MH_CreateHook((LPVOID)dsl__DB__try_open_unk1, &h_dsl__DB__try_open_unk1, reinterpret_cast<LPVOID*>(&o_dsl__DB__try_open_unk1));
  //MH_EnableHook((LPVOID)dsl__DB__try_open_unk1);
  MH_CreateHook((LPVOID)PackageManager__resource, &h_PackageManager__resource, reinterpret_cast<LPVOID*>(&o_PackageManager__resource));
  MH_EnableHook((LPVOID)PackageManager__resource);
  MH_CreateHook((LPVOID)Package__preload, &h_Package__preload, reinterpret_cast<LPVOID*>(&o_Package__preload));
  MH_EnableHook((LPVOID)Package__preload);

  MH_CreateHook((LPVOID)CreateFileA, &h_CreateFileA, reinterpret_cast<LPVOID*>(&o_CreateFileA));
  MH_EnableHook((LPVOID)CreateFileA);
  MH_CreateHook((LPVOID)CreateFileW, &h_CreateFileW, reinterpret_cast<LPVOID*>(&o_CreateFileW));
  MH_EnableHook((LPVOID)CreateFileW);
  //auto dsl__LuaRuntimeError__LuaRuntimeError = FindPattern("raid_win64_release.exe", "dsl::LuaRuntimeError::ctor", "\x48\x89\x5C\x24\x00\x48\x89\x4C\x24\x00\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xAC\x24\x00\x00\x00\x00\x48\x81\xEC\x00\x00\x00\x00\x49\x8B\xF0", "xxxx?xxxx?xxxxxxxxxxxxxxx????xxx????xxx");
  //MINHOOK_ERROR_CHECKING("Failed to create dsl__LuaRuntimeError__LuaRuntimeError hook:", MH_CreateHook((LPVOID)dsl__LuaRuntimeError__LuaRuntimeError, &h_dsl__LuaRuntimeError__LuaRuntimeError, reinterpret_cast<LPVOID*>(&o_dsl__LuaRuntimeError__LuaRuntimeError)));
  //MINHOOK_ERROR_CHECKING("Failed to enable dsl__LuaRuntimeError__LuaRuntimeError hook:", MH_EnableHook((LPVOID)dsl__LuaRuntimeError__LuaRuntimeError));
}

BOOL __stdcall DllMain(HINSTANCE instance, DWORD reason, LPVOID reserved) {
  DisableThreadLibraryCalls(instance);

  
  if(reason == DLL_PROCESS_ATTACH) {
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
    MINHOOK_ERROR_CHECKING("Failed to initialise MinHook:", MH_Initialize());

    MultiFileTransportTesting();
    //wxSetInstance(instance);
    //wxSetInstance(GetModuleHandleA("raid_win64_release.exe"));
    wxInitialize();
    setup_EWS(instance);
  } else if(reason == DLL_PROCESS_DETACH) {
    if (wxAppConsole::GetInstance() != nullptr) {
      for (auto event : registered_events) {
        delete event;
      }
      wxTheApp->OnExit();
      delete wxTheApp;
      wxEntryCleanup();
      wxUninitialize();
    }
    MH_Uninitialize();
  }
  return TRUE;
}
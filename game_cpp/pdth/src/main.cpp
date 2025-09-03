#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Psapi.h>

#include <lua_funcs.h>

#include <EWS.h>
#include <HashlistLib.h>

#include <wx/wx.h>
#include <wx/evtloop.h>

#include <MinHook.h>

#include <filesystem>

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
#pragma comment(lib, "Version.lib")

#pragma optimize("", off)

size_t FindPattern(const char* module, const char* funcname, const char* pattern, const char* mask);

#define X86_THISCALL_HOOK_HELPER_DEFINE_FUNCTION(classname, name, returnvalue, args) \
inline static returnvalue (classname::* o_##name)(args); \
returnvalue h_##name(args)

template<typename T>
void* get_class_func_addr(T* func) {
  union {
    T* pfunc;
    void* addr;
  };
  pfunc = func;
  return addr;
}

template<typename classT, typename memberT>
void* get_class_func_addr(memberT classT::* func) {
  union {
    memberT classT::* pfunc;
    void* addr;
  };
  pfunc = func;
  return addr;
}

#define MH_STATUS_CHECK(debugname, status)\
if(status != MH_OK) {\
std::cout << debugname ": " << MH_StatusToString(status);\
}

#define X86_THISCALL_HOOK_HELPER_HOOK_CLASS_FUNCTION_ADDRESS(classname, name, address) \
{\
auto addr = (void*)address;\
MH_STATUS status = MH_CreateHook((LPVOID)addr, get_class_func_addr(&classname::h_##name), (LPVOID*)get_class_func_addr(&classname::o_##name));\
MH_STATUS_CHECK(#classname "::" #name " CreateHook", status)\
status = MH_EnableHook((LPVOID)addr);\
MH_STATUS_CHECK(#classname "::" #name " EnableHook", status)\
}

#define X86_NON_THISCALL_HOOK_HELPER_HOOK_FUNCTION_ADDRESS(name, address) \
{\
auto addr = (void*)address;\
MH_STATUS status = MH_CreateHook((LPVOID)addr, (LPVOID)&h_##name, (LPVOID*)&o_##name);\
MH_STATUS_CHECK(#name " CreateHook", status)\
status = MH_EnableHook((LPVOID)addr);\
MH_STATUS_CHECK(#name " EnableHook", status)\
}
#define X86_THISCALL_HOOK_HELPER_HOOK_CLASS_FUNCTION(classname, name, pattern, mask) \
X86_THISCALL_HOOK_HELPER_HOOK_CLASS_FUNCTION_ADDRESS(classname, name, FindPattern("payday_win32_release.exe", #classname "::" #name, pattern, mask));
#define X86_NON_THISCALL_HOOK_HELPER_HOOK_FUNCTION(name, pattern, mask) \
X86_NON_THISCALL_HOOK_HELPER_HOOK_FUNCTION_ADDRESS(name, FindPattern("payday_win32_release.exe", #name, pattern, mask));

#define X86_NON_THISCALL_HOOK_HELPER_DEFINE_FUNCTION(name, callconv, returnvalue, args)\
typedef returnvalue(callconv* name##_t)(args);\
name##_t o_##name;\
returnvalue callconv h_##name(args)

int EWS_Log(lua_State* L)
{
  if (lua_type(L, 1) == LUA_TSTRING) {
    std::cout << lua_tostring(L, 1) << std::endl;
  }
  return 0;
}

lua_CFunction luaB_dofile;

struct dsl__LuaInterpreter {
  lua_State* L;

  X86_THISCALL_HOOK_HELPER_DEFINE_FUNCTION(dsl__LuaInterpreter, constructor, dsl__LuaInterpreter*, CONCAT_ARGS(int thread, bool libs, bool classes, bool main, int a))
  {
    (this->*o_constructor)(thread, libs, classes, main, a);

    if (main && libs) {
      //EWS::add_members(L);
      lua_register(L, "EWS_Log", EWS_Log);
      lua_register(L, "dofile", luaB_dofile);

      luaL_loadstring(L, "dofile(\"./mods/ews/ews_setup.lua\")");

      int error = lua_pcall(L, 0, 0, 0);
      if (error) {
        std::cout << lua_tostring(L, -1) << std::endl;
        __debugbreak();
        lua_pop(L, 1);
      }
    }

    return this;
  }
};

struct PDTH_MSVC2008_string {
  char PAD[3];
  union {
    char data[16];
    char* ptr;
  };
  unsigned int _Mysize;
  unsigned int _Myres;

  const char* get_str() const
  {
    if (_Myres > 15) {
      return ptr;
    }
    else {
      return data;
    }
  }
};
static_assert(sizeof(PDTH_MSVC2008_string) == 0x1C);
static_assert(offsetof(PDTH_MSVC2008_string, _Mysize) == 0x14);

// TEMP DEBUGGING
struct FileDataStore {
  X86_THISCALL_HOOK_HELPER_DEFINE_FUNCTION(FileDataStore, constructor, int, CONCAT_ARGS(PDTH_MSVC2008_string* path, DWORD create_mode, DWORD access_, DWORD share_mode))
  {
    if (!std::filesystem::exists(path->get_str()))
      __debugbreak();

    auto ret = (this->*o_constructor)(path, create_mode, access_, share_mode);

    return ret;
  }
};

struct dsl__LuaRuntimeError {
  virtual ~dsl__LuaRuntimeError() {};
  virtual const char* what() { return "[dsl::LuaRuntimeError::what] called on non Diesel class instance!"; };

  X86_THISCALL_HOOK_HELPER_DEFINE_FUNCTION(dsl__LuaRuntimeError, constructor, dsl__LuaRuntimeError*, CONCAT_ARGS(lua_State* L, PDTH_MSVC2008_string* what))
  {
    auto ret = (this->*o_constructor)(L, what);

    std::cout << "LuaRuntimeError thrown:" << std::endl;
    std::cout << this->what() << std::endl;

    return ret;
  }
};

int dsl__ScriptIdstring__s(lua_State* L) {
  auto _id = *(dsl::idstring*)lua_touserdata(L, 1);

  lua_pushstring(L, attempt_to_find_source_idstring_from_hashlist(_id).c_str());

  return 1;
}

X86_NON_THISCALL_HOOK_HELPER_DEFINE_FUNCTION(ScriptIdstring__add_members, __cdecl, int, CONCAT_ARGS(lua_State* L))
{
  auto ret = o_ScriptIdstring__add_members(L);

  lua_pushcclosure(L, dsl__ScriptIdstring__s, 0);
  lua_setfield(L, -2, "s");

  return ret;
}

class DieselApp : public wxApp {
public:
  bool OnInit() override;

  wxEventLoopBase* GetDieselMainLoop()
  {
    //wxEventLoopBaseTiedPtr mainLoop(&m_mainLoop, CreateMainLoop());
    if (m_mainLoop == nullptr) {
      m_mainLoop = CreateMainLoop();
      wxEventLoopBase::SetActive(m_mainLoop);
    }

    return m_mainLoop;
  }
};

bool DieselApp::OnInit()
{
  return true;
}

struct Application {
  X86_THISCALL_HOOK_HELPER_DEFINE_FUNCTION(Application, update, char, CONCAT_ARGS())
  {
    auto ret = (this->*o_update)();

    if (wxTheApp) {
      wxTheApp->ProcessPendingEvents();
      ((DieselApp*)wxTheApp)->GetDieselMainLoop()->DispatchTimeout(1);
    }

    return ret;
  }
};

typedef void (__cdecl *LuaInterpreter_initializer)(lua_State* L);
void(__cdecl* dsl__LuaInterpreter__register_initializer)(LuaInterpreter_initializer func);

void setup_EWS() {
  if(AttachConsole(ATTACH_PARENT_PROCESS) || AllocConsole())
    freopen("CONOUT$", "w", stdout);

  pdth_lua_setup(FindPattern);

  wxApp::SetInstance(new DieselApp());

  wxEntryStart(GetModuleHandleA("payday_win32_release.exe"));

  wxImage::AddHandler(new wxPNGHandler);
  wxImage::AddHandler(new wxBMPHandler);

  ((DieselApp*)wxTheApp)->GetDieselMainLoop();

  wxSizerFlags::DisableConsistencyChecks();

  load_hashlist();

  luaB_dofile = (lua_CFunction)FindPattern("payday_win32_release.exe", "luaB_dofile", "\x56\x8B\x74\x24\x00\x8B\x46\x00\x57\x3B\x46\x00\x73\x00\x3D\x00\x00\x00\x00\x74\x00\x83\x78\x00\x00\x7F", "xxxx?xx?xxx?x?x????x?xx??x");
  dsl__LuaInterpreter__register_initializer = (decltype(dsl__LuaInterpreter__register_initializer))FindPattern("payday_win32_release.exe", "", "\x64\xA1\x00\x00\x00\x00\x6A\x00\x68\x00\x00\x00\x00\x50\x64\x89\x25\x00\x00\x00\x00\xA1\x00\x00\x00\x00\x83\xEC\x00\x53\x8B\x5C\x24", "xx????x?x????xxxx????x????xx?xxxx");

  dsl__LuaInterpreter__register_initializer(EWS::add_members);

  X86_THISCALL_HOOK_HELPER_HOOK_CLASS_FUNCTION(Application, update, "\x55\x8B\xEC\x83\xE4\x00\x6A\x00\x64\xA1\x00\x00\x00\x00\x68\x00\x00\x00\x00\x50\x64\x89\x25\x00\x00\x00\x00\x81\xEC", "xxxxx?x?xx????x????xxxx????xx");
  X86_THISCALL_HOOK_HELPER_HOOK_CLASS_FUNCTION(dsl__LuaInterpreter, constructor, "\x6A\x00\x68\x00\x00\x00\x00\x64\xA1\x00\x00\x00\x00\x50\x64\x89\x25\x00\x00\x00\x00\x83\xEC\x00\x53\x33\xDB\x56\x8B\xF1\x89\x1E", "x?x????xx????xxxx????xx?xxxxxxxx");
  X86_THISCALL_HOOK_HELPER_HOOK_CLASS_FUNCTION(dsl__LuaRuntimeError, constructor, "\x6A\x00\x68\x00\x00\x00\x00\x64\xA1\x00\x00\x00\x00\x50\x64\x89\x25\x00\x00\x00\x00\x81\xEC\x00\x00\x00\x00\x53\x55\x56\x57\x8B\xBC\x24\x00\x00\x00\x00\x33\xDB", "x?x????xx????xxxx????xx????xxxxxxx????xx");
  X86_THISCALL_HOOK_HELPER_HOOK_CLASS_FUNCTION(FileDataStore, constructor, "\x6A\x00\x68\x00\x00\x00\x00\x64\xA1\x00\x00\x00\x00\x50\x64\x89\x25\x00\x00\x00\x00\x83\xEC\x00\x53\x56\x8B\xF1\x57\x89\x74\x24\x00\x8B\x7C\x24", "x?x????xx????xxxx????xx?xxxxxxxx?xxx");
  X86_NON_THISCALL_HOOK_HELPER_HOOK_FUNCTION(ScriptIdstring__add_members, "\x56\x8B\x74\x24\x00\x33\xC9\x51\xB8\x00\x00\x00\x00\x68\x00\x00\x00\x00\x56\xA3\x00\x00\x00\x00\x89\x0D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x68\x00\x00\x00\x00\x6A\x00\x56\xE8\x00\x00\x00\x00\x33\xC9\x51\xB8\x00\x00\x00\x00\x68\x00\x00\x00\x00\x56\xA3\x00\x00\x00\x00\x89\x0D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x68\x00\x00\x00\x00\x6A\x00\x56\xE8\x00\x00\x00\x00\x6A", "xxxx?xxxx????x????xx????xx????x????x????x?xx????xxxx????x????xx????xx????x????x????x?xx????x");
  
}

BOOL __stdcall DllMain(HINSTANCE, DWORD reason, LPVOID reserved) {
  if(reason == DLL_PROCESS_ATTACH) {
    MH_Initialize();
    wxInitialize();

    setup_EWS();

  }
  else if (reason == DLL_PROCESS_DETACH) {
    if (wxAppConsole::GetInstance() != nullptr) {
      wxTheApp->OnExit();
      delete wxTheApp;
      wxEntryCleanup();
      wxUninitialize();
    }
    MH_Uninitialize();
  }
  return TRUE;
}


static MODULEINFO GetModuleInfo(std::string szModule)
{
  MODULEINFO modinfo = { 0 };
  HMODULE hModule = GetModuleHandleA(szModule.c_str());
  if (hModule == 0)
    return modinfo;
  GetModuleInformation(GetCurrentProcess(), hModule, &modinfo, sizeof(MODULEINFO));
  return modinfo;
}
static bool CheckSignature(const char* pattern, size_t patternLength, const char* mask, size_t base, size_t size, size_t i,
  size_t* result)
{
  bool found = true;
  for (size_t j = 0; j < patternLength; j++) {
    found &= mask[j] == '?' || pattern[j] == *(char*)(base + i + j);
  }
  if (found) {
    // printf("Found %s: 0x%p\n", funcname, base + i);
    *result = base + i;
    return true;
  }

  return false;
}
static size_t FindPattern(const char* module, const char* funcname, const char* pattern, const char* mask)
{

  MODULEINFO mInfo = GetModuleInfo(module);
  size_t base = (size_t)mInfo.lpBaseOfDll;
  size_t size = (size_t)mInfo.SizeOfImage;
  size_t patternLength = (size_t)strlen(mask);

  for (size_t i = 0; i < size - patternLength; i++) {
    size_t result;
    bool correct = CheckSignature(pattern, patternLength, mask, base, size, i, &result);
    if (correct) {
#ifdef CHECK_DUPLICATE_SIGNATURES
      // Sigdup checking
      for (size_t ci = i + 1; ci < size - patternLength; ci++) {
        size_t addr;
        bool correct = CheckSignature(pattern, patternLength, mask, base, size, ci, &addr);
        if (correct) {
          string err = string("Found duplicate signature for ") + string(funcname) + string(" at ") +
            to_string(result) + string(",") + to_string(addr);
          RAIDHOOK_LOG_WARN(err);
          hintOut = NULL; // Don't cache sigs with errors
        }
      }
#endif

      return result;
    }
  }
  return NULL;
}

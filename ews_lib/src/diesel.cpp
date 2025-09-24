#include "diesel.h"

namespace dsl {
  decltype(dsl::push_lua_Vector3) push_lua_Vector3 = nullptr;
}

void dsl::init_push_lua_functions(SignatureScannerFunction scanner)
{
#if GAME_RAIDWW2
  dsl::push_lua_Vector3 = (decltype(dsl::push_lua_Vector3))scanner("raid_win64_release.exe", "push_lua_Vector3", "\x48\x83\xEC\x00\xF3\x0F\x10\x02\xF3\x0F\x11\x44\x24", "xxx?xxxxxxxxx");
#else
#error Missing signature for dsl::push_lua<Vector3>
#endif
}

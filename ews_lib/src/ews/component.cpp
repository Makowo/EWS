#include "component.h"

Component::~Component() {
}

void Component::AddLuaFunctions(lua_State* L)
{
  REGISTER_LUA_CLASS_FUNCTION(EWS_EventHandler_Connect, "connect");
}

int EWS_Component_GarbageCollection(lua_State* L) {
  auto component = get_ews_object_from_top<Component>(L, 1);

  (void)component;

  __debugbreak();
  return 0;
}

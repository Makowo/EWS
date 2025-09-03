#include "Control.h"

void Control::AddLuaFunctions(lua_State* L)
{
  Window::AddLuaFunctions(L);
}

#pragma once

struct lua_State;

#ifdef EWS_STATIC_BUILD
#define EXPORT __declspec(dllexport)
#else
#define EXPORT __declspec(dllimport)
#endif

class __declspec(dllexport) EWS
{
public:
  EWS() = delete;
  EWS(const EWS& other) = delete;
  EWS& operator=(const EWS& other) = delete;
public:
  static void add_members(lua_State* L);
};
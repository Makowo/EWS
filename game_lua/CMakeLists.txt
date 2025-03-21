cmake_minimum_required(VERSION 3.26)

if(GAME)

set(GAME_LUA_DIR ${CMAKE_SOURCE_DIR}/game_lua)

  add_library(game_lua STATIC
    ${GAME_LUA_DIR}/src/gamelua.cpp
    ${GAME_LUA_DIR}/src/raidww2.cpp
    ${GAME_LUA_DIR}/src/pdth.cpp
    ${GAME_LUA_DIR}/src/pd2.cpp
  )

target_include_directories(game_lua PRIVATE
  ${GAME_LUA_DIR}/include
)

if(WIN64)
target_compile_definitions(game_lua PRIVATE GAME_LUA_COMP WIN64)
else()
target_compile_definitions(game_lua PRIVATE GAME_LUA_COMP)
endif()

endif()
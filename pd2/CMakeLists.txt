cmake_minimum_required(VERSION 3.26)

if(NOT WIN64)

set(PD2_DIR ${CMAKE_SOURCE_DIR}/pd2)

file(GLOB_RECURSE PD2_SOURCE ${PD2_DIR}/src/*.cpp)

  add_library(EWS_PD2 SHARED
    ${PD2_SOURCE}
  )

  target_include_directories(EWS_PD2 PRIVATE
    ${EWS_LIB_INCLUDE_DIR}
    ${GAME_LUA_INCLUDE_DIR}
  )

  target_link_libraries(EWS_PD2 ews_lib game_lua)

endif()
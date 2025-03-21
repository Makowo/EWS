cmake_minimum_required(VERSION 3.26)

if(NOT WIN64)

set(PDTH_DIR ${CMAKE_SOURCE_DIR}/pdth)

file(GLOB_RECURSE PDTH_SOURCE ${PDTH_DIR}/src/*.cpp)

add_library(EWS_PDTH SHARED
${PDTH_SOURCE}
)

target_include_directories(EWS_PDTH PRIVATE
  ${EWS_LIB_INCLUDE_DIR}
  ${GAME_LUA_INCLUDE_DIR}
)

target_link_libraries(EWS_PDTH ews_lib game_lua)

endif()
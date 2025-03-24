cmake_minimum_required(VERSION 3.26)

set(RAID_WW2_DIR ${CMAKE_SOURCE_DIR}/raidww2)

file(GLOB_RECURSE RAID_SOURCE ${RAID_WW2_DIR}/src/*.cpp)

add_library(EWS_RAIDWW2 SHARED
${RAID_SOURCE}
)

target_include_directories(EWS_RAIDWW2 PRIVATE
  ${EWS_LIB_INCLUDE_DIR}
  ${GAME_LUA_INCLUDE_DIR}
  ${CMAKE_SOURCE_DIR}/Dependencies/minhook/include
  ${WX_DIR}/lib/vc_lib/mswud
  ${WX_DIR}/include
  ${WX_DIR}/interface
)

target_link_libraries(EWS_RAIDWW2 ews_lib minhook ${WX_LIBS})
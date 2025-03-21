cmake_minimum_required(VERSION 3.26)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCE ${CMAKE_SOURCE_DIR}/ews_lib/src/*.cpp)

set(EWS_LIB_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/ews_lib/include)

add_library(ews_lib STATIC
  ${SOURCE}
  ${CMAKE_SOURCE_DIR}/ews_lib/include/EWS.h
  )

target_include_directories(ews_lib PRIVATE
  ${CMAKE_SOURCE_DIR}/ews_lib/include
  ${CMAKE_SOURCE_DIR}/ews_lib/src
  ${CMAKE_SOURCE_DIR}/ews_lib/src/ews
  ${GAME_LUA_INCLUDE_DIR}
  ${WX_DIR}/lib/vc_lib/mswud
  ${WX_DIR}/include
  ${WX_DIR}/interface
)


if(GAME)
target_link_libraries(ews_lib game_lua ${WX_LIBS} Comctl32.lib Rpcrt4.lib)
else()
target_link_libraries(ews_lib ${LUA_LIB_FILE} ${WX_LIBS} Comctl32.lib Rpcrt4.lib)
endif()

if(GAME)
target_compile_definitions(ews_lib PRIVATE GAME)
endif()
#target_link_libraries(ews_lib game_lua Comctl32.lib Rpcrt4.lib)
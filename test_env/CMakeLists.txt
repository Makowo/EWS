cmake_minimum_required(VERSION 3.26)

add_executable(test_env
  src/main.cpp
)

target_include_directories(test_env PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/../ews_lib/include
  ${GAME_LUA_INCLUDE_DIR}
  ${WX_DIR}/lib/vc_lib/mswud
  ${WX_DIR}/include
  ${WX_DIR}/interface
)

target_link_libraries(test_env ews_lib ${LUA_LIB_FILE} ${WX_LIBS})
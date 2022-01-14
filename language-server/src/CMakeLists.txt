cmake_minimum_required(VERSION 3.21.0)

add_subdirectory(lsp)
add_subdirectory(json)

add_library(rc-language-server-lib
  server.cpp
  interpreter.cpp
  str_utils.cpp
)

target_link_libraries(rc-language-server-lib json lsp)

add_executable(rc-language-server
  main.cpp
)

target_link_libraries(rc-language-server rc-language-server-lib)
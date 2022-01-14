cmake_minimum_required(VERSION 3.13.0)

add_library(lsp
  message.cpp
)
target_include_directories(lsp PUBLIC ${SRC_DIR})
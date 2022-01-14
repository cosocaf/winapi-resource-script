cmake_minimum_required(VERSION 3.13.0)

add_library(json
  string.cpp
  number.cpp
  boolean.cpp
  null.cpp
  array.cpp
  object.cpp
  element.cpp
  decoder.cpp
)
target_include_directories(json PUBLIC ${SRC_DIR})
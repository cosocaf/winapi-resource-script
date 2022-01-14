cmake_minimum_required(VERSION 3.21.0)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/release-1.11.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

function(add_unit_test name)
  add_executable(${name} ${name}.cpp)
  target_include_directories(${name} PUBLIC
    ${SRC_DIR}
  )
  target_link_libraries(${name}
    rc-language-server-lib
    gtest_main
  )
  gtest_discover_tests(${name})
endfunction(add_unit_test)

include(GoogleTest)

add_unit_test(json_decoder_test)

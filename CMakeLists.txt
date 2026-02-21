cmake_minimum_required(VERSION 3.20)
project(json_helpers LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(json_helpers INTERFACE)
add_library(json_helpers::json_helpers ALIAS json_helpers)

target_include_directories(json_helpers INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

if (MSVC)
  target_compile_options(json_helpers INTERFACE /W4 /permissive-)
else()
  target_compile_options(json_helpers INTERFACE -Wall -Wextra -Wpedantic)
endif()

include(CTest)
enable_testing()

add_executable(json_helpers_basic_test tests/test_basic.cpp)
target_link_libraries(json_helpers_basic_test PRIVATE json_helpers::json_helpers)
add_test(NAME json_helpers.basic COMMAND json_helpers_basic_test)

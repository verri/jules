cmake_minimum_required(VERSION 3.17)
project(Jules VERSION 0.5.0)

configure_file("include/jules/version.hpp.in" "include/jules/version.hpp")
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules ${CMAKE_MODULE_PATH})

add_library(jules INTERFACE)

target_compile_features(jules INTERFACE cxx_std_23)

target_include_directories(jules INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include/>)
target_include_directories(jules SYSTEM INTERFACE $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>)

find_package(debug_assert 1.1 QUIET)
if(NOT ${debug_assert_FOUND})
  message("Unable to find debug_assert, using submodule...")
  add_subdirectory(third_party/debug_assert)
endif()
target_link_libraries(jules INTERFACE debug_assert)

option(JULES_BUILD_TEST "whether or not to build the test" OFF)
if(JULES_BUILD_TEST)
  enable_testing()
  add_subdirectory(test)
endif()

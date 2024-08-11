cmake_minimum_required(VERSION 3.17)
project(Jules VERSION 0.5.0)

configure_file("include/jules/version.hpp.in" "include/jules/version.hpp")
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules ${CMAKE_MODULE_PATH})

add_library(jules INTERFACE)

target_compile_features(jules INTERFACE cxx_std_20)

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

find_package(range-v3 0.10.0 QUIET)
if(NOT ${range-v3_FOUND})
  message("Unable to find range-v3, using submodule...")
  set(RANGES_CXX_STD 17 CACHE STRING "")
  set(RANGE_V3_NO_HEADER_CHECK ON CACHE BOOL "")
  set(RANGE_V3_NO_TESTING ON CACHE BOOL "")
  set(RANGE_V3_NO_PERF ON CACHE BOOL "")
  set(RANGE_V3_NO_EXAMPLE ON CACHE BOOL "")
  add_subdirectory(third_party/range-v3)
endif()
target_link_libraries(jules INTERFACE range-v3)

option(JULES_BUILD_TEST "whether or not to build the test" OFF)
if(JULES_BUILD_TEST)
  enable_testing()
  add_subdirectory(test)
endif()

include(CMakePackageConfigHelpers)

file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/jules-config.cmake "
find_package(range-v3 0.10.0 REQUIRED)
find_package(debug_assert 1.1 REQUIRED)
include(\${CMAKE_CURRENT_LIST_DIR}/jules-targets.cmake)
")

write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/jules-config-version.cmake
  VERSION ${JULES_VERSION}
  COMPATIBILITY ExactVersion
)

install(TARGETS jules EXPORT jules-targets DESTINATION lib)
install(EXPORT jules-targets DESTINATION lib/cmake/jules)
install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/jules-config.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/jules-config-version.cmake
  DESTINATION lib/cmake/jules)

install(DIRECTORY include/ DESTINATION include FILES_MATCHING PATTERN "*.hpp")
install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/include/ DESTINATION include FILES_MATCHING PATTERN "*.hpp")

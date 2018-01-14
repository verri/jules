cmake_minimum_required(VERSION 3.2)
project(Jules VERSION 0.4.0)

configure_file("include/jules/version.hpp.in" "include/jules/version.hpp")
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules ${CMAKE_MODULE_PATH})

add_library(jules INTERFACE)

target_compile_features(jules INTERFACE cxx_std_17)

set(header_files
  include/jules/core/detail/numbers.hpp
  include/jules/core/debug.hpp
  include/jules/core/meta.hpp
  include/jules/core/type.hpp
  include/jules/core/range.hpp
  include/jules/version.hpp
  include/jules/array/strided_descriptor.hpp
  include/jules/array/slicing.hpp
  include/jules/array/expr_array.hpp
  include/jules/array/fwd.hpp
  include/jules/array/builder.hpp
  include/jules/array/meta/reference.hpp
  include/jules/array/meta/common.hpp
  include/jules/array/meta/expression.hpp
  include/jules/array/io.hpp
  include/jules/array/ref_array.hpp
  include/jules/array/detail/iterator.hpp
  include/jules/array/detail/common.hpp
  include/jules/array/unary_expr_array.hpp
  include/jules/array/mapper.hpp
  include/jules/array/slicing/absolute.hpp
  include/jules/array/slicing/bounded.hpp
  include/jules/array/slicing/range.hpp
  include/jules/array/slicing/relative.hpp
  include/jules/array/functional.hpp
  include/jules/array/blas.hpp
  include/jules/array/overlap.hpp
  include/jules/array/math.hpp
  include/jules/array/binary_expr_array.hpp
  include/jules/array/strided_ref_array.hpp
  include/jules/array/descriptor.hpp
  include/jules/array/numeric.hpp
  include/jules/array/allocator.hpp
  include/jules/array/array.hpp
  include/jules/base/index_view.hpp
  include/jules/base/random.hpp
  include/jules/base/async.hpp
  include/jules/base/math.hpp
  include/jules/base/const_vector.hpp
  include/jules/base/numeric.hpp
  include/jules/dataframe/detail/common.hpp
  include/jules/dataframe/detail/column_model.hpp
  include/jules/dataframe/column.hpp
  include/jules/dataframe/action.hpp
  include/jules/dataframe/dataframe.hpp
  include/jules/dataframe/numeric.hpp)

target_sources(jules INTERFACE
  $<INSTALL_INTERFACE:${header_files}>
  $<INSTALL_INTERFACE:include/jules/version.hpp>)

target_include_directories(jules INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>)
target_include_directories(jules SYSTEM INTERFACE $<INSTALL_INTERFACE:/include>)

find_package(debug_assert 1.1 QUIET)
if(NOT ${debug_assert_FOUND})
  message("Unable to find debug_assert, using submodule...")
  add_subdirectory(third_party/debug_assert)
endif()
target_link_libraries(jules INTERFACE debug_assert)

set(JULES_RANGE_V3_INCLUDE_DIR "" CACHE STRING "custom path to the range-v3 library")
if(JULES_RANGE_V3_INCLUDE_DIR)
  target_include_directories(jules INTERFACE $<BUILD_INTERFACE:${JULES_RANGE_V3_INCLUDE_DIR}>)
else()
  target_include_directories(jules INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/third_party/range-v3/include/>)
  target_include_directories(jules SYSTEM INTERFACE $<INSTALL_INTERFACE:/third_party/range-v3/include>)
endif()

option(JULES_BUILD_TEST "build test" ON)
if(${JULES_BUILD_TEST})
  enable_testing()
  add_subdirectory(test)
endif()

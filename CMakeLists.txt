cmake_minimum_required(VERSION 3.6)
project(Jules VERSION 0.4.0)

configure_file("include/jules/version.hpp.in" "include/jules/version.hpp")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(jules INTERFACE)

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

install(DIRECTORY include/ DESTINATION include FILES_MATCHING PATTERN "*.hpp")

option(JULES_BUILD_TEST "build test" ON)
if(${JULES_BUILD_TEST})
  enable_testing()
  add_subdirectory(test)
endif()

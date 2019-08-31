if(NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/catch.hpp)
  file(DOWNLOAD
    https://github.com/catchorg/Catch2/releases/download/v2.1.0/catch.hpp
    ${CMAKE_CURRENT_BINARY_DIR}/catch.hpp
    STATUS status
    LOG log)

  list(GET status 0 status_code)
  list(GET status 1 status_string)

  if(NOT status_code EQUAL 0)
    message(FATAL_ERROR "error downloading catch: ${status_string}" "${log}")
  endif()
endif()

find_package(CBLAS REQUIRED)
if (NOT ${CBLAS_FOUND})
  message(FATAL_ERROR "CBLAS not found.")
endif()

set(source_files
  core/debug.cpp
  core/range.cpp
  array/axis.cpp
  array/functional.cpp
  array/strided_ref_array.cpp
  array/numeric.cpp
  array/math.cpp
  array/io.cpp
  array/ref_array.cpp
  array/builder.cpp
  array/slicing.cpp
  array/vector.cpp
  array/allocator.cpp
  array/matrix.cpp
  array/blas.cpp
  array/array.cpp
  base/const_vector.cpp
  base/random.cpp
  base/numeric.cpp
  dataframe/column.cpp
  dataframe/column_model.cpp
  dataframe/dataframe.cpp
  dataframe/action.cpp
  test_suite.cpp
  version.cpp)

add_executable(jules_test_suite ${source_files})
set_target_properties(jules_test_suite PROPERTIES CXX_STANDARD_REQUIRED ON CXX_EXTENSIONS OFF)

option(JULES_TEST_COVERAGE "whether or not add coverage instrumentation" OFF)
if (JULES_TEST_COVERAGE)
  target_compile_options(jules_test_suite PRIVATE "$<$<CONFIG:DEBUG>:-O0>")
  target_compile_options(jules_test_suite PRIVATE "$<$<CONFIG:DEBUG>:--coverage>")
  set_target_properties(jules_test_suite PROPERTIES LINK_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} --coverage")
else()
  target_compile_options(jules_test_suite PRIVATE "$<$<CONFIG:DEBUG>:-O0>")
  target_compile_options(jules_test_suite PRIVATE "$<$<CONFIG:DEBUG>:-fno-omit-frame-pointer>")
  target_compile_options(jules_test_suite PRIVATE "$<$<CONFIG:DEBUG>:-fsanitize=address,leak,undefined>")
  set_target_properties(jules_test_suite PROPERTIES LINK_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fsanitize=address,leak,undefined -fuse-ld=gold")
endif()

target_include_directories(jules_test_suite PUBLIC
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CBLAS_INCLUDE_DIR})

target_link_libraries(jules_test_suite PUBLIC
  jules
  ${CBLAS_LIBRARIES})

target_compile_definitions(jules_test_suite PUBLIC -DJULES_DEBUG_THROWS)

if(MSVC)
  if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  else()
    target_compile_options(jules_test_suite PRIVATE "/W4")
  endif()
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  target_compile_options(jules_test_suite PRIVATE -Wall -Wextra -Werror -pedantic)
endif()

add_test(NAME test.jules COMMAND jules_test_suite)

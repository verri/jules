Include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.0.1 # or a later release
)

FetchContent_MakeAvailable(Catch2)

find_package(CBLAS REQUIRED)
if (NOT ${CBLAS_FOUND})
  message(FATAL_ERROR "CBLAS not found.")
endif()

find_package(Boost 1.72 REQUIRED)

set(source_files
  core/debug.cpp
  core/type.cpp
  core/range.cpp
  core/string.cpp
  array/axis.cpp
  array/functional.cpp
  array/mapped_ref_array.cpp
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
  base/random.cpp
  base/math.cpp
  base/numeric.cpp
  data/column.cpp
  data/column_model.cpp
  data/data.cpp
  # data/action.cpp
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
  ${CBLAS_INCLUDE_DIR}
  ${Boost_INCLUDE_DIRS})

target_link_libraries(jules_test_suite PUBLIC
  jules
  ${CBLAS_LIBRARIES}
  Catch2::Catch2WithMain)

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

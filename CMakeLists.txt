cmake_minimum_required(VERSION 3.1)
project(Jules VERSION 0.4.0)

configure_file(
  "include/jules/version.hpp.in"
  "include/jules/version.hpp")

add_library(jules INTERFACE)
file(GLOB_RECURSE header_files include/*.hpp)
target_sources(jules INTERFACE $<INSTALL_INTERFACE:${header_files} $<INSTALL_INTERFACE:include/jules/version.hpp)

install(DIRECTORY include/ DESTINATION include FILES_MATCHING PATTERN "*.hpp")

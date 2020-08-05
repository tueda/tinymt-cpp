cmake_minimum_required(VERSION 3.11)

# Detect if the project is being bundled.

if(NOT DEFINED PROJECT_NAME)
  set(NOT_SUBPROJECT ON)
endif()

# Project information.

project(
  tinymt
  VERSION 0.0.1
  DESCRIPTION
    "A C++ implementation of the TinyMT pseudo random number generator"
  LANGUAGES CXX)

add_library(${PROJECT_NAME} INTERFACE)

target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_11)
target_include_directories(
  ${PROJECT_NAME}
  INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>)

if(NOT_SUBPROJECT)
  include(CTest)

  # Additional compiler flags for testing.

  include(CheckCXXCompilerFlag)
  check_cxx_compiler_flag("-Wall -Wextra" CXX_HAS_ALL_WARNING_FLAG)
  if(CXX_HAS_ALL_WARNING_FLAG)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
  endif()

  check_cxx_compiler_flag("-Wshadow" CXX_HAS_SHADOW_FLAG)
  if(CXX_HAS_SHADOW_FLAG)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wshadow")
  endif()

  check_cxx_compiler_flag("-pedantic" CXX_HAS_PEDANTIC_FLAG)
  if(CXX_HAS_PEDANTIC_FLAG)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic")
  endif()
endif()

if((NOT_SUBPROJECT OR TINYMT_CPP_BUILD_TESTING) AND BUILD_TESTING)
  add_subdirectory(tests)
endif()

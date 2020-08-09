cmake_minimum_required(VERSION 3.11)

# Detect if the project is being bundled.

if(NOT DEFINED PROJECT_NAME)
  set(NOT_SUBPROJECT ON)
endif()

# Project information.

project(
  tinymt
  VERSION 0.0.1
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

  # Document generation.

  find_package(Doxygen)

  if(DOXYGEN_FOUND)
    add_subdirectory(docs)
  else()
    message(STATUS "Doxygen not found")
  endif()

  # Code coverage.

  add_library(coverage_config INTERFACE)

  option(USE_CODE_COVERAGE "Enable coverage reporting" OFF)

  if(USE_CODE_COVERAGE)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
      if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(coverage_config INTERFACE -O0 -g -coverage)
        if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.13)
          target_link_options(coverage_config INTERFACE -coverage)
        else()
          target_link_libraries(coverage_config INTERFACE -coverage)
        endif()
      else()
        message(STATUS "Coverage reporting requires GCC or LLVM/Clang")
      endif()
    else()
      message(STATUS "Coverage reporting requires Debug build type")
    endif()
  endif()

  # ASAN/UBSAN.

  add_library(sanitizer_config INTERFACE)

  option(USE_SANITIZER "Enable compiler sanitizers" OFF)

  if(USE_SANITIZER)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
      if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(
          sanitizer_config INTERFACE -O0 -g -fsanitize=address,undefined
                                     -fno-omit-frame-pointer)
        if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.13)
          target_link_options(sanitizer_config INTERFACE
                              -fsanitize=address,undefined)
        else()
          target_link_libraries(sanitizer_config
                                INTERFACE -fsanitize=address,undefined)
        endif()
      else()
        message(STATUS "Compiler sanitizers require GCC or LLVM/Clang")
      endif()
    else()
      message(STATUS "Compiler sanitizers require Debug build type")
    endif()
  endif()

  # Clang-Tidy.

  option(USE_CLANG_TIDY "Enable Clang-Tidy" OFF)

  if(USE_CLANG_TIDY)
    set(CMAKE_CXX_CLANG_TIDY clang-tidy)
  endif()
endif()

if((NOT_SUBPROJECT OR TINYMT_CPP_BUILD_TESTING) AND BUILD_TESTING)
  add_subdirectory(tests)

  if(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_subdirectory(benches)
  endif()
endif()

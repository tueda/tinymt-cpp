cmake_minimum_required(VERSION 3.8)

# Detect if the project is the root or being bundled.

if(NOT DEFINED PROJECT_NAME)
  set(IS_ROOT_PROJECT ON)
endif()

# Project information.

project(
  tinymt
  VERSION 0.0.1
  LANGUAGES CXX)

add_library(${PROJECT_NAME} INTERFACE)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_11)

target_include_directories(
  ${PROJECT_NAME}
  INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>)

if(IS_ROOT_PROJECT)
  include(CTest)

  set(PROJECT_CONFIG_LIB "tinymt_cpp_private_config")
  add_library(${PROJECT_CONFIG_LIB} INTERFACE)

  # Additional compiler flags for testing.

  set(CMAKE_CXX_EXTENSIONS OFF)

  include(CheckCXXCompilerFlag)

  macro(TRY_CXX_COMPILER_FLAG flag var)
    check_cxx_compiler_flag(${flag} ${var})
    if(${var})
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${flag}")
    endif()
  endmacro()

  function(check_cxx_compiler_and_linker_flag flag var)
    set(separated_flags ${flag})
    # cmake-lint: disable=E1120
    separate_arguments(separated_flags)
    # cmake-lint: disable=C0103
    set(CMAKE_REQUIRED_LINK_OPTIONS ${separated_flags})
    check_cxx_compiler_flag(${flag} ${var})
  endfunction()

  if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    try_cxx_compiler_flag("-Weverything -Wno-c++98-compat-pedantic"
                          CXX_HAS_WARN_EVERYTHING_FLAG)
  elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    try_cxx_compiler_flag("/W4" CXX_HAS_W4_FLAG)
    try_cxx_compiler_flag("/WX" CXX_HAS_WX_FLAG)
  else()
    # GCC.
    try_cxx_compiler_flag("-Wall -Wextra" CXX_HAS_WARN_ALL_AND_EXTRA_FLAG)
    try_cxx_compiler_flag("-Wshadow" CXX_HAS_WARN_SHADOW_FLAG)
    try_cxx_compiler_flag("-Wold-style-cast" CXX_HAS_WARN_OLD_STYLE_CAST_FLAG)
    try_cxx_compiler_flag("-Wsign-conversion" CXX_HAS_WARN_SIGN_CONVERSION_FLAG)
  endif()

  if(NOT (CMAKE_CXX_COMPILER_ID MATCHES "MSVC"))
    # GCC or Clang.
    try_cxx_compiler_flag("-Werror" CXX_HAS_ERROR_FLAG)
    try_cxx_compiler_flag("-pedantic" CXX_HAS_PEDANTIC_FLAG)
    try_cxx_compiler_flag("-pedantic-errors" CXX_HAS_PEDANTIC_ERRORS_FLAG)
  endif()

  # Document generation.

  option(BUILD_DOCUMENTS "Build documents" ON)

  if(BUILD_DOCUMENTS)
    find_package(Doxygen)

    if(DOXYGEN_FOUND)
      add_subdirectory(docs)
    else()
      message(WARNING "Building documents requires Doxygen")
    endif()
  endif()

  # Benchmarking.

  option(BUILD_BENCHMARKING "Build benchmarks" OFF)

  # Code coverage.

  option(USE_CODE_COVERAGE "Enable coverage reporting" OFF)

  if(USE_CODE_COVERAGE)
    check_cxx_compiler_and_linker_flag("-O0 -g -coverage" CXX_HAS_COVERAGE_FLAG)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
      if(CXX_HAS_COVERAGE_FLAG)
        target_compile_options(${PROJECT_CONFIG_LIB} INTERFACE -O0 -g -coverage)
        if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.13)
          target_link_options(${PROJECT_CONFIG_LIB} INTERFACE -coverage)
        else()
          target_link_libraries(${PROJECT_CONFIG_LIB} INTERFACE -coverage)
        endif()
      else()
        message(WARNING "Coverage reporting is not supported by the compiler")
      endif()
    else()
      message(WARNING "Coverage reporting requires GCC or LLVM/Clang")
    endif()
  endif()

  # ASAN/UBSAN.

  option(USE_SANITIZER "Enable compiler sanitizers" OFF)

  if(USE_SANITIZER)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
      check_cxx_compiler_and_linker_flag(
        "-O0 -g -fsanitize=address,undefined -fno-omit-frame-pointer"
        CXX_HAS_SANITIZER_FLAG)
      if(CXX_HAS_SANITIZER_FLAG)
        target_compile_options(
          ${PROJECT_CONFIG_LIB} INTERFACE -O0 -g -fsanitize=address,undefined
                                          -fno-omit-frame-pointer)
        if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.13)
          target_link_options(${PROJECT_CONFIG_LIB} INTERFACE
                              -fsanitize=address,undefined)
        else()
          target_link_libraries(${PROJECT_CONFIG_LIB}
                                INTERFACE -fsanitize=address,undefined)
        endif()
      else()
        message(WARNING "Compiler sanitizers are not supported by the compiler")
      endif()
    else()
      message(WARNING "Compiler sanitizers require GCC or LLVM/Clang")
    endif()
  endif()

  # Clang-Tidy.

  option(USE_CLANG_TIDY "Enable Clang-Tidy" OFF)

  if(USE_CLANG_TIDY)
    set(CMAKE_CXX_CLANG_TIDY clang-tidy)
  endif()
endif()

if((IS_ROOT_PROJECT OR TINYMT_CPP_BUILD_TESTING) AND BUILD_TESTING)
  add_subdirectory(tests)
endif()

if(IS_ROOT_PROJECT AND BUILD_BENCHMARKING)
  add_subdirectory(benches)
endif()

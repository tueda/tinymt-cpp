cmake_minimum_required(VERSION 3.11)
include(FetchContent)

if(IS_ROOT_PROJECT)
  # Define the "bench" target.
  add_custom_target(
    bench
    COMMAND test-bench.exe
    COMMENT "Running benchmark tests")
endif()

FetchContent_Declare(
  nanobench
  GIT_REPOSITORY https://github.com/martinus/nanobench.git
  GIT_TAG e2ecbaa
  GIT_SHALLOW TRUE)
FetchContent_GetProperties(nanobench)
if(NOT nanobench_POPULATED)
  FetchContent_Populate(nanobench)
endif()

function(tinymt_cpp_bench name)
  add_executable(${name}.exe ${name}.cpp)
  target_link_libraries(${name}.exe tinymt)
  target_include_directories(${name}.exe
                             PRIVATE "${nanobench_SOURCE_DIR}/src/include")
  if(IS_ROOT_PROJECT)
    target_link_libraries(${name}.exe ${PROJECT_CONFIG_LIB})
    add_dependencies(bench ${name}.exe)
  endif()
endfunction()

tinymt_cpp_bench(test-bench)

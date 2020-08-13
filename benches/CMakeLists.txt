if(NOT_SUBPROJECT)
  # Define the "bench" target.
  add_custom_target(
    bench
    COMMAND test-bench.exe
    COMMENT "Running benchmark tests")
endif()

include(FetchContent)
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
  if(NOT_SUBPROJECT)
    add_dependencies(bench ${name}.exe)
  endif()
endfunction()

tinymt_cpp_bench(test-bench)

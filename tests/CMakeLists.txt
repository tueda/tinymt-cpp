if(NOT_SUBPROJECT)
  # Define the "check" target. Use "make check" instead of "make test"; the
  # latter is known not to update executables (cmake/cmake#8774).
  add_custom_target(
    check
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    COMMENT "Running tests")
endif()

include(FetchContent)
FetchContent_Declare(
  doctest
  GIT_REPOSITORY https://github.com/onqtam/doctest.git
  GIT_TAG 2.4.0)
FetchContent_GetProperties(doctest)
if(NOT doctest_POPULATED)
  FetchContent_Populate(doctest)
  add_subdirectory(${doctest_SOURCE_DIR} ${doctest_BINARY_DIR})
endif()

function(tinymt_cpp_test name)
  add_executable(${name}.exe ${name}.cpp)
  target_link_libraries(${name}.exe tinymt)
  target_link_libraries(${name}.exe doctest)
  if(NOT_SUBPROJECT)
    target_link_libraries(${name}.exe coverage_config)
    add_dependencies(check ${name}.exe)
  endif()
  add_test(NAME ${name} COMMAND ${name}.exe)
endfunction()

tinymt_cpp_test(test-include)
tinymt_cpp_test(test-expand)
tinymt_cpp_test(test-rfc8682)
tinymt_cpp_test(test-tinymt32)
tinymt_cpp_test(test-tinymt32_dc)

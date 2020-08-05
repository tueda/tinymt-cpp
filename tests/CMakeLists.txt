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

macro(tinymt_cpp_test NAME)
  add_executable(${NAME}.exe ${NAME}.cpp)
  target_link_libraries(${NAME}.exe tinymt)
  target_link_libraries(${NAME}.exe doctest)
  add_test(NAME ${NAME} COMMAND ${NAME}.exe)
endmacro()

tinymt_cpp_test(test-include)
tinymt_cpp_test(test-rfc8682)

add_executable(test-nothing.exe test-nothing.cpp)
target_link_libraries(test-nothing.exe tinymt)
add_test(NAME test-nothing COMMAND test-nothing.exe)

add_executable(test-tinymt32.exe test-tinymt32.cpp)
target_link_libraries(test-tinymt32.exe tinymt)
add_test(NAME test-tinymt32 COMMAND test-tinymt32.exe)

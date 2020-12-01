set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/day1_2/bin)
add_subdirectory(src)
add_subdirectory(input)
add_executable(day1_2 src/main.cpp)
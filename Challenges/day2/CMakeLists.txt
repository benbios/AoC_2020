set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/day2/bin)

add_subdirectory(src)
add_subdirectory(input)
add_executable(day2 src/day2.cpp)
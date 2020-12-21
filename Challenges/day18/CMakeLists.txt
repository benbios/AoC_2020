get_filename_component(dirname ${CMAKE_CURRENT_SOURCE_DIR} NAME)
message ("DIRNAME " ${dirname})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${dirname}/bin)

add_subdirectory(input)
add_executable(${dirname} src/main.cpp)
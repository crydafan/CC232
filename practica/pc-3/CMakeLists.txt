add_executable(pc3_main src/main.cpp)
target_include_directories(pc3_main PRIVATE include)

add_subdirectory(demos)
add_subdirectory(tests)

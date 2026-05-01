add_library(solution src/solution.cpp)
target_include_directories(solution PUBLIC include)

add_executable(main src/main.cpp)
target_link_libraries(main PRIVATE solution)

add_subdirectory(tests)

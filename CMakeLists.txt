PROJECT(main)

cmake_minimum_required(VERSION 3.9)
add_executable(demo main.cpp)
target_link_libraries(demo curl)

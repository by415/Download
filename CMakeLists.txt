PROJECT(main)

cmake_minimum_required(VERSION 3.9)
add_subdirectory(log)
add_executable(demo main.cpp download.cpp)
target_link_libraries(demo log curl pthread)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g")

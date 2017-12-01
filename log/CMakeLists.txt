aux_source_directory(. DIR_LIB_SRCS)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
add_library(log SHARED ${DIR_LIB_SRCS})

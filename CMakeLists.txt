cmake_minimum_required(VERSION 3.8)
project(lbp_matcher)

set(CMAKE_CXX_STANDARD 14)


include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/src)


add_subdirectory(src)

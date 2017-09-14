cmake_minimum_required(VERSION 3.7)
project(lbp_matcher)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp ImageHash.cpp ImageHash.h AverageHash.cpp AverageHash.h DifferenceHash.cpp DifferenceHash.h PerceptualHash.cpp PerceptualHash.h HistogramComparison.cpp HistogramComparison.h types.h)
add_executable(lbp_matcher ${SOURCE_FILES})
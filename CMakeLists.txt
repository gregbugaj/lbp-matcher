cmake_minimum_required(VERSION 3.7)
project(lbp_matcher)

set(CMAKE_CXX_STANDARD 14)

SET(GCC_CUSTOM_COMPILE_FLAGS "-fverbose-asm")
SET(GCC_CUSTOM_LINK_FLAGS    "")


SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_CUSTOM_COMPILE_FLAGS}" )
SET( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_CUSTOM_LINK_FLAGS}" )


set(SOURCE_FILES main.cpp ImageHash.cpp ImageHash.h AverageHash.cpp AverageHash.h DifferenceHash.cpp DifferenceHash.h PerceptualHash.cpp PerceptualHash.h HistogramComparison.cpp HistogramComparison.h types.h)
add_executable(lbp_matcher ${SOURCE_FILES})
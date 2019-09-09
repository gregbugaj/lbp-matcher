cmake_minimum_required(VERSION 3.8)
project(lbp_matcher)

set(CMAKE_CXX_STANDARD 17)

SET(GCC_CUSTOM_COMPILE_FLAGS "-fverbose-asm -std=gnu++17  -mpopcnt")
SET(GCC_CUSTOM_LINK_FLAGS    "-lstdc++fs")

SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_CUSTOM_COMPILE_FLAGS}" )
SET( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_CUSTOM_LINK_FLAGS}" )

ADD_LIBRARY(lept STATIC IMPORTED)
SET_PROPERTY(TARGET lept PROPERTY IMPORTED_LOCATION /usr/local/lib/liblept.so)
# PREDATOR
#SET_PROPERTY(TARGET lept PROPERTY IMPORTED_LOCATION /usr/local/lib/libleptonica.so)


set(SOURCE_FILES main.cpp ImageHash.cpp ImageHash.h AverageHash.cpp AverageHash.h DifferenceHash.cpp DifferenceHash.h PerceptualHash.cpp PerceptualHash.h HistogramComparison.cpp HistogramComparison.h types.h HashDistance.cpp HashDistance.h leptonutil.cpp leptonutil.h ImageSimilarity.cpp ImageSimilarity.h fileutil.cpp fileutil.h LBPMatcher.cpp LBPMatcher.h low.cpp low.h Segmenter.cpp Segmenter.h Extractor.cpp Extractor.h Clustering.cpp Clustering.h catch.hpp TestCase.cpp heatmap.h heatmap.cpp LbpGenerator.cpp LbpGenerator.hpp)
add_executable(lbp_matcher ${SOURCE_FILES})

TARGET_LINK_LIBRARIES(lbp_matcher lept stdc++fs)

# copy test-deck files
message("++ EnvParameter : CMAKE_BINARY_DIR     = ${CMAKE_BINARY_DIR}")

file(GLOB MY_TESTDECK_FILES "${CMAKE_SOURCE_DIR}/test-deck")
file(COPY ${MY_TESTDECK_FILES} DESTINATION "${CMAKE_BINARY_DIR}/")
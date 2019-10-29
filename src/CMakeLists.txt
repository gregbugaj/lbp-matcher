cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

project(lbp_matcher)
set(TARGET_NAME "lbp_matcher")

set(CMAKE_CXX_STANDARD 17)

SET(GCC_CUSTOM_COMPILE_FLAGS "-fverbose-asm -std=gnu++17  -mpopcnt")
SET(GCC_CUSTOM_LINK_FLAGS    "-lstdc++fs")

SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_CUSTOM_COMPILE_FLAGS}" )
SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_CUSTOM_LINK_FLAGS}" )

# Set a default build type if none was specified
if(NOT CMAKE_BUILD_TYPE)
    message(STATUS "Setting build type to 'Release' as none was specified.")
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
endif()

message("EnvParameter : CMAKE_BUILD_TYPE          = ${CMAKE_BUILD_TYPE}")
message("EnvParameter : CMAKE_SOURCE_DIR          = ${CMAKE_SOURCE_DIR}")
message("EnvParameter : CMAKE_CURRENT_SOURCE_DIR  = ${CMAKE_CURRENT_SOURCE_DIR}")
message("EnvParameter : CMAKE_BINARY_DIR          = ${CMAKE_BINARY_DIR}")
message("EnvParameter : CMAKE_CURRENT_BINARY_DIR  = ${CMAKE_CURRENT_BINARY_DIR}")
message("EnvParameter : CMAKE_INSTALL_PREFIX      = ${CMAKE_INSTALL_PREFIX}")

###########################################################
# Where are the additional libraries installed? Note: provide includes
# path here, subsequent checks will resolve everything else
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/modules")

find_program(GIT "git")
if (GIT)
    execute_process(
            COMMAND git log -1 --format=%H
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            OUTPUT_VARIABLE GIT_COMMIT_HASH
            OUTPUT_STRIP_TRAILING_WHITESPACE
    )
else ()
    set(GIT_COMMIT_HASH "unknown")
endif ()

configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/version.h.in
        ${CMAKE_CURRENT_BINARY_DIR}/generated/version.h
)

# Source files
set(SOURCE_FILES main.cpp ImageHash.cpp ImageHash.h AverageHash.cpp AverageHash.h DifferenceHash.cpp DifferenceHash.h PerceptualHash.cpp PerceptualHash.h HistogramComparison.cpp HistogramComparison.h types.h HashDistance.cpp HashDistance.h leptonutil.cpp leptonutil.h ImageSimilarity.cpp ImageSimilarity.h fileutil.cpp fileutil.h LBPMatcher.cpp LBPMatcher.h low.cpp low.h Segmenter.cpp Segmenter.h Extractor.cpp Extractor.h Clustering.cpp Clustering.h catch.hpp TestCase.cpp heatmap.h heatmap.cpp LbpGenerator.hpp generators/lbp/Lbp.cpp generators/lbp/Lbp.hpp generators/elbp/Elbp.cpp generators/elbp/Elbp.hpp generators/slbp/Slbp.cpp generators/slbp/Slbp.hpp generators/ilbp/Ilbp.cpp generators/ilbp/Ilbp.hpp Lenet.hpp utils.hpp)

# Build as executable
add_executable(${TARGET_NAME} ${SOURCE_FILES})

### Leptonica
include(FindLeptonica)

if (LEPTONICA_FOUND)
    message(STATUS "Leptonica ready")
    include_directories(${LEPTONICA_INCLUDE_DIRS})
    target_link_libraries(${TARGET_NAME} ${LEPTONICA_LIBRARIES})
else ()
    message(FATAL_ERROR "libleptonica-dev is required install with 'apt-get install libleptonica-dev'")
endif(LEPTONICA_FOUND)
### Leptonica

# MxNet
include(FindMxNet)

if (MxNet_FOUND)
    message(STATUS "MxNet ready")
    include_directories(${MxNet_INCLUDE_DIRS})
    target_link_libraries(${TARGET_NAME} ${MxNet_LIB})
else ()
    message(FATAL_ERROR "MXNET not ready, compile from source with 'USE_CPP_PACKAGE=1' ")
endif(MxNet_FOUND)

# Generated
include_directories(${CMAKE_CURRENT_BINARY_DIR}/generated)

#ADD_LIBRARY(lept STATIC IMPORTED)
#SET_PROPERTY(TARGET lept PROPERTY IMPORTED_LOCATION /usr/local/lib/liblept.so)

# PREDATOR
#SET_PROPERTY(TARGET lept PROPERTY IMPORTED_LOCATION /usr/local/lib/libleptonica.so)

# copy test-deck files
message("++ EnvParameter : CMAKE_BINARY_DIR     = ${CMAKE_BINARY_DIR}")

file(GLOB MY_TESTDECK_FILES "${CMAKE_SOURCE_DIR}/test-deck")
file(COPY ${MY_TESTDECK_FILES} DESTINATION "${CMAKE_BINARY_DIR}/")

# Filesystem support
target_link_libraries(${TARGET_NAME} stdc++fs)
# install(TARGETS lbp-matcher DESTINATION bin)
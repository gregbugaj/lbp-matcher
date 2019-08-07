#!/bin/sh

#rm -rf build
cd build
cmake -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON  -DCMAKE_BUILD_TYPE=Debug .
make  -j8 && make install

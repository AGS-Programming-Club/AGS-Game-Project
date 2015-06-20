#!/bin/bash

pushd `dirname $0` > /dev/null
SCRIPT_PATH=`pwd -P`
popd > /dev/null

CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE:-Debug}
BUILD_UNIT_TESTS=${BUILD_UNIT_TESTS:-ON}

pushd "${SCRIPT_PATH}/.." > /dev/null
mkdir build
cd build &&
cmake -G"Unix Makefiles" \
      -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} \
      -DBUILD_UNIT_TESTS=${BUILD_UNIT_TESTS} \
      .. &&
make -j4
popd > /dev/null

#!/bin/bash

pushd `dirname $0` > /dev/null
SCRIPT_PATH=`pwd -P`
popd > /dev/null

pushd "${SCRIPT_PATH}/.." > /dev/null
mkdir build
cd build &&
cmake -G"Unix Makefiles" \
      -DCMAKE_BUILD_TYPE=Debug \
      -DBUILD_UNIT_TESTS=ON \
      .. &&
make -j4
popd > /dev/null

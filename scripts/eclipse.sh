#!/bin/bash

pushd `dirname $0` > /dev/null
SCRIPT_PATH=`pwd -P`
popd > /dev/null

CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE:-Debug}
BUILD_UNIT_TESTS=${BUILD_UNIT_TESTS:-ON}
CMAKE_ECLIPSE_VERSION=${CMAKE_ECLIPSE_VERSION:-4.4}

pushd "${SCRIPT_PATH}/.." > /dev/null
cmake -G"Eclipse CDT4 - Unix Makefiles" \
      -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} \
      -DBUILD_UNIT_TESTS=${BUILD_UNIT_TESTS} \
      -DCMAKE_ECLIPSE_VERSION=${CMAKE_ECLIPSE_VERSION} \
      .
popd > /dev/null

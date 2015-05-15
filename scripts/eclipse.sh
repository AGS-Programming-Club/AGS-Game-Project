#!/bin/bash

pushd `dirname $0` > /dev/null
SCRIPT_PATH=`pwd -P`
popd > /dev/null

pushd "${SCRIPT_PATH}/.." > /dev/null
cmake -G"Eclipse CDT4 - Unix Makefiles" \
      -DCMAKE_BUILD_TYPE=Debug \
      -DBUILD_UNIT_TESTS=ON \
      -DCMAKE_ECLIPSE_VERSION=4.4 \
      .
popd > /dev/null

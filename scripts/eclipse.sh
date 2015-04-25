#!/bin/bash

pushd `dirname $0` > /dev/null
SCRIPT_PATH=`pwd -P`
popd > /dev/null

pushd ${SCRIPT_PATH}/.. > /dev/null
cmake -G"Eclipse CDT4 - Unix Makefiles" \
      -DCMAKE_BUILD_TYPE=Debug \
      -DCMAKE_ECLIPSE_VERSION=4.4 \
      . &&
if [ -d ../.metadata ]; then
    mkdir -p ../.metadata/.plugins/org.eclipse.debug.core/.launches
    cp -f ${SCRIPT_PATH}/client.launch \
          ../.metadata/.plugins/org.eclipse.debug.core/.launches/client.launch
fi
popd > /dev/null

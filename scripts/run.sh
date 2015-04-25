#!/bin/bash

pushd `dirname $0` > /dev/null
SCRIPT_PATH=`pwd -P`
popd > /dev/null

pushd ${SCRIPT_PATH}/.. > /dev/null
cd ./build/bin/client
./client
popd > /dev/null

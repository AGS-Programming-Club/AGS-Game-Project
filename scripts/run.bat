@echo off

set SCRIPT_PATH=%~dp0

pushd "%SCRIPT_PATH%"
cd build\bin\client
client
popd

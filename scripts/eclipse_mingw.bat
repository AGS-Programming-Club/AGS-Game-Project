@echo off

set SCRIPT_PATH=%~dp0

pushd "%SCRIPT_PATH%\.."
cmake -G"Eclipse CDT4 - MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug -DBUILD_UNIT_TESTS=ON -DCMAKE_ECLIPSE_VERSION=4.4 . || goto :error
:error
popd

pause

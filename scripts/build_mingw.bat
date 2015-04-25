@echo off

set SCRIPT_PATH=%~dp0

pushd "%SCRIPT_PATH%"
mkdir build
cd build &&
cmake -G"MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug .. &&
mingw32-make -j4
popd

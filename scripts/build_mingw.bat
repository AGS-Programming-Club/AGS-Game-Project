@echo off

set SCRIPT_PATH=%~dp0

pushd "%SCRIPT_PATH%\.."
mkdir build
cd build || goto :error
cmake -G"MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug .. || goto :error
mingw32-make -j4 || goto :error
:error
popd

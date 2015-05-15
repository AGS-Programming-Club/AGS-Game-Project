@echo off

set SCRIPT_PATH=%~dp0

pushd "%SCRIPT_PATH%\.."
cd build
ctest --output-on-failure
popd

pause

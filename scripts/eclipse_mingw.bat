@echo off

set SCRIPT_PATH=%~dp0

pushd "%SCRIPT_PATH%\.."
cmake -G"Eclipse CDT4 - MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug -DCMAKE_ECLIPSE_VERSION=4.4 . || goto :error
::if exist ..\.metadata (
::    if not exist ..\.metadata\.plugins\org.eclipse.debug.core\.launches (
::        md ..\.metadata\.plugins\org.eclipse.debug.core\.launches
::    )
::    copy "%SCRIPT_PATH%/client.launch" ..\.metadata\.plugins\org.eclipse.debug.core\.launches\client.launch
::)
:error
popd

pause

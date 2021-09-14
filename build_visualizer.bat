set BUILD_DIR=build
set CMAKE_COMMAND=cmake . -A Win32 -B %BUILD_DIR%
echo %CMAKE_COMMAND%
call %CMAKE_COMMAND%

echo Press any key to open visualizer.sln or close this shell...
pause

start %BUILD_DIR%\visualizer.sln
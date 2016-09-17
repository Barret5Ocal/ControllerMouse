@echo off

IF NOT EXIST ..\..\build mkdir ..\..\build
pushd ..\..\build
cl -Zi ..\Project\code\win32_xboxonemouse.cpp user32.lib gdi32.lib winmm.lib
popd

@echo off
setlocal
pushd "%~dp0"

rem Init environments
set GAME_CONFIGURATOR_LOCATION=%1
set PACKAGE_LOCATION=%2
set GAME_CONFIGURATOR=%GAME_CONFIGURATOR_LOCATION%\\build\\release\\GameConfigurator.exe

rem Go to game location
cd %GAME_CONFIGURATOR_LOCATION%

rem Generate makefile
qmake6 -makefile

rem Build
make -j4

rem Copy GameConfigurator to package
echo Copy GameConfigurator to package
copy "%GAME_CONFIGURATOR%" "%PACKAGE_LOCATION%"

rem Deploy configurator
echo Deploy configurator
windeployqt --release --qmldir %GAME_CONFIGURATOR_LOCATION%\\qml --libdir %PACKAGE_LOCATION%\\Libraries  --plugindir %PACKAGE_LOCATION%\\Plugins  %GAME_CONFIGURATOR%

popd

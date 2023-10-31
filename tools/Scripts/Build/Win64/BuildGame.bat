@echo off
setlocal
pushd "%~dp0"

set GAME_LOCATION=%1
set PACKAGE_LOCATION=%2
set GAME_FILE=%GAME_LOCATION%\\target\\wasm32-unknown-unknown\\release\\ChibiGame.wasm 
set ASSETS_DIR=%PACKAGE_LOCATION%\\assets
set CONFIG_DIR=%PACKAGE_LOCATION%\\config

cd %GAME_LOCATION%

rem Build ChibiGame
cargo build --target wasm32-unknown-unknown --release

rem Copy game
echo Copy game %GAME_FILE% to %PACKAGE_LOCATION%
copy "%GAME_FILE%" "%PACKAGE_LOCATION%"

if not exist "%ASSETS_DIR%" (
    mkdir "%ASSETS_DIR%"
)

if not exist "%CONFIG_DIR%" (
    mkdir "%CONFIG_DIR%"
)

xcopy "%GAME_LOCATION%\\assets\\*" "%ASSETS_DIR%" /E

popd

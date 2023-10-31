@echo off
setlocal
pushd "%~dp0"

set GAME_LOCATION=%~dp0Game\\ChibiGame.wasm
set WASM_RUNNER_LOCATION=%~dp0Tools\\wasm-server-runner.exe

cd %~dp0Game

%WASM_RUNNER_LOCATION% %GAME_LOCATION%

popd

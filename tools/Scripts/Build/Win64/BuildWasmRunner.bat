@echo off
setlocal
pushd "%~dp0"

set WASM_RUNNER_LOCATION=%1
set PACKAGE_LOCATION=%2
set WASM_SERVER_RUNNER=%WASM_RUNNER_LOCATION%\\target\\release\\wasm-server-runner.exe

cd %WASM_RUNNER_LOCATION%

rem Build wasm runner
cargo build --release

rem Coppy runner
echo Coppy runner %WASM_SERVER_RUNNER% to %PACKAGE_LOCATION%
copy "%WASM_SERVER_RUNNER%" "%PACKAGE_LOCATION%"

popd

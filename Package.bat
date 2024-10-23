@echo off
setlocal
pushd "%~dp0"

rem Environments
set PROJECT_LOCATION=%~dp0
set WASM_SERVER_RUNNER=%~dp0tools/wasm-server-runner
set GAME_CONFIGURATOR=%~dp0GameConfigurator
set GAME=%~dp0ChibiGame
set TWITCH_BOT=%~dp0TwitchBot

rem Install submodule...
echo Setup submodules...
git submodule update --init --recursive

rem Setup Package folder
echo Setup Package folder
set PACKAGE_DIRECTORY=%~dp0ChibiChatBotGame
set PACKAGE_GAME_DIRECTORY=%PACKAGE_DIRECTORY%/Game
set PACKAGE_TOOLS_DIRECTORY=%PACKAGE_DIRECTORY%/Tools

if not exist %PACKAGE_DIRECTORY% (
    mkdir "%PACKAGE_DIRECTORY%"
)

if not exist %PACKAGE_GAME_DIRECTORY% (
    mkdir "%PACKAGE_GAME_DIRECTORY%"
)

if not exist %PACKAGE_TOOLS_DIRECTORY% (
    mkdir "%PACKAGE_TOOLS_DIRECTORY%"
)

rem Setup requirements
echo "Install rust requirements"
rustup target add wasm32-unknown-unknown

rem Build twitch bot
echo Build twitch bot
call "%~dp0tools/Scripts/Build/Win64/BuildTwitchBot.bat" %TWITCH_BOT% %PACKAGE_DIRECTORY%

rem Build chibi game
echo Build chibi game
call "%~dp0tools/Scripts/Build/Win64/BuildGame.bat" %GAME% %PACKAGE_GAME_DIRECTORY%

rem Build Game configurator
echo Build Game configurator
call "%~dp0tools/Scripts/Build/Win64/BuildGameConfigurator.bat" %GAME_CONFIGURATOR% %PACKAGE_DIRECTORY%

rem Build WASM runner
echo Build WASM runner
call "%~dp0tools/Scripts/Build/Win64/BuildWasmRunner.bat" %WASM_SERVER_RUNNER% %PACKAGE_TOOLS_DIRECTORY%

rem Copy runners
echo Copy runners
copy %~dp0tools\\Scripts\\Runners\\Win64\\RunChatBot.bat %PACKAGE_DIRECTORY%
copy %~dp0tools\\Scripts\\Runners\\Win64\\RunGame.bat %PACKAGE_DIRECTORY%

rem Done!
goto :end

rem Error happened. Wait for a keypress before quitting.
:error
pause

:end
popd

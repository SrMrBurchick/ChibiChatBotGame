@echo off
setlocal
pushd "%~dp0"

rem Init environments
set TWITCH_BOT_LOCATION=%1
set PACKAGE_LOCATION=%2

set TWITCH_BOT=%CHAT_BOT_LOCATION%\\target\\build\\release\\TwitchBot.exe

rem Go to chat bot location
cd %TWITCH_BOT_LOCATION%

rem Build
cargo build --release

rem Copy chat bot to package
echo Copy twitch bot %TWITCH_BOT% to %PACKAGE_LOCATION%

copy "%TWITCH_BOT%" "%PACKAGE_LOCATION%"

popd

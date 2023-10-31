@echo off
setlocal
pushd "%~dp0"

rem Init environments
set CHAT_BOT_LOCATION=%1
set PACKAGE_LOCATION=%2

set CHAT_BOT=%CHAT_BOT_LOCATION%\\dist\\chat_bot.exe

rem Go to chat bot location
cd %CHAT_BOT_LOCATION%

rem Build
pyinstaller --onefile --exclude ./Game/config/ChibiChatBotConfig.json chat_bot.py

rem Copy chat bot to package
echo Copy chat bot %CHAT_BOT% to %PACKAGE_LOCATION%

copy "%CHAT_BOT%" "%PACKAGE_LOCATION%"

popd

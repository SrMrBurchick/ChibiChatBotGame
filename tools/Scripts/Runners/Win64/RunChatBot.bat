@echo off
setlocal
pushd "%~dp0"

set CHAT_BOT=%~dp0chat_bot.exe

cd %~dp0

%CHAT_BOT%

popd

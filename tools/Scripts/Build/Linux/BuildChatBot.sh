#!/bin/bash

# Init environments
CHAT_BOT_LOCATION=$1
PACKAGE_LOCATION=$2

# Go to chat bot location
cd ${CHAT_BOT_LOCATION}

## Build
pyinstaller --onefile --exclude ./Game/config/ChibiChatBotConfig.json chat_bot.py

## Copy chat bot to package
cp ${CHAT_BOT_LOCATION}/dist/chat_bot ${PACKAGE_LOCATION}

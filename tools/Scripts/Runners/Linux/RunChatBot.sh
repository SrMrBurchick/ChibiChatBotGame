#!/bin/bash

CURRENT_LOCATION="$PWD"
CHAT_BOT="{$CURRENT_LOCATION}/chat_bot/chat_bot"

if [ ! -f ${CHAT_BOT} ]; then
    echo "Chat bot server not configured"
    exit 1
fi

# Run chat bot
${CHAT_BOT}

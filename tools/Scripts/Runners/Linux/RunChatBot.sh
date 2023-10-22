#!/bin/bash

CURRENT_LOCATION="$PWD"
CHAT_BOT="chat_bot"

if [ ! -f ${CHAT_BOT} ]; then
    echo "Chato bot server not configured"
    exit 1
fi

# Run chat bot
${CHAT_BOT} &

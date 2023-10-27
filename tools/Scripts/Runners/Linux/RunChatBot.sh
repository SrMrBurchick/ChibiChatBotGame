#!/bin/bash

CURRENT_LOCATION="$PWD"
CHAT_BOT="{$CURRENT_LOCATION}/chat_bot/chat_bot"

if [ ! -f ${CHAT_BOT} ]; then
    echo "Chat bot server not configured"
    exit 1
fi

CHAT_BOT_ID=0

# Trap kill signal
term() {
    notify-send "Kill chat bot($CHAT_BOT_ID)"
    if [ "$CHAT_BOT_ID" -ne 0 ]; then
        kill -9 $CHAT_BOT_ID
    fi

    exit 0
}

trap term SIGTERM

# Run chat bot
${CHAT_BOT} &
CHAT_BOT_ID=$!

while true; do
    sleep 5
done

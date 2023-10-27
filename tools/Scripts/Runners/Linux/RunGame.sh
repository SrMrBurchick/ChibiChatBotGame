#!/bin/bash

CURRENT_LOCATION="$PWD"
GAME_LOCATION="${CURRENT_LOCATION}/Game"
WASM_SERVER_RUNNER_LOCATION="${CURRENT_LOCATION}/Tools/wasm-server-runner"
GAME="ChibiGame.wasm"

if [ ! -f ${WASM_SERVER_RUNNER_LOCATION} ]; then
    echo "WASM server runner not configured"
    exit 1
fi

# Go to game location
cd ${GAME_LOCATION}

BROWSER_ID=0
GAME_ID=0

# Trap kill signal
term() {
    notify-send "Kill subprocesses Browser($BROWSER_ID) and Game($GAME_ID)"
    if [ "$BROWSER_ID" -ne 0 ]; then
        kill -9 $BROWSER_ID
    fi

    if [ "$GAME_ID" -ne 0 ]; then
        kill -9 $GAME_ID
    fi

    exit 0
}

trap term SIGTERM

# Run game
chromium "http://127.0.0.1:1334" &
BROWSER_ID=$!

$WASM_SERVER_RUNNER_LOCATION ${GAME} < /dev/null > /dev/null 2>&1 &
GAME_ID=$!

while true; do
    sleep 5
done

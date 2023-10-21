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

# Run game
$WASM_SERVER_RUNNER_LOCATION ${GAME}

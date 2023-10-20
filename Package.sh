#!/bin/bash
## Init submodules
git submodule update --init --recursive

## Environment variables
PROJECT_LOCATION="$PWD"
WASM_SERVER_RUNNER="$PWD/tools/wasm-server-runner"
GAME_CONFIGURATOR="$PWD/GameConfigurator"
GAME="$PWD/ChibiGame"
CHAT_BOT="$PWD/ChatBot"


## Create ChibiChatBotGame directory
PACKAGE_DIRECTORY="$PWD/ChibiChatBotGame"
PACKAGE_TOOLS_DIRECTORY="$PACKAGE_DIRECTORY/Tools"
if [ ! -d ${PACKAGE_DIRECTORY} ]; then
    echo "Create ChibiChatBotGame package directory"
    mkdir ${PACKAGE_DIRECTORY}
    if [ ! -d ${PACKAGE_TOOLS_DIRECTORY} ]; then
        mkdir ${PACKAGE_TOOLS_DIRECTORY}
    fi
fi

########################## Setup wasm-server-runner ############################
cd ${WASM_SERVER_RUNNER}
cargo build --release

### Go back
cd ${PROJECT_LOCATION}

### Copy binary to the package folder
cp ${WASM_SERVER_RUNNER}/target/release/wasm-server-runner ${PACKAGE_TOOLS_DIRECTORY}

######################### Build GameConfigurator ###############################
cd ${GAME_CONFIGURATOR}
### Generate makefile
qmake6 -makefile

### Build
make release

### Copy GameConfigurator to package
cp ${GAME_CONFIGURATOR}/build/release/GameConfigurator ${PACKAGE_DIRECTORY}

######################### Build Game ###########################################
cd ${GAME}

### Build
cargo build --target wasm32-unknown-unknown --release

### Copy game to package
cp ${GAME}/target/wasm32-unknown-unknown/release/ChibiGame.wasm ${PACKAGE_DIRECTORY}

######################### Build Chat bot #######################################
cd ${CHAT_BOT}

## Build
pyinstaller chat_bot.py

## Copy chat bot to package
cp ${CHAT_BOT}/build/chat_bot/chat_bot ${PACKAGE_DIRECTORY}

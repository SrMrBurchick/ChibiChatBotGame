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
PACKAGE_GAME_DIRECTORY="$PACKAGE_DIRECTORY/Game"
PACKAGE_TOOLS_DIRECTORY="$PACKAGE_DIRECTORY/Tools"

if [ ! -d ${PACKAGE_DIRECTORY} ]; then
    echo "Create ChibiChatBotGame package directory"
    mkdir ${PACKAGE_DIRECTORY}
    mkdir ${PACKAGE_TOOLS_DIRECTORY}
    mkdir ${PACKAGE_GAME_DIRECTORY}
    mkdir ${PACKAGE_GAME_DIRECTORY}/assets
    mkdir ${PACKAGE_GAME_DIRECTORY}/config
fi

########################## Setup wasm-server-runner ############################
${PROJECT_LOCATION}/tools/Scripts/Build/Linux/BuildWasmRunner.sh ${WASM_SERVER_RUNNER} ${PACKAGE_TOOLS_DIRECTORY}

######################### Build GameConfigurator ###############################
${PROJECT_LOCATION}/tools/Scripts/Build/Linux/BuildGameConfigurator.sh ${GAME_CONFIGURATOR} ${PACKAGE_DIRECTORY}

######################### Build Game ###########################################
${PROJECT_LOCATION}/tools/Scripts/Build/Linux/BuildGame.sh ${GAME} ${PACKAGE_GAME_DIRECTORY}

## Copy game runner script to package directory
cp ${PROJECT_LOCATION}/tools/Scripts/Runners/Linux/RunGame.sh ${PACKAGE_DIRECTORY}
cp ${PROJECT_LOCATION}/tools/Scripts/Runners/Linux/RunChatBot.sh ${PACKAGE_DIRECTORY}

######################### Build Chat bot #######################################
${PROJECT_LOCATION}/tools/Scripts/Build/Linux/BuildChatBot.sh ${CHAT_BOT} ${PACKAGE_DIRECTORY}

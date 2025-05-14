#!/bin/bash
## Init submodules
git submodule update --init --recursive

# Check for input argument for platform
if [ "$#" -ne 1 ]; then
    echo "Usage: $0 {1|2}"
    echo "1 - Linux"
    echo "2 - Windows"
    read PLATFORM_ID
fi

case $PLATFORM_ID in
    1)
        PLATFORM="Linux"
        ;;
    2)
        PLATFORM="Windows"
        ;;
    *)
        PLATFORM="Linux"
        ;;
esac

echo "Packaging project for $PLATFORM"

## Environment variables
PROJECT_LOCATION="$PWD"
WASM_SERVER_RUNNER="$PWD/tools/wasm-server-runner"
GAME_CONFIGURATOR="$PWD/GameConfigurator"
GAME="$PWD/ChibiGame"
TWITCH_BOT="$PWD/TwitchBot"

## Create ChibiChatBotGame directory
PACKAGE_DIRECTORY="$PWD/Packaged/$PLATFORM/ChibiTwitchBotGame"
PACKAGE_GAME_DIRECTORY="$PACKAGE_DIRECTORY/Game"
PACKAGE_TOOLS_DIRECTORY="$PACKAGE_DIRECTORY/Tools"

if [ ! -d ${PACKAGE_DIRECTORY} ]; then
    echo "Create ChibiChatBotGame package directory"
    mkdir -p ${PACKAGE_DIRECTORY}
    mkdir -p ${PACKAGE_TOOLS_DIRECTORY}
    mkdir -p ${PACKAGE_GAME_DIRECTORY}
    mkdir -p ${PACKAGE_GAME_DIRECTORY}/assets
    mkdir -p ${PACKAGE_GAME_DIRECTORY}/config
fi

########################## Setup wasm-server-runner ############################
${PROJECT_LOCATION}/tools/Scripts/Build/Linux/BuildWasmRunner.sh ${WASM_SERVER_RUNNER} ${PACKAGE_TOOLS_DIRECTORY} ${PLATFORM}

######################### Build GameConfigurator ###############################
${PROJECT_LOCATION}/tools/Scripts/Build/Linux/BuildGameConfigurator.sh ${GAME_CONFIGURATOR} ${PACKAGE_DIRECTORY} ${PLATFORM}

######################### Build Game ###########################################
${PROJECT_LOCATION}/tools/Scripts/Build/Linux/BuildGame.sh ${GAME} ${PACKAGE_GAME_DIRECTORY}

######################### Build Twitch bot #####################################
${PROJECT_LOCATION}/tools/Scripts/Build/Linux/BuildTwitchBot.sh ${TWITCH_BOT} ${PACKAGE_DIRECTORY} ${PLATFORM}

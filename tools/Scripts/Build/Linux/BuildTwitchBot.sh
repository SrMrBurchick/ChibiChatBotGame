#!/bin/bash

# Init environments
TWITCH_BOT_LOCATION=$1
PACKAGE_LOCATION=$2
PLATFORM=$3

# Go to chat bot location
cd ${TWITCH_BOT_LOCATION}

## Build
case $PLATFORM in
    Windows)
        cargo build --release --target x86_64-pc-windows-gnu

        ### Copy binary to the package folder
        cp ${TWITCH_BOT_LOCATION}/target/x86_64-pc-windows-gnu/release/TwitchBot.exe ${PACKAGE_LOCATION}
        ;;
    *)
        cargo build --release

        ### Copy binary to the package folder
        cp ${TWITCH_BOT_LOCATION}/target/release/TwitchBot ${PACKAGE_LOCATION}
        ;;
esac

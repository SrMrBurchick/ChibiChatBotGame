#!/bin/bash

# Init environments
TWITCH_BOT_LOCATION=$1
PACKAGE_LOCATION=$2

# Go to chat bot location
cd ${TWITCH_BOT_LOCATION}

## Build
cargo build --release

## Copy chat bot to package
cp ${TWITCH_BOT_LOCATION}/target/release/TwitchBot ${PACKAGE_LOCATION}


#!/bin/bash

# Init environments
GAME_LOCATION=$1
PACKAGE_LOCATION=$2

# Go to game location
cd ${GAME_LOCATION}

### Build
# cargo build --target wasm32-unknown-unknown
cargo build --target wasm32-unknown-unknown --release

### Copy game to package
cp ${GAME_LOCATION}/target/wasm32-unknown-unknown/release/ChibiGame.wasm ${PACKAGE_LOCATION}
# cp ${GAME_LOCATION}/target/wasm32-unknown-unknown/debug/ChibiGame.wasm ${PACKAGE_LOCATION}
cp -r ${GAME_LOCATION}/assets/fonts ${PACKAGE_LOCATION}/assets

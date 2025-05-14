#!/bin/bash

# Init environments
WASM_RUNNER_LOCATION=$1
PACKAGE_LOCATION=$2
PLATFORM=$3

### Go wasm server runner
cd ${WASM_RUNNER_LOCATION}

## Build
case $PLATFORM in
    Windows)
        cargo build --release --target x86_64-pc-windows-gnu

        ### Copy binary to the package folder
        cp ${WASM_RUNNER_LOCATION}/target/x86_64-pc-windows-gnu/release/wasm-server-runner.exe ${PACKAGE_LOCATION}
        ;;
    *)
        cargo build --release

        ### Copy binary to the package folder
        cp ${WASM_RUNNER_LOCATION}/target/release/wasm-server-runner ${PACKAGE_LOCATION}
        ;;
esac

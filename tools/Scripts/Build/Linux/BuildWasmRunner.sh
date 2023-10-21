#!/bin/bash

# Init environments
WASM_RUNNER_LOCATION=$1
PACKAGE_LOCATION=$2

### Go wasm server runner
cd ${WASM_RUNNER_LOCATION}

## Build
cargo build --release

### Copy binary to the package folder
cp ${WASM_RUNNER_LOCATION}/target/release/wasm-server-runner ${PACKAGE_LOCATION}

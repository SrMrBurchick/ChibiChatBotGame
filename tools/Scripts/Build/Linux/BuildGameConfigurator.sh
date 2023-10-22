#!/bin/bash

# Init environments
GAME_CONFIGURATOR_LOCATION=$1
PACKAGE_LOCATION=$2

# Go to game location
cd ${GAME_CONFIGURATOR_LOCATION}

### Generate makefile
qmake6 -makefile

### Build
make

### Copy GameConfigurator to package
cp ${GAME_CONFIGURATOR_LOCATION}/build/release/GameConfigurator ${PACKAGE_LOCATION}

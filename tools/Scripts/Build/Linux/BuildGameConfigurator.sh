#!/bin/bash

# Init environments
GAME_CONFIGURATOR_LOCATION=$1
PACKAGE_LOCATION=$2
PLATFORM=$3

# Go to game location
cd ${GAME_CONFIGURATOR_LOCATION}

### Generate makefile
case $PLATFORM in
    Windows)
        qmake6 -makefile -spec win32-g++ "CONFIG+=release" "QMAKE_CXX=x86_64-w64-mingw32-g++" "QMAKE_CC=x86_64-w64-mingw32-gcc" "QMAKE_LINK=x86_64-w64-mingw32-g++"
        ;;
    *)
        qmake6 -makefile
        ;;
esac

### Build
make

### Copy GameConfigurator to package
cp ${GAME_CONFIGURATOR_LOCATION}/build/release/GameConfigurator ${PACKAGE_LOCATION}

# Twitch ChatBotGame
A simple chibi like game that provides possibility to controll itself by chat commands.
You can configure custom commands which be represented as unike sprite or sequence of them.

## Build
### Game
First of all make sure that you have preinstalled rust, and wasm32 target for it, and wasm-server-runner.
Go to the *ChibiGame* directory and then execute:
```bash
$ cargo run --target wasm32-unknown-unknown
```
After that you be able to open browser by url that will be provided by wasm-server-runner

### Configurator
Go to the *GameConfigurator* directory
```bash
$ qmake -makefile
```
That will generate a makefile. Than just execute make command
```bash
$ make --release
```
The compiled program should be by path `./GameConfigurator/build/release/GameConfigurator`

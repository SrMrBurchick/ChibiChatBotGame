# Setup Dev Guide
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
$ qmake6 -makefile
```
That will generate a makefile. Than just execute make command
```bash
$ make
```
The compiled program should be by path `./GameConfigurator/build/release/GameConfigurator`

## Win64 setup
- Install [qt](https://www.qt.io/)
- Install [scoop sh](https://scoop.sh/)
- Install make gcc: `scoop install make gcc`
- Install rustup: `scoop install rustup`
- Add target: `rustup target add wasm32-unknown-unknown`
- Install [visual studio](https://visualstudio.microsoft.com/downloads/)

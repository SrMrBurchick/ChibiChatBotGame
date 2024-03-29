######################################################################
# Automatically generated by qmake (3.1) Mon Oct 2 05:57:59 2023
######################################################################

QT += core quick quickcontrols2 gui
CONFIG += c++21
# CONFIG += debug_and_release

TEMPLATE = app
TARGET = GameConfigurator
INCLUDEPATH += ./Sources/Public

HEADERS += Sources/Public/Configuration/ConfigLoader.h                          \
           Sources/Public/Configuration/ConfigObject.h                          \
           Sources/Public/Models/ActionsListModel.h                             \
           Sources/Public/Components/ActionComponent.h                          \
           Sources/Public/Models/AnimationSequenceModel.h                       \
           Sources/Public/Models/SpriteSheet.h                                  \
           Sources/Public/Managers/ProcessManager.h                             \
           Sources/Public/Managers/ProcessInterface.h                           \
           Sources/Public/Managers/Processes/ChatBotProcess.h                   \
           Sources/Public/Managers/Processes/GameProcess.h                      \
           Sources/Public/Models/NotificationModel.h                            \
           Sources/Public/Managers/NotificationsManager.h                       \
           Sources/Public/Models/PredefinedActionsModel.h                       \
           Sources/Public/System/Logger.h

SOURCES += Sources/GameConfigurator.cpp                                         \
           Sources/Private/Configuration/ConfigLoad.cpp                         \
           Sources/Private/Configuration/ConfigObject.cpp                       \
           Sources/Private/Models/ActionsListModel.cpp                          \
           Sources/Private/Models/AnimationSequenceModel.cpp                    \
           Sources/Private/Components/ActionComponent.cpp                       \
           Sources/Private/Models/SpriteSheet.cpp                               \
           Sources/Private/Managers/ProcessManager.cpp                          \
           Sources/Private/Managers/ProcessInterface.cpp                        \
           Sources/Private/Managers/Processes/ChatBotProcess.cpp                \
           Sources/Private/Managers/Processes/GameProcess.cpp                   \
           Sources/Private/Models/NotificationModel.cpp                         \
           Sources/Private/Managers/NotificationsManager.cpp                    \
           Sources/Private/Models/PredefinedActionsList.cpp                     \
           Sources/Private/System/Logger.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH = qml
QML2_IMPORT_PATH = qml

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Executables path
DEFINES += CHAT_BOT_PATH="./chat_bot"
DEFINES += GAME_ASSETS_PATH="./Game/assets"
DEFINES += CONFIG_FILE="./Game/config/ChibiChatBotConfig.json"
DEFINES += GAME_ASSET_IMAGE_PATH="./Game/assets/sprite-sheet.png"
DEFINES += WASM_SERVER_RUNNER="./Tools/wasm-server-runner"
DEFINES += GAME_PATH="./Game"
DEFINES += GAME="./Game/ChibiGame.wasm"

# Check if the target OS is Windows
win32 {
    DEFINES += CHAT_BOT_PATH="./chat_bot.exe"
    DEFINES += WASM_SERVER_RUNNER="./Tools/wasm-server-runner.exe"
}

# Directories
DESTDIR = build/release

CONFIG(debug, debug|release) {
    DESTDIR = build/debug
}

OBJECTS_DIR = $$DESTDIR/.obj
MOC_DIR = $$DESTDIR/.moc
RCC_DIR = $$DESTDIR/.qrc
UI_DIR = $$DESTDIR/.ui

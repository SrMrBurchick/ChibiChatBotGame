######################################################################
# Automatically generated by qmake (3.1) Mon Oct 2 05:57:59 2023
######################################################################

QT += core quick quickcontrols2
CONFIG += c++21
CONFIG += debug_and_release

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
           Sources/Public/Managers/Processes/GameProcess.h

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
           Sources/Private/Managers/Processes/GameProcess.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH = qml
QML2_IMPORT_PATH = qml

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Directories
release: DESTDIR = build/release
debug:   DESTDIR = build/debug

OBJECTS_DIR = $$DESTDIR/.obj
MOC_DIR = $$DESTDIR/.moc
RCC_DIR = $$DESTDIR/.qrc
UI_DIR = $$DESTDIR/.ui

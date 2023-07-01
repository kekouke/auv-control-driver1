TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle                 # Не собирать маковский архив
CONFIG -= qt
QT -= gui
QT -= core
DESTDIR = ../../bin                  # Папка с бинарниками проекта
OBJECTS_DIR = $$PWD/build            # Путь объектников
MOC_DIR = $$PWD/build                # Путь моков
RCC_DIR = $$PWD/build                # Путь ресорцов
TARGET = driver

INSTALL.path = $$DESTDIR             # Куда копируем

SOURCES += \
    command.cpp \
    commands.cpp \
    commandshandler.cpp \
    gamepad.cpp \
    main.cpp \
    application.cpp \
    sender.cpp


HEADERS += \
    command.h \
    commands.h \
    commandshandler.h \
    messages.h \
    application.h \
    gamepad.h \
    motion.h \
    sender.h


win32 {
    # Путь до библиотеки SDL:
    SDL2_PATH = "..\..\lib\SDL2-2.26.4\i686-w64-mingw32"

    # Путь до библиотеки IPC:
    IPC_LIB_PATH = "..\..\lib\ipc_lib\win32\usr"
}

win64 {
    # Путь до библиотеки SDL:
    SDL2_PATH = "..\..\lib\SDL2-2.26.4\x86_64-w64-mingw32"

    # Путь до библиотеки IPC:
}

linux-g++ {
    # Путь до библиотеки SDL:

    # Путь до библиотеки IPC:
    IPC_LIB_PATH = "..\..\lib\ipc_lib\linux\usr"
}


INCLUDEPATH += \
    $${SDL2_PATH}\include \
    $${IPC_LIB_PATH}\include
LIBS += \
    -L$${SDL2_PATH}\lib -lSDL2 -lSDL2main \
    -L$${IPC_LIB_PATH}\lib -lipc

INSTALL.files  += $${IPC_LIB_PATH}/lib/*      # Что копируем
INSTALL.files  += $$PWD/../../load            # Директория с настройками
INSTALL.files  += $$PWD/run_with_viewer.cmd   # Скрипт запуска приложения
INSTALL.files += $${SDL2_PATH}\bin\SDL2.dll   # Библиотека SDL
INSTALLS       += INSTALL                     # Добавляем к установке свои копирования

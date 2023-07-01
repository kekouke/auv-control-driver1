TARGET  = utility

QT += quick qml

CONFIG += c++11


SDL2_PATH = "C:\libs\SDL2-2.26.4\i686-w64-mingw32"

INCLUDEPATH += \
    $${SDL2_PATH}\include \

LIBS += \
    -L$${SDL2_PATH}\lib -lSDL2 -lSDL2main \

SOURCES += \
        controller.cpp \
        gamepad.cpp \
        main.cpp

RESOURCES += qml.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    command.h \
    controller.h \
    gamepad.h


DESTDIR = ../../bin
OBJECTS_DIR = $$PWD/build            # Путь объектников
MOC_DIR = $$PWD/build                # Путь моков
RCC_DIR = $$PWD/build                # Путь ресорцов

QT += widgets network
CONFIG += c++11

#QT_QPA_PLATFORM=wayland

SOURCES += \
    src/Backend.cpp \
    src/TodoBlock.cpp \
    src/TodoWindow.cpp \
    src/dataEngine.cpp \
    src/qPadNote.cpp \
    src/NetworkEngine.cpp

HEADERS += \
    include/Backend.h \
    include/TodoBlock.h \
    include/TodoWindow.h \
    include/data.h \
    include/dataEngine.h \
    include/NetworkEngine.h

INCLUDEPATH += include

RESOURCES += \
    Data/ResourceFile.qrc

DESTDIR = $$PWD/build/ \
        $$PWD/Data/

DISTFILES = $$PWD/Data/sampleConfig.json

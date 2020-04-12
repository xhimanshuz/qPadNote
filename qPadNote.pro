QT += widgets network
CONFIG += c++11

#QT_QPA_PLATFORM=wayland

SOURCES += \
    Backend.cpp \
    TodoBlock.cpp \
    TodoWindow.cpp \
    dataEngine.cpp \
    qPadNote.cpp \
    NetworkEngine.cpp

HEADERS += \
    Backend.h \
    TodoBlock.h \
    TodoWindow.h \
    data.h \
    dataEngine.h \
    NetworkEngine.h

RESOURCES += \
    ResourceFile.qrc

DESTDIR = $$PWD/build/

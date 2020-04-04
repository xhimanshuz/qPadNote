QT += widgets
CONFIG += c++11

#QT_QPA_PLATFORM=wayland

SOURCES += \
    Backend.cpp \
    TodoBlock.cpp \
    TodoWindow.cpp \
    dataEngine.cpp \
    qPadNote.cpp

HEADERS += \
    Backend.h \
    TodoBlock.h \
    TodoWindow.h \
    dataEngine.h

RESOURCES += \
    ResourceFile.qrc

DESTDIR = $$PWD/build/

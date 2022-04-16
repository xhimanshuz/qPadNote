QT += widgets network

SOURCES += \
    src/JsonFileIO.cpp \
    src/Backend.cpp \
    src/TodoBlock.cpp \
    src/TodoWindow.cpp \
    src/dataEngine.cpp \
    src/qPadNote.cpp

HEADERS += \
    include/JsonFileIO.h \
    include/AbstractIO.h \
    include/Backend.h \
    include/Log.h \
    include/TodoBlock.h \
    include/TodoWindow.h \
    include/data.h \
    include/dataEngine.h

OS_TYPE = "unix"
BUILD_TYPE = "debug"

CONFIG(release, debug|release) {
  BUILD_TYPE = "release"
}


win32 {
    OS_TYPE = "win32"

    CONFIG += static
    INCLUDEPATH += include
    DEFINES += "__PRETTY_FUNCTION__=__FUNCSIG__"
}

unix {
    INCLUDEPATH += include \
      /home/gnu/Documents/Codes/git/spdlog/include
#      LIBS += -lspdlog
}

message("OS Detected: $$OS_TYPE")
message("Building Mode $$BUILD_TYPE")

RESOURCES += \
    Data/ResourceFile.qrc


DESTDIR = $$PWD/build/

DISTFILES = $$PWD/build/config.json \
            README.md \
            stylesheet.qss \
            $$PWD/build/qPadNote.conf

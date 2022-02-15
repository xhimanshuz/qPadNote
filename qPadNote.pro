QT += widgets network
CONFIG += c++17

SOURCES += \
    applicationconfig.cpp \
    firebaseauth.cpp \
    src/Block.cpp \
    src/Backend.cpp \
    src/Firebase.cpp \
    src/TodoBlock.cpp \
    src/TodoWindow.cpp \
    src/dataEngine.cpp \
    src/qPadNote.cpp \
    userui.cpp

HEADERS += \
    applicationconfig.h \
    firebaseauth.h \
    include/AbstractIO.h \
    include/Block.h \
    include/Protocol.h \
    include/Backend.h \
    include/TodoBlock.h \
    include/TodoWindow.h \
    include/data.h \
    include/dataEngine.h \
    include/Firebase.h \
    userui.h

OS_TYPE = "unix"
BUILD_TYPE = "debug"

CONFIG(release, debug|release) {
  BUILD_TYPE = "release"
}

win32 {
    OS_TYPE = "win32"

    CONFIG += static
    LIBS += -L$$PWD/libs/windows/$$BUILD_TYPE \
            -ladvapi32 -lws2_32 -lcrypt32 -liphlpapi -lpsapi -luserenv -lshell32 -lUser32 -lOle32 \
            -lfirebase_app -lfirebase_database -lfirebase_auth -lfirebase_analytics

    INCLUDEPATH += include \
                   C:\\local\\boost_1_76_0
}

unix {
    LIBS += -L$$PWD/libs/unix/$$BUILD_TYPE \
            -lpthread -lglib-2.0 -lsecret-1 \
            -lfirebase_app -lfirebase_database -lfirebase_auth -lfirebase_analytics

    INCLUDEPATH += include
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

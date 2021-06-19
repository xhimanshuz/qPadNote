QT += widgets network
CONFIG += c++17 static staticlib
QMAKE_CXXFLAGS += -static

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

INCLUDEPATH += include \
            /usr/local/include/mongocxx/v_noabi \
            /usr/local/include/bsoncxx/v_noabi

RESOURCES += \
    Data/ResourceFile.qrc

DESTDIR = $$PWD/build/

DISTFILES = $$PWD/build/config.json \
            README.md \
            build/stylesheet.qss

LIBS += -Wl,--verbose \
        -L$$PWD/lib -lfirebase_auth -lfirebase_database -lfirebase_analytics -lfirebase_app -lpthread -lglib-2.0 -lsecret-1

QT += widgets network
CONFIG += c++17 static staticlib
QMAKE_CXXFLAGS += -static

SOURCES += \
    src/Block.cpp \
    src/Backend.cpp \
    src/Firebase.cpp \
    src/TodoBlock.cpp \
    src/TodoWindow.cpp \
    src/dataEngine.cpp \
    src/qPadNote.cpp

HEADERS += \
    include/AbstractIO.h \
    include/Block.h \
    include/Protocol.h \
    include/Backend.h \
    include/TodoBlock.h \
    include/TodoWindow.h \
    include/data.h \
    include/dataEngine.h \
    include/Firebase.h

INCLUDEPATH += include \
            /usr/local/include/mongocxx/v_noabi \
            /usr/local/include/bsoncxx/v_noabi

RESOURCES += \
    Data/ResourceFile.qrc

DESTDIR = $$PWD/build/

DISTFILES = $$PWD/build/config.json \
            README.md

LIBS += -Wl,--verbose -lpthread \
        -L$$PWD/lib -lfirebase_database -lfirebase_analytics -lfirebase_app

QT += widgets network
CONFIG += c++11 static

#QT_QPA_PLATFORM=wayland

SOURCES += \
    src/Block.cpp \
    src/Backend.cpp \
    src/NetworkEngine.cpp \
    src/TodoBlock.cpp \
    src/TodoWindow.cpp \
    src/dataEngine.cpp \
    src/qPadNote.cpp

HEADERS += \
    include/AbstractIO.h \
    include/Block.h \
    include/Protocol.h \
    include/Backend.h \
    include/NetworkEngine.h \
    include/TodoBlock.h \
    include/TodoWindow.h \
    include/data.h \
    include/dataEngine.h

INCLUDEPATH += include \
            /usr/local/include/mongocxx/v_noabi \
            /usr/local/include/bsoncxx/v_noabi

RESOURCES += \
    Data/ResourceFile.qrc

DESTDIR = $$PWD/build/

DISTFILES = $$PWD/build/config.json \
            README.md

LIBS += -lpthread \
        -L/usr/local/lib -lmongocxx -lbsoncxx

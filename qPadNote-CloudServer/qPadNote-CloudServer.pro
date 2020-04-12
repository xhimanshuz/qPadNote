HEADERS += \
    cloudSession.h \
    cloudserverengine.h \
    data.h

SOURCES += \
    Run.cpp \
    cloudSession.cpp \
    cloudserverengine.cpp

LIBS += -lpthread

DESTDIR = $$PWD/build

DISTFILES += build/setting.json

HEADERS += \
    include/cloudSession.h \
    include/cloudserverengine.h \
    include/data.h

SOURCES += \
    src/Run.cpp \
    src/cloudSession.cpp \
    src/cloudserverengine.cpp

INCLUDEPATH += include

LIBS += -lpthread

DESTDIR = $$PWD/build

DISTFILES += build/setting.json

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4) {
  QT += avwidgets
} else {
  CONFIG += avwidgets
}
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

unix:!mac {
LIBS += -L$$QT.core.libs -lQtAV -lQtAVWidgets
}
unix:mac {
INCLUDEPATH += $$QT.core.libs/QtAV.framework/Versions/1/Headers
QMAKE_LFLAGS += -F$$QT.core.libs
LIBS += -framework QtAV QtAVWidgets
}
win32{
LIBS += -L$$QT.core.libs -lQtAV1 -lQtAVWidgets1
}

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    playerwindow.cpp \
    registrationwidget.cpp

HEADERS += \
    Adresses.h \
    mainwindow.h \
    playerwindow.h \
    registrationwidget.h

FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    stream_service_ru_RU.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

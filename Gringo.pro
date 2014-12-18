#-------------------------------------------------
#
# Project created by QtCreator 2014-01-21T12:22:25
#
#-------------------------------------------------

QT       += core gui xml svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Gringo
TEMPLATE = app

SOURCES += main.cpp \
    window.cpp

HEADERS  += \
    window.hpp

RESOURCES += \
    systray.qrc

target.path = /bin
INSTALLS += target

wrapper.files = gringo-timetracking
wrapper.path = /bin
INSTALLS += wrapper

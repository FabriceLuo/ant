#-------------------------------------------------
#
# Project created by QtCreator 2017-03-17T23:43:12
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


TARGET = Ant
TEMPLATE = app


SOURCES += main.cpp\
        Ant.cpp \
    AntSetting.cpp \
    AntBackup.cpp \
    AntFinder.cpp \
    AntCommander.cpp \
    AntVersion.cpp \
    AntSvn.cpp \
    AntGit.cpp \
    AntCommon.cpp \
    AntDiff.cpp

HEADERS  += Ant.h \
    AntSetting.h \
    AntBackup.h \
    AntFinder.h \
    AntCommander.h \
    AntVersion.h \
    AntSvn.h \
    AntGit.h \
    AntCommon.h \
    AntDiff.h

INCLUDEPATH += /usr/local/include


RESOURCES += \
    ant.qrc

win32: LIBS += -lws2_32

win32: LIBS += -L$$PWD/../../AntDep/install/libssh2/lib/ -llibssh2

INCLUDEPATH += $$PWD/../../AntDep/install/libssh2/include
DEPENDPATH += $$PWD/../../AntDep/install/libssh2/include

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
    CheckboxHeaderView.cpp \
    CheckboxTable.cpp \
    AntSyncer.cpp \
    AntSetting.cpp \
    AntBackup.cpp \
    AntLog.cpp \
    AntTransfer.cpp \
    AntFinder.cpp \
    AntCommander.cpp \
    AntVersion.cpp \
    AntSvn.cpp \
    AntGit.cpp \
    AntCommon.cpp

HEADERS  += Ant.h \
    CheckboxHeaderView.h \
    CheckboxTable.h \
    AntSyncer.h \
    AntSetting.h \
    AntBackup.h \
    AntLog.h \
    AntTransfer.h \
    AntFinder.h \
    AntCommander.h \
    AntVersion.h \
    AntSvn.h \
    AntGit.h \
    AntCommon.h

RESOURCES += \
    ant.qrc

LIBS += -lssh2

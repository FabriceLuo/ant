﻿#include "AntSvn.h"
#include <QProcess>
#include <QFileInfo>
#include <QDir>
#include <QRegularExpression>
#include <QList>

AntSvn::AntSvn(QString dir)
    :AntVersion(dir)
{

}

QList<VersionEntry> AntSvn::getchangeList()
{
    QString cmd("svn");
    QStringList param;
    param << "status" << getWorkDir();

    QProcess process;
    process.start(cmd, param);
    process.waitForStarted();
    process.waitForFinished();

    QList<VersionEntry> list;

    QString changeString = process.readAllStandardOutput();
    QStringList changeList = changeString.split(QRegularExpression("\r?\n"));
    int changeCount = changeList.count();

    QDir svnDir(getWorkDir());

    for(int i=0;i < changeCount; i++)
    {
        QString changeEntry = changeList.at(i);
        changeEntry.remove(QRegularExpression("^ +"));
        QStringList entryList = changeEntry.split(QRegularExpression(" +"));

        if(entryList.count() < 2)
        {
            continue;
        }
        QFileInfo fileInfo(entryList.at(1));
        if(fileInfo.isDir())
        {
            continue;
        }

        VersionEntry node;
        node.path    = fileInfo.absoluteFilePath();

        QString status = entryList.at(0);
        if(status.compare("A") == 0)
        {
            node.status = EntryAdd;
        }
        else if(status.compare("M") == 0)
        {
            node.status = EntryModify;
        }
        else if(status.compare("D") == 0)
        {
            node.status = EntryDelete;
        }
        else if(status.compare("?") == 0) {
            node.status = EntryUnversion;
        }
        else {
            node.status = EntryError;
        }

        node.name    = fileInfo.fileName();
        node.size    = fileInfo.size();

        list.push_back(node);
    }


    return list;
}

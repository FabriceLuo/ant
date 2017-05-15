#include "AntGit.h"
#include <QProcess>
#include <QDir>
#include <QFileInfo>
#include <QRegularExpression>

AntGit::AntGit(QString dir)
    :AntVersion(dir)
{

}

QList<VersionEntry> AntGit::getchangeList()
{
    QString cmd("git");
    QStringList param = {
        "-C",
        getWorkDir(),
        "status",
        "-s"
    };

    QProcess process;
    process.start(cmd, param);
    process.waitForStarted();
    process.waitForFinished();

    QList<VersionEntry> list;

    QString changeString = process.readAllStandardOutput();
    QStringList changeList = changeString.split("\n");
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
        QFileInfo fileInfo(svnDir, entryList.at(1));

        VersionEntry node;
        node.path    = fileInfo.absoluteFilePath();
        node.status  = !entryList.at(0).compare("M") ? EntryModify : EntryAdd;
        node.name    = fileInfo.baseName();
        node.size    = fileInfo.size();

        list.push_back(node);
    }


    return list;
}

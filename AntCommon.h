#ifndef ANTCOMMON_H
#define ANTCOMMON_H

#include <QMessageBox>

//需要添加的代码，防止中文出现乱码
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

#include <QString>
#include <QList>

enum EntryStatus
{
    EntryError,
    EntryAdd,
    EntryModify,
    EntryDelete,
    EntryUnversion
};

class AntCommon
{
public:
    AntCommon();
    static QString getEntryStatusString(EntryStatus status);
};

class VersionEntry
{
public:
    QString path;
    QString name;
    int size;
    EntryStatus status;
};

enum VersionType {
    VersionTypeErr,
    VersionTypeSvn,
    VersionTypeGit
};

#endif // ANTCOMMON_H

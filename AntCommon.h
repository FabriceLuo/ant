#ifndef ANTCOMMON_H
#define ANTCOMMON_H

#include <QString>
#include <QList>

enum EntryStatus
{
    EntryError,
    EntryAdd,
    EntryModify,
    EntryDelete
};

class AntCommon
{
public:
    AntCommon();
};

class VersionEntry
{
public:
    QString path;
    QString name;
    int64_t size;
    EntryStatus status;
};

enum VersionType {
    VersionTypeErr,
    VersionTypeSvn,
    VersionTypeGit
};

#endif // ANTCOMMON_H

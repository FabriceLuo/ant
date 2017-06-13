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

class SyncEntry
{
public:
    QString src;
    QString des;
};


enum VersionTableCol {
    VersionBegin = 0,
    VersionCheck = VersionBegin,
    VersionName,
    VersionLocalPath,
    VersionRemotePath,
    VersionModifyTime,
    VersionStatus,
    VersionEnd = VersionStatus
};

enum NodeStatus {
    NodeStatusUnkown,
    NodeStatusOff,
    NodeStatusOn,
    NodeStatusError
};

class ClusterNode {
public:
    QString name;
    QString ip;
    QString username;
    QString password;
    bool online;
};

enum ClusterServiceType {
    ClusterServiceStart,
    ClusterServiceStop,
    ClusterServiceRestart
};

class ClusterServiceCmd {
    ClusterServiceType type;
    QString description;
    QString command;
};

class ClusterService {
public:
    QString name;
    QString description;
    QList<ClusterServiceCmd> cmdList;
};


#endif // ANTCOMMON_H

#ifndef ANTSYNC_H
#define ANTSYNC_H

#include <QString>
#include "AntCommon.h"

class AntSync
{
public:
    AntSync(QString master, QString username, QString password);
    bool syncToMaster(const QList<SyncEntry> &entryList);
    bool syncToCluster(const QList<SyncEntry> &entryList);

private:
    QString m_master, m_username, m_password;
    QStringList m_slaveList;

    QString getMaster();
    QString getUserName();
    QString getPassword();

    QList<ClusterNode> getClusterNodeList();
    bool syncToNode(QString node, const QList<SyncEntry> &entryList);

};

#endif // ANTSYNC_H

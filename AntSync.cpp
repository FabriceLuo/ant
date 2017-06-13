#include "AntSync.h"

#include <QMessageBox>


#include <QFileInfo>
#include <QDir>
#include "AntCommander.h"
#include "AntCluster.h"

AntSync::AntSync(QString master, QString username, QString password):
    m_master(master), m_username(username), m_password(password)
{

}

bool AntSync::syncToMaster(const QList<SyncEntry> &entryList)
{
    QString master = getMaster();
    return syncToNode(master, entryList);
}

bool AntSync::syncToCluster(const QList<SyncEntry> &entryList)
{
    ClusterNode node;
    node.ip = getMaster();
    node.username = getUserName();
    node.password = getPassword();

    AntCluster cluster(node);
    QList<ClusterNode> nodeList = cluster.getNodeList();
    if(nodeList.isEmpty())
    {
        return false;
    }

    QList<ClusterNode>::iterator nodeBegin = nodeList.begin();
    QList<ClusterNode>::iterator nodeEnd = nodeList.end();

    while(nodeBegin != nodeEnd)
    {
        if(! syncToNode(nodeBegin->ip, entryList))
        {
            fprintf(stderr, "sync code to cluster failed");
            return false;
        }
        nodeBegin++;
    }

    return true;
}

QString AntSync::getMaster()
{
    return m_master;
}

QString AntSync::getUserName()
{
    return m_username;
}

QString AntSync::getPassword()
{
    return m_password;
}

bool AntSync::syncToNode(QString node, const QList<SyncEntry> &entryList)
{
    if(entryList.isEmpty())
    {
        return true;
    }
    QString username = getUserName(), password = getPassword();
    AntCommander commander(node, username, password);

    int entryCount = entryList.count();
    int i = 0;
    while (i < entryCount) {
        SyncEntry entry = entryList.at(i);
        QString dir = QFileInfo(entry.des).path();
        if(! commander.mkdir(dir))
        {
            fprintf(stderr, "create dir failed");
            return false;
        }
        if(! commander.scpTo(entry.src, entry.des))
        {
            fprintf(stderr, "scp file(%s) to file(%s) at node(%s) failed", entry.src.toStdString().c_str(), entry.des.toStdString().c_str());
            return false;
        }
        i++;
    }

    return true;
}





















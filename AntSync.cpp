#include "AntSync.h"

#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonParseError>

#include "AntCommander.h"

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
    QList<ClusterNode> nodeList = getClusterNodeList();
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



QList<ClusterNode> AntSync::getClusterNodeList()
{
    QString master = getMaster(), username = getUserName(), password = getPassword();

    QString cmd = "cat";
    QStringList param;
    QList<ClusterNode> list;
    param << "/cfs/.members";

    QString stdOut, stdErr;
    AntCommander commander(master, username, password);
    if(0 != commander.exec(cmd, param, stdOut, stdErr))
    {
        fprintf(stderr, "read /cfs/.members failed");
        return list;
    }

    if(stdOut.isEmpty())
    {
        fprintf(stderr, "/cfs/.members content error");
        return list;
    }
    //std::string content = stdOut.toStdString();
    //QJsonDocument doc = QJsonDocument::fromRawData(content.c_str(), content.size());
    QJsonParseError error;
    QByteArray data = stdOut.toLatin1();
    QJsonDocument doc = QJsonDocument::fromJson(data, &error);
    if(doc.isNull() || doc.isArray())
    {
        fprintf(stderr, "convert cfs .members to json doc failed");
        return list;
    }

    QJsonObject root = doc.object();
    if(root.isEmpty())
    {
        fprintf(stderr, "cfs members content error");
        return list;
    }

    if(! root.contains("nodelist"))
    {
        fprintf(stderr, "no node list content was found");
        return list;
    }

    QJsonObject nodelistObj = root.value("nodelist").toObject();
    QStringList nameList = nodelistObj.keys();
    for(int i = 0; i < nameList.count(); i++)
    {
        QJsonObject nodeObj = nodelistObj.value(nameList.at(i)).toObject();

        ClusterNode node;
        node.name = nameList.at(i);
        if(nodeObj.value("online").toInt() == 0)
        {
            node.online = false;
        }
        else {
            node.online = true;
        }
        node.ip = nodeObj.value("ip").toString();

        list.push_back(node);
    }

    return list;
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
        if(! commander.scpTo(entry.src, entry.des))
        {
            fprintf(stderr, "scp file(%s) to file(%s) at node(%s) failed", entry.src.toStdString().c_str(), entry.des.toStdString().c_str());
            return false;
        }
        i++;
    }

    return true;
}





















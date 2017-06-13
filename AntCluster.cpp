#include "AntCluster.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonParseError>

AntCluster::AntCluster(ClusterNode node):
    m_master(node)
{

}

QList<ClusterNode> AntCluster::getNodeList()
{
    QString master = m_master.ip, username = m_master.username, password = m_master.password;

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

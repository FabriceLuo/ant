#include "AntClusterSetting.h"
#include <QFile>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QDebug>
#include <QJsonParseError>
AntClusterSetting::AntClusterSetting(QString cfgPath):
    m_cfgPath(cfgPath),
    NODE_LIST_KEY("ClusterList"),
    SEARCH_DIR_KEY("SearchDir"),
    NODE_NAME_KEY("name"),
    NODE_IP_KEY("ip"),
    NODE_USERNAME_KEY("username"),
    NODE_PASSWORD_KEY("password")
{

}

bool AntClusterSetting::getNodeList(QList<ClusterNode> &list)
{
    QJsonObject *root = getSetting();
    if(root == NULL)
    {
        fprintf(stderr, "get setting failed");
        return false;
    }

    QJsonValue nodeListValue = root->value(NODE_LIST_KEY);
    if(nodeListValue.isUndefined())
    {
        return false;
    }

    QJsonObject nodeListObj = nodeListValue.toObject();
    QStringList nodeList = nodeListObj.keys();

    QStringList::iterator begin = nodeList.begin();
    QStringList::iterator end = nodeList.end();

    while (begin != end) {
        ClusterNode node;
        QJsonObject nodeObj = nodeListObj.value(*begin).toObject();
        //name
        if(!nodeObj.contains(NODE_NAME_KEY) ||
                !nodeObj.contains(NODE_IP_KEY) ||
                !nodeObj.contains(NODE_PASSWORD_KEY) ||
                !nodeObj.contains(NODE_USERNAME_KEY))
        {
            fprintf(stderr, "key is not exist");
            return false;
        }
        node.name = nodeObj.value(NODE_NAME_KEY).toString();
        node.ip = nodeObj.value(NODE_IP_KEY).toString();
        node.username = nodeObj.value(NODE_USERNAME_KEY).toString();
        node.password = nodeObj.value(NODE_PASSWORD_KEY).toString();
        begin++;

        list.push_back(node);
    }
    return true;
}

bool AntClusterSetting::setNodeList(const QList<ClusterNode> &list)
{
    QJsonObject *root = getSetting();
    if(root == NULL)
    {
        fprintf(stderr, "get setting failed");
        return false;
    }
    //get node list jsonobj
    int nodeCount = list.count();
    QJsonObject nodeListObj;
    for(int i = 0; i < nodeCount; i++)
    {
        const ClusterNode node = list.at(i);
        QJsonObject nodeObj;
        nodeObj.insert(NODE_NAME_KEY, QJsonValue(node.name));
        nodeObj.insert(NODE_USERNAME_KEY, QJsonValue(node.username));
        nodeObj.insert(NODE_PASSWORD_KEY, QJsonValue(node.password));
        nodeObj.insert(NODE_IP_KEY, QJsonValue(node.ip));

        nodeListObj.insert(node.name, nodeObj);
    }

    //replace old node list
    root->remove(NODE_LIST_KEY);
    root->insert(NODE_LIST_KEY, nodeListObj);

    return setSetting(root);
}

bool AntClusterSetting::getNodeSearchDir(QString uuid, QStringList &dirList)
{
    return false;
}

bool AntClusterSetting::getGlobalSearchDir(QStringList &dirList)
{
    QJsonObject *root = getSetting();
    if(root == NULL)
    {
        fprintf(stderr, "get setting failed");
        return false;
    }

    QJsonValue searchDirValue = root->value(SEARCH_DIR_KEY);
    if(!searchDirValue.isArray())
    {
        return false;
    }
    QJsonArray searchDirArr = searchDirValue.toArray();
    int dirCount = searchDirArr.count();
    for(int i = 0;i < dirCount; i++)
    {
        dirList.push_back(searchDirArr.at(i).toString());
    }
    return true;
}

QString AntClusterSetting::getCfgPath()
{
    return m_cfgPath;
}

QJsonDocument AntClusterSetting::readSetting()
{
    QJsonDocument doc;
    //read content from json file
    QString cfgPath = getCfgPath();
    QFile file(cfgPath);
    if(! file.open(QIODevice::ReadOnly))
    {
        fprintf(stderr, "open cfg file failed");
        return doc;
    }
    QByteArray content = file.readAll();

    doc = QJsonDocument::fromJson(content);
    file.close();
    return doc;
}

bool AntClusterSetting::writeSetting(QJsonDocument setting)
{
    //read content from json file
    QString cfgPath = getCfgPath();
    QFile file(cfgPath);
    if(! file.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        fprintf(stderr, "open cfg file failed");
        return false;
    }

    QByteArray content = setting.toJson();
    if(file.write(content) != content.size() )
    {
        fprintf(stderr, "write cfg failed");
        return false;
    }

    return true;
}

QJsonObject *AntClusterSetting::getSetting()
{
    QJsonObject *setting = NULL;
    QJsonDocument doc = readSetting();
    if(doc.isNull())
    {
        fprintf(stderr, "read cfg failed");
    }
    else
    {
        setting = new QJsonObject(doc.object());
    }
    return setting;
}

bool AntClusterSetting::setSetting(QJsonObject *setting)
{
    if(setting == NULL)
    {
        fprintf(stderr, "setting is null");
        return false;
    }

    QJsonDocument doc(*setting);
    if(! writeSetting(doc))
    {
        fprintf(stderr, "write cfg path failed");
        return false;
    }
    return true;
}
















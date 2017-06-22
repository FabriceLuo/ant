#include "AntSetting.h"
#include <QFile>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QDebug>
#include <QJsonParseError>

AntSetting::AntSetting(QString cfgPath):
    m_cfgPath(cfgPath),
    NODE_LIST_KEY("ClusterList"),
    SEARCH_DIR_KEY("SearchDir"),
    SERVICE_LIST_KEY("Service"),
    NODE_NAME_KEY("name"),
    NODE_IP_KEY("ip"),
    NODE_USERNAME_KEY("username"),
    NODE_PASSWORD_KEY("password"),
    SERVICE_NAME_KEY("name"),
    SERVICE_DES_KEY("description"),
    SERVICE_CMD_KEY("cmdList"),
    SERVICE_CMD_TYPE_KEY("type"),
    SERVICE_CMD_DES_KEY("description"),
    SERVICE_CMD_COMMAND_KEY("command")
{

}

bool AntSetting::getNodeList(QList<ClusterNode> &list)
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

bool AntSetting::setNodeList(const QList<ClusterNode> &list)
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
    if(root->contains(NODE_LIST_KEY))
    {
        root->remove(NODE_LIST_KEY);
    }
    root->insert(NODE_LIST_KEY, nodeListObj);

    return setSetting(root);
}

bool AntSetting::getNodeSearchDir(QString uuid, QStringList &dirList)
{
    return false;
}

bool AntSetting::getGlobalSearchDir(QStringList &dirList)
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

bool AntSetting::setGlobalSearchDir(const QStringList &dirList)
{
    QJsonObject *root = getSetting();
    if(root == NULL)
    {
        fprintf(stderr, "get setting failed");
        return false;
    }
    QJsonArray searchDirObj;
    int dirCount = dirList.count();
    for(int i = 0; i < dirCount; i++)
    {
        searchDirObj.push_back(dirList.at(i));
    }
    if(root->contains(SEARCH_DIR_KEY))
    {
        root->remove(SEARCH_DIR_KEY);
    }
    root->insert(SEARCH_DIR_KEY, searchDirObj);
    if(! setSetting(root))
    {
        fprintf(stderr, "write search directory failed");
        return false;
    }
    return true;
}

bool AntSetting::getServiceList(QList<ClusterService> &list)
{
    QJsonObject *root = getSetting();
    if(root == NULL)
    {
        fprintf(stderr, "get setting failed");
        return false;
    }
    QJsonValue serviceListValue = root->value(SERVICE_LIST_KEY).toObject();
    if(serviceListValue.isUndefined() || !serviceListValue.isObject())
    {
        fprintf(stderr, "read service info failed");
        return false;
    }
    QJsonObject serviceListObj = serviceListValue.toObject();
    QStringList nameList = serviceListObj.keys();

    int serviceCount = nameList.count();
    for(int i = 0; i < serviceCount; i++)
    {
        QJsonObject serviceObj = serviceListObj.value(nameList.at(i)).toObject();
        ClusterService service;
        service.name = serviceObj.value(SERVICE_NAME_KEY).toString();
        service.description = serviceObj.value(SERVICE_DES_KEY).toString();

        QJsonArray cmdListArr = serviceObj.value(SERVICE_CMD_KEY).toArray();
        int cmdCount = cmdListArr.count();
        QList<ClusterServiceCmd> cmdList;
        for(int j = 0; j < cmdCount; j++)
        {
            ClusterServiceCmd cmd;
            QJsonObject cmdObj = cmdListArr.at(j).toObject();

            cmd.type = (ClusterServiceType)cmdObj.value(SERVICE_CMD_TYPE_KEY).toInt();
            cmd.command = cmdObj.value(SERVICE_CMD_COMMAND_KEY).toString();
            cmd.description = "";

            cmdList.push_back(cmd);
        }
        service.cmdList = cmdList;

        list.push_back(service);
    }

    return true;
}

bool AntSetting::setServiceList(const QList<ClusterService> &list)
{
    QJsonObject serviceListObj;

    int listCount = list.count();
    for(int i = 0 ; i < listCount; i++)
    {

        ClusterService service = list.at(i);

        QJsonObject serviceObj;
        serviceObj.insert(SERVICE_NAME_KEY, QJsonValue(service.name));
        serviceObj.insert(SERVICE_DES_KEY, QJsonValue(service.description));

        QJsonArray cmdListArr;
        QList<ClusterServiceCmd> cmdList = service.cmdList;
        int cmdCount = cmdList.count();

        for(int j = 0; j < cmdCount; j++)
        {
            QJsonObject cmdObj;
            cmdObj.insert(SERVICE_CMD_TYPE_KEY, QJsonValue(cmdList.at(j).type));
            cmdObj.insert(SERVICE_CMD_DES_KEY, QJsonValue(cmdList.at(j).description));
            cmdObj.insert(SERVICE_CMD_COMMAND_KEY,  QJsonValue(cmdList.at(j).command));

            cmdListArr.push_back(cmdObj);
        }
        serviceObj.insert(SERVICE_CMD_KEY, cmdListArr);

        serviceListObj.insert(service.name, serviceObj);
    }

    QJsonObject *root = getSetting();
    if(root == NULL)
    {
        fprintf(stderr, "read setting failed");
        return false;
    }

    if(root->contains(SERVICE_LIST_KEY))
    {
        root->remove(SERVICE_LIST_KEY);
    }
    root->insert(SERVICE_LIST_KEY, serviceListObj);

    if(! setSetting(root))
    {
        fprintf(stderr, "set setting failed");
        return false;
    }

    return true;
}

QString AntSetting::getCfgPath()
{
    return m_cfgPath;
}

QJsonDocument AntSetting::readSetting()
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

bool AntSetting::writeSetting(QJsonDocument setting)
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

QJsonObject *AntSetting::getSetting()
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

bool AntSetting::setSetting(QJsonObject *setting)
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
















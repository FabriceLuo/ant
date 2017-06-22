#ifndef ANTSETTING_H
#define ANTSETTING_H

#include <QString>
#include <QJsonDocument>
#include <QObject>
#include "AntCommon.h"

class AntSetting
{
public:
    AntSetting(QString cfgPath = "./Setting/cluster_setting.json");
    bool getNodeList(QList<ClusterNode> &list);
    bool setNodeList(const QList<ClusterNode> &list);
    bool getNodeSearchDir(QString uuid, QStringList &dirList);
    bool getGlobalSearchDir(QStringList &dirList);
    bool setGlobalSearchDir(const QStringList &dirList);
    bool getServiceList(QList<ClusterService> &list);
    bool setServiceList(const QList<ClusterService> &list);
private:
    QString m_cfgPath;
    QJsonObject *m_setting;

    QString getCfgPath();

    QJsonDocument readSetting();
    bool writeSetting(QJsonDocument setting);

    QJsonObject *getSetting();
    bool setSetting(QJsonObject *setting);

    const QString NODE_LIST_KEY, SEARCH_DIR_KEY, SERVICE_LIST_KEY;

    const QString NODE_NAME_KEY, NODE_USERNAME_KEY, NODE_PASSWORD_KEY, NODE_IP_KEY;

    const QString SERVICE_NAME_KEY, SERVICE_DES_KEY, SERVICE_CMD_KEY, SERVICE_CMD_TYPE_KEY, SERVICE_CMD_COMMAND_KEY, SERVICE_CMD_DES_KEY;
};

#endif // ANTCLUSTERSETTING_H

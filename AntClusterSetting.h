#ifndef ANTCLUSTERSETTING_H
#define ANTCLUSTERSETTING_H

#include <QString>
#include <QJsonDocument>
#include <QObject>
#include "AntCommon.h"

class AntClusterSetting
{
public:
    AntClusterSetting(QString cfgPath = "./Setting/cluster_setting.json");
    bool getNodeList(QList<ClusterNode> &list);
    bool setNodeList(const QList<ClusterNode> &list);
    bool getNodeSearchDir(QString uuid, QStringList &dirList);
    bool getGlobalSearchDir(QStringList &dirList);
private:
    QString m_cfgPath;
    QJsonObject *m_setting;

    QString getCfgPath();

    QJsonDocument readSetting();
    bool writeSetting(QJsonDocument setting);

    QJsonObject *getSetting();
    bool setSetting(QJsonObject *setting);

    const QString NODE_LIST_KEY, SEARCH_DIR_KEY;

    const QString NODE_NAME_KEY, NODE_USERNAME_KEY, NODE_PASSWORD_KEY, NODE_IP_KEY;
};

#endif // ANTCLUSTERSETTING_H

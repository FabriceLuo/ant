#ifndef ANTCLUSTER_H
#define ANTCLUSTER_H

#include "AntCommon.h"
#include "AntCommander.h"
#include <QList>

class AntCluster
{
public:
    AntCluster(ClusterNode node);
    QList<ClusterNode> getNodeList();

    //deal with master service
    bool restartMasterService(ClusterService service);
    bool stopMasterService(ClusterService service);
    bool startMasterService(ClusterService service);

    //deal with cluster service
    bool restartClusterService(ClusterService service);
    bool stopClusterService(ClusterService service);
    bool startClusterService(ClusterService service);
private:
    ClusterNode m_master;

    bool restartNodeService(ClusterService service);
    bool stopNodeService(ClusterService service);
};

#endif // ANTCLUSTER_H

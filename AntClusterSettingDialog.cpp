#include "AntClusterSettingDialog.h"
#include <QLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "AntClusterSetting.h"
#include <QTableWidgetItem>


AntClusterSettingDialog::AntClusterSettingDialog()
{
    initWidget();
    initDeploy();
    initConnect();

    loadSetting();
}

void AntClusterSettingDialog::initWidget()
{
    m_listLabel = new QLabel("集群信息");

    m_addBtn = new QPushButton("新增");
    m_deleteBtn = new QPushButton("删除");

    m_clusterList = new QTableWidget();
    m_clusterList->setFixedWidth(500);
    m_clusterHeader << tr("") << tr("名称") << tr("用户名") << tr("密码") << tr("IP");
    m_clusterHeaderWidth << 24 << 100 << 100 << 150 << 105;
    setHorizontalHeader(m_clusterHeader);
    setHorizontalHeaderWidth(m_clusterHeaderWidth);

}

void AntClusterSettingDialog::initConnect()
{
    connect(m_addBtn, SIGNAL(clicked(bool)), this, SLOT(addClusterNode()));
    connect(m_deleteBtn, SIGNAL(clicked(bool)), this, SLOT(deleteClusterNode()));
}

void AntClusterSettingDialog::initDeploy()
{
    QHBoxLayout *headerLayout   = new QHBoxLayout();
    QHBoxLayout *bodyLayout     = new QHBoxLayout();
    QVBoxLayout *opLayout       = new QVBoxLayout();
    QVBoxLayout *mainLayout     = new QVBoxLayout();

    headerLayout->addWidget(m_listLabel);
    headerLayout->addStretch();

    bodyLayout->addWidget(m_clusterList);

    opLayout->addWidget(m_addBtn);
    opLayout->addWidget(m_deleteBtn);

    bodyLayout->addLayout(opLayout);

    mainLayout->addLayout(headerLayout);
    mainLayout->addLayout(bodyLayout);

    setLayout(mainLayout);
}

void AntClusterSettingDialog::setHorizontalHeader(QStringList header)
{
    m_clusterList->setColumnCount(header.count());
    QStringList::Iterator begin = header.begin();
    QStringList::iterator end   = header.end();

    int i = 0;
    while(begin != end)
    {
        QTableWidgetItem *item = new QTableWidgetItem();
        item->setText(*begin);

        m_clusterList->setHorizontalHeaderItem(i, item);
        begin++;
        i++;
    }
}

void AntClusterSettingDialog::setHorizontalHeaderWidth(QList<int> width)
{
    QList<int>::Iterator begin = width.begin();
    QList<int>::Iterator end   = width.end();

    int i = 0;
    while(begin != end)
    {
        m_clusterList->setColumnWidth(i, *begin);
        begin++;
        i++;
    }
}

void AntClusterSettingDialog::loadSetting()
{
    AntClusterSetting settingObj;
    QList<ClusterNode> list;
    if(! settingObj.getNodeList(list))
    {
        fprintf(stderr, "load cluster setting failed");
        return;
    }
    if(list.isEmpty())
    {
        fprintf(stderr, "node list is empty");
        return;
    }
    //display node info
    int nodeCount = list.count();
    m_clusterList->clearContents();
    m_clusterList->setRowCount(nodeCount);
    QTableWidgetItem *item = NULL;
    for(int i = 0; i < nodeCount; i++)
    {
        //node check
        item = new QTableWidgetItem();
        item->setCheckState(Qt::Unchecked);
        m_clusterList->setItem(i, NodeCheck, item);

        //node name
        item = new QTableWidgetItem();
        item->setText(list.at(i).name);
        m_clusterList->setItem(i, NodeName, item);

        //node username
        item = new QTableWidgetItem();
        item->setText(list.at(i).username);
        m_clusterList->setItem(i, NodeUserName, item);

        //node password
        item = new QTableWidgetItem();
        item->setText(list.at(i).password);
        m_clusterList->setItem(i, NodePassword, item);

        //node ip
        item = new QTableWidgetItem();
        item->setText(list.at(i).ip);
        m_clusterList->setItem(i, NodeIP, item);
    }
}

void AntClusterSettingDialog::saveSetting()
{
    QList<ClusterNode> list;
    int nodeCount = m_clusterList->rowCount();

    for(int i = 0; i < nodeCount; i++)
    {
        //check is null
        for(int j = NodeName; j < NodeIP; j ++)
        {
            if(m_clusterList->item(i, j)->text().isEmpty())
            {
                QMessageBox::critical(this, "配置", "集群配置不允许空项");
                return;
            }
        }
        ClusterNode node;
        node.name = m_clusterList->item(i, NodeName)->text();
        node.username = m_clusterList->item(i, NodeUserName)->text();
        node.password = m_clusterList->item(i, NodePassword)->text();
        node.ip = m_clusterList->item(i, NodeIP)->text();

        list.push_back(node);
    }

    AntClusterSetting settingObj;
    if(! settingObj.setNodeList(list))
    {
        QMessageBox::critical(this, "配置", "保存集群配置失败");
    }
}

void AntClusterSettingDialog::addClusterNode()
{
    int curNodeCount = m_clusterList->rowCount();

    m_clusterList->setRowCount(curNodeCount + 1);

    QTableWidgetItem *item = NULL;

    item = new QTableWidgetItem();
    item->setCheckState(Qt::Unchecked);
    m_clusterList->setItem(curNodeCount, NodeCheck, item);

    for(int i = NodeName; i <= NodeIP; i++)
    {
        item = new QTableWidgetItem();
        item->setText("");
        m_clusterList->setItem(curNodeCount, i, item);
    }
}

void AntClusterSettingDialog::deleteClusterNode()
{
    int curNodeCount = m_clusterList->rowCount();
    QList<ClusterNode> remainList;
    for(int i = 0; i < curNodeCount; i++)
    {
        if(m_clusterList->item(i, NodeCheck)->checkState() == Qt::Checked)
        {
            continue;
        }
        ClusterNode node;
        node.name       = m_clusterList->item(i, NodeName)->text();
        node.username   = m_clusterList->item(i, NodeUserName)->text();
        node.password   = m_clusterList->item(i, NodePassword)->text();
        node.ip         = m_clusterList->item(i, NodeIP)->text();

        remainList.push_back(node);
    }
    int remainCount = remainList.count();
    m_clusterList->clearContents();
    m_clusterList->setRowCount(remainCount);

    QTableWidgetItem *item = NULL;
    for(int i = 0; i < remainCount; i++)
    {
        //node check
        item = new QTableWidgetItem();
        item->setCheckState(Qt::Unchecked);
        m_clusterList->setItem(i, NodeCheck, item);

        //node name
        item = new QTableWidgetItem();
        item->setText(remainList.at(i).name);
        m_clusterList->setItem(i, NodeName, item);

        //node username
        item = new QTableWidgetItem();
        item->setText(remainList.at(i).username);
        m_clusterList->setItem(i, NodeUserName, item);

        //node password
        item = new QTableWidgetItem();
        item->setText(remainList.at(i).password);
        m_clusterList->setItem(i, NodePassword, item);

        //node ip
        item = new QTableWidgetItem();
        item->setText(remainList.at(i).ip);
        m_clusterList->setItem(i, NodeIP, item);
    }
}

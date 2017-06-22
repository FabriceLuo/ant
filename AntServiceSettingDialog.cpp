#include "AntServiceSettingDialog.h"

#include <QPushButton>
#include <QLabel>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "AntSetting.h"

#include "AntServiceEditDialog.h"

AntServiceSettingDialog::AntServiceSettingDialog(QWidget *parent) : QWidget(parent)
{
    initWidget();
    initConnect();
    initDeploy();

    loadSetting();
}

void AntServiceSettingDialog::initWidget()
{
    m_addBtn = new QPushButton(tr("新增"));
    m_deleteBtn = new QPushButton(tr("删除"));
    m_modifyBtn = new QPushButton(tr("编辑"));

    m_serviceLabel = new QLabel(tr("服务列表"));

    m_serviceList = new QTableWidget();
    m_serviceHeader << "" << "名称" << "描述";
    m_serviceHeaderWidth << 24 << 150 << 300;
    setHorizontalHeader(m_serviceHeader);
    setHorizontalHeaderWidth(m_serviceHeaderWidth);
}

void AntServiceSettingDialog::initConnect()
{
    connect(m_addBtn, SIGNAL(clicked(bool)), this, SLOT(addService()));
    connect(m_modifyBtn, SIGNAL(clicked(bool)), this, SLOT(modifyService()));
    connect(m_deleteBtn, SIGNAL(clicked(bool)), this, SLOT(deleteService()));
}

void AntServiceSettingDialog::initDeploy()
{
    QVBoxLayout *mainLayout = new QVBoxLayout();
    QHBoxLayout *bodyLayout = new QHBoxLayout();
    QVBoxLayout *opLayout = new QVBoxLayout();

    opLayout->addWidget(m_addBtn);
    opLayout->addWidget(m_modifyBtn);
    opLayout->addWidget(m_deleteBtn);

    bodyLayout->addWidget(m_serviceList);
    bodyLayout->addLayout(opLayout);

    mainLayout->addWidget(m_serviceLabel);
    mainLayout->addLayout(bodyLayout);

    setLayout(mainLayout);
}

void AntServiceSettingDialog::loadSetting()
{
    AntSetting setting;
    if(! setting.getServiceList(m_list))
    {
        fprintf(stderr, "read setting failed");
        return;
    }
    showService();
}

void AntServiceSettingDialog::setHorizontalHeaderWidth(QList<int> width)
{
    QList<int>::Iterator begin = width.begin();
    QList<int>::Iterator end   = width.end();

    int i = 0;
    while(begin != end)
    {
        m_serviceList->setColumnWidth(i, *begin);
        begin++;
        i++;
    }
}

void AntServiceSettingDialog::setHorizontalHeader(QStringList header)
{
    m_serviceList->setColumnCount(header.count());
    QStringList::Iterator begin = header.begin();
    QStringList::iterator end   = header.end();

    int i = 0;
    while(begin != end)
    {
        QTableWidgetItem *item = new QTableWidgetItem();
        item->setText(*begin);

        m_serviceList->setHorizontalHeaderItem(i, item);
        begin++;
        i++;
    }
}

void AntServiceSettingDialog::showService()
{
    int serviceCount = m_list.count();
    m_serviceList->clearContents();
    m_serviceList->setRowCount(serviceCount);

    QTableWidgetItem *item = NULL;
    for(int i = 0; i < serviceCount; i++)
    {
        item = new QTableWidgetItem();
        item->setCheckState(Qt::Unchecked);
        item->setFlags(item->flags() & (~Qt::ItemIsEditable));
        m_serviceList->setItem(i, ServiceCheck, item);

        item = new QTableWidgetItem();
        item->setText(m_list.at(i).name);
        item->setFlags(item->flags() & (~Qt::ItemIsEditable));
        m_serviceList->setItem(i, ServiceName, item);

        item = new QTableWidgetItem();
        item->setText(m_list.at(i).description);
        item->setFlags(item->flags() & (~Qt::ItemIsEditable));
        m_serviceList->setItem(i, ServiceDescription, item);
    }
}

void AntServiceSettingDialog::addService()
{
    AntServiceEditDialog editDialog("", "", "", "", "", this);
    if(editDialog.exec() == QDialog::Accepted)
    {
        ClusterService service;
        service.name = editDialog.getServiceName();
        service.description = editDialog.getServiceDes();

        ClusterServiceCmd startCmd;
        startCmd.type = ClusterServiceStart;
        startCmd.command = editDialog.getServiceStartCmd();

        ClusterServiceCmd stopCmd;
        stopCmd.type = ClusterServiceStop;
        stopCmd.command = editDialog.getServiceStopCmd();

        ClusterServiceCmd restartCmd;
        restartCmd.type = ClusterServiceRestart;
        restartCmd.command = editDialog.getServiceRestartCmd();

        service.cmdList.push_back(startCmd);
        service.cmdList.push_back(stopCmd);
        service.cmdList.push_back(restartCmd);

        m_list.push_back(service);
    }

    showService();
}

void AntServiceSettingDialog::modifyService()
{
    ClusterService service;
    int modifyServiceIndex = 0, checkCount = 0;
    int serviceCount = m_serviceList->rowCount();
    QTableWidgetItem *item = NULL;

    for(int i = 0; i < serviceCount; i++)
    {
        item = m_serviceList->item(i, ServiceCheck);
        if(item->checkState() == Qt::Checked)
        {
            checkCount++;
            modifyServiceIndex = i;
        }
    }

    if(checkCount > 1)
    {
        //only one can be modify
    }
    else if(checkCount < 1)
    {
        //must select one
    }
    else {
        //service name
        QString serviceName = (m_serviceList->item(modifyServiceIndex, ServiceName))->text();
        int listCount = m_list.count();
        //find details info
        for(int i = 0; i < listCount; i++)
        {
            if(serviceName.compare(m_list.at(i).name) == 0)
            {
                service = m_list.at(i);
                break;
            }
        }

        QString serviceDes = service.description;

        QString startCmd, stopCmd, restartCmd;
        QList<ClusterServiceCmd> cmdList = service.cmdList;
        int cmdCount = cmdList.count();

        for(int i = 0; i < cmdCount; i++)
        {
            ClusterServiceCmd cmd = cmdList.at(i);
            switch (cmd.type) {
            case ClusterServiceStart:
                startCmd = cmd.command;
                break;
            case ClusterServiceStop:
                stopCmd = cmd.command;
                break;
            case ClusterServiceRestart:
                restartCmd = cmd.command;
            default:
                break;
            }
        }

        AntServiceEditDialog editDialog(serviceName, serviceDes, startCmd, stopCmd, restartCmd, this);
        if(editDialog.exec() == QDialog::Accepted)
        {
            ClusterService service;
            service.name = editDialog.getServiceName();
            service.description = editDialog.getServiceDes();

            ClusterServiceCmd startCmd;
            startCmd.type = ClusterServiceStart;
            startCmd.command = editDialog.getServiceStartCmd();

            ClusterServiceCmd stopCmd;
            stopCmd.type = ClusterServiceStop;
            stopCmd.command = editDialog.getServiceStopCmd();

            ClusterServiceCmd restartCmd;
            restartCmd.type = ClusterServiceRestart;
            restartCmd.command = editDialog.getServiceRestartCmd();

            service.cmdList.push_back(startCmd);
            service.cmdList.push_back(stopCmd);
            service.cmdList.push_back(restartCmd);

            m_list.replace(modifyServiceIndex, service);
        }

        showService();
    }
}

void AntServiceSettingDialog::deleteService()
{
    int listCount = m_serviceList->rowCount();
    QTableWidgetItem *item = NULL;
    for(int i = 0; i < listCount; i++)
    {
        item = m_serviceList->item(i, ServiceCheck);
        if(item->checkState() == Qt::Checked)
        {
            int serviceCount = m_list.count();

            QString serviceName = m_serviceList->item(i, ServiceName)->text();

            QList<ClusterService>::iterator begin = m_list.begin();
            QList<ClusterService>::iterator end = m_list.end();
            while(begin != end)
            {
                if(serviceName.compare(begin->name) == 0)
                {
                    begin = m_list.erase(begin);
                }
                else {
                    begin++;
                }
            }
        }
    }
    showService();
}

void AntServiceSettingDialog::saveSetting()
{
    AntSetting setting;
    if(! setting.setServiceList(m_list))
    {
        fprintf(stderr, "set cluster service failed");
        return;
    }
}

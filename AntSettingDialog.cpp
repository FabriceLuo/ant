#include "AntSettingDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

AntSettingDialog::AntSettingDialog(QWidget *parent, Qt::WindowFlags f):
    QDialog(parent, f),
    CLUSTER_TAB_NAME(tr("集群")),
    SEARCH_TAB_NAME(tr("代码搜索")),
    SERVICE_TAB_NAME(tr("集群服务"))

{
    setWindowTitle(tr("设置"));
    initWidget();
    initDepoly();
    initConnect();
}

void AntSettingDialog::initWidget()
{
    m_settingTab = new QTabWidget();

    m_clusterDialog = new AntClusterSettingDialog;
    m_searchDialog  = new AntSearchSettingDialog;
    m_serviceDialog = new AntServiceSettingDialog;

    m_okBtn = new QPushButton(tr("确定"));
    m_applyBtn = new QPushButton(tr("应用"));
}

void AntSettingDialog::initDepoly()
{
    m_settingTab->addTab(m_clusterDialog, CLUSTER_TAB_NAME);
    m_settingTab->addTab(m_searchDialog, SEARCH_TAB_NAME);
    m_settingTab->addTab(m_serviceDialog, SERVICE_TAB_NAME);

    QVBoxLayout *mainLayout = new QVBoxLayout();

    mainLayout->addWidget(m_settingTab);

    QHBoxLayout *tailLayout = new QHBoxLayout();
    tailLayout->addStretch();
    tailLayout->addWidget(m_okBtn);
    tailLayout->addWidget(m_applyBtn);

    mainLayout->addLayout(tailLayout);
    setLayout(mainLayout);
}

void AntSettingDialog::initConnect()
{
    connect(m_applyBtn, SIGNAL(clicked(bool)), this, SLOT(saveSetting()));
    connect(m_okBtn, SIGNAL(clicked(bool)), this, SLOT(saveSettingAndExit()));
}

void AntSettingDialog::saveSettingAndExit()
{
    saveSetting();
    QDialog::accept();
}

void AntSettingDialog::saveSetting()
{
    m_clusterDialog->saveSetting();
    m_searchDialog->saveSetting();
    m_serviceDialog->saveSetting();
}

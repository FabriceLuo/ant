#include "AntSettingDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

AntSettingDialog::AntSettingDialog(QWidget *parent, Qt::WindowFlags f):
    CLUSTER_TAB_NAME(tr("��Ⱥ")), QWidget(parent, f),
    SEARCH_TAB_NAME(tr("��������"))
{
    setWindowTitle(tr("����"));
    initWidget();
    initDepoly();
    initConnect();
}

void AntSettingDialog::initWidget()
{
    m_settingTab = new QTabWidget();

    m_clusterDialog = new AntClusterSettingDialog;
    m_searchDialog  = new AntSearchSettingDialog;

    m_okBtn = new QPushButton(tr("ȷ��"));
    m_applyBtn = new QPushButton(tr("Ӧ��"));
}

void AntSettingDialog::initDepoly()
{
    m_settingTab->addTab(m_clusterDialog, CLUSTER_TAB_NAME);
    m_settingTab->addTab(m_searchDialog, SEARCH_TAB_NAME);

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
    destroy();
}

void AntSettingDialog::saveSetting()
{
    m_clusterDialog->saveSetting();
    m_searchDialog->saveSetting();
}

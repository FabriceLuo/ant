#include "AntSettingDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

AntSettingDialog::AntSettingDialog(QWidget *parent, Qt::WindowFlags f):
    CLUSTER_TAB_NAME(tr("��Ⱥ")), QWidget(parent, f)
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

    m_okBtn = new QPushButton(tr("ȷ��"));
    m_applyBtn = new QPushButton(tr("Ӧ��"));
}

void AntSettingDialog::initDepoly()
{
    m_settingTab->addTab(m_clusterDialog, CLUSTER_TAB_NAME);

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
}

void AntSettingDialog::saveSettingAndExit()
{

}

void AntSettingDialog::saveSetting()
{
    QWidget *curWidget = m_settingTab->currentWidget();

    ((AntClusterSettingDialog*)curWidget)->saveSetting();
}

#include "AntServiceEditDialog.h"
#include "AntCommon.h"
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>

AntServiceEditDialog::AntServiceEditDialog(const QString &name,
                                           const QString &description,
                                           const QString &startCmd,
                                           const QString &stopCmd,
                                           const QString &restartCmd,
                                           QWidget *parent,
                                           Qt::WindowFlags f):
    m_name(name), m_des(description), m_start(startCmd), m_stop(stopCmd), m_restart(restartCmd)
{
    initWidget();
    initConnect();
    initDeploy();

    setWindowIcon(QIcon(":/icon/icon/ant_128px.ico"));
}

QString AntServiceEditDialog::getServiceName()
{
    return m_name;
}

QString AntServiceEditDialog::getServiceDes()
{
    return m_des;
}

QString AntServiceEditDialog::getServiceStartCmd()
{
    return m_start;
}

QString AntServiceEditDialog::getServiceStopCmd()
{
    return m_stop;
}

QString AntServiceEditDialog::getServiceRestartCmd()
{
    return m_restart;
}

void AntServiceEditDialog::initWidget()
{
    m_nameLabel = new QLabel(tr("名称"));
    m_desLabel  = new QLabel(tr("描述"));
    m_startLabel = new QLabel(tr("启动命令"));
    m_stopLabel = new QLabel(tr("停止命令"));
    m_restartLabel = new QLabel(tr("重启命令"));

    m_nameEdit = new QLineEdit(m_name);
    m_nameEdit->setFixedWidth(200);
    m_desEdit  = new QLineEdit(m_des);
    m_desEdit->setFixedWidth(200);
    m_startEdit = new QLineEdit(m_start);
    m_startEdit->setFixedWidth(200);
    m_stopEdit = new QLineEdit(m_stop);
    m_stopEdit->setFixedWidth(200);
    m_restartEdit = new QLineEdit(m_restart);
    m_restartEdit->setFixedWidth(200);

    m_yesBtn = new QPushButton(tr("确定"));
    m_yesBtn->setFixedWidth(60);
    m_noBtn  = new QPushButton(tr("取消"));
    m_noBtn->setFixedWidth(60);
}

void AntServiceEditDialog::initConnect()
{
    connect(m_yesBtn, SIGNAL(clicked(bool)), this, SLOT(confirmEdit()));
    connect(m_noBtn, SIGNAL(clicked(bool)), this, SLOT(cancelEdit()));
}

void AntServiceEditDialog::initDeploy()
{
    QVBoxLayout *mainLayout = new QVBoxLayout();

    QHBoxLayout *nameLayout = new QHBoxLayout();
    nameLayout->addWidget(m_nameLabel);
    nameLayout->addWidget(m_nameEdit);

    QHBoxLayout *desLayout      = new QHBoxLayout();
    desLayout->addWidget(m_desLabel);
    desLayout->addWidget(m_desEdit);

    QHBoxLayout *startLayout    = new QHBoxLayout();
    startLayout->addWidget(m_startLabel);
    startLayout->addWidget(m_startEdit);

    QHBoxLayout *stopLayout     = new QHBoxLayout();
    stopLayout->addWidget(m_stopLabel);
    stopLayout->addWidget(m_stopEdit);

    QHBoxLayout *restartLayout  = new QHBoxLayout();
    restartLayout->addWidget(m_restartLabel);
    restartLayout->addWidget(m_restartEdit);

    QHBoxLayout *opLayout = new QHBoxLayout();
    opLayout->addWidget(m_yesBtn);
    opLayout->addWidget(m_noBtn);

    mainLayout->addLayout(nameLayout);
    mainLayout->addLayout(desLayout);
    mainLayout->addLayout(startLayout);
    mainLayout->addLayout(stopLayout);
    mainLayout->addLayout(restartLayout);

    mainLayout->addLayout(opLayout);

    setLayout(mainLayout);
}

void AntServiceEditDialog::confirmEdit()
{
    //get latest info
    m_name = m_nameEdit->text();
    m_des  = m_desEdit->text();
    m_start = m_startEdit->text();
    m_stop = m_stopEdit->text();
    m_restart = m_restartEdit->text();

    QDialog::accept();
}

void AntServiceEditDialog::cancelEdit()
{
    QDialog::reject();
}

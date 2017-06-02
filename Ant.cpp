
#include "Ant.h"
#include <QTimerEvent>
#include <QIcon>
#include <QStringList>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QProcess>
#include <QString>
#include <QRegExp>
#include <QRegularExpression>
#include <QDir>
#include <QFileInfo>
#include <QTableWidget>
#include <QHeaderView>
#include <QTableWidgetItem>
#include "AntVersion.h"
#include "AntGit.h"
#include "AntSvn.h"
#include "AntFinder.h"
#include "AntCommander.h"
#include <QDateTime>
#include <QColor>
//for json handle
#include <QJsonDocument>
#include <QJsonObject>

#include "AntDiff.h"
#include "AntSync.h"
#include "AntClusterSetting.h"


Ant::Ant(QWidget *parent) :
    QMainWindow(parent),
    commander(NULL), m_diff(NULL)
{   
    m_headerTitle << tr("") << tr("文件名") << tr("本地路径") << tr("远程路径") << tr("修改时间") << tr("状态");
    m_headerWidth << 24 << 150 << 420 << 420 << 154 << 30;

    m_table = new QTableWidget();

    setHorizontalHeader(m_headerTitle);
    setHorizontalHeaderWidth(m_headerWidth);

    setFixedSize(1300, 800);

    setWindowIcon(QIcon(":/icon/icon/ant_128px.ico"));

    QHBoxLayout *handleLayout = new QHBoxLayout();
    QVBoxLayout *bodyLayout   = new QVBoxLayout();
    QVBoxLayout *mainLayout   = new QVBoxLayout();

    //初始化按钮
    m_addrEdit = new QLineEdit();
    m_addrEdit->setFixedWidth(220);

    m_addrLabel         = new QLabel("集群地址");
    m_updateButton      = new QPushButton("更新列表");
    m_syncButton        = new QPushButton("同步");

    m_versionAddrLabel  = new QLabel("代码目录");
    m_versionAddrEdit   = new QLineEdit();
    m_versionAddrEdit->setFixedWidth(400);

    m_userNameLabel = new QLabel("用户名");
    m_passwordLabel = new QLabel("密码");

    m_userNameEdit = new QLineEdit();
    m_passwordEdit = new QLineEdit();

    handleLayout->addWidget(m_addrLabel);
    handleLayout->addWidget(m_addrEdit);
    handleLayout->addWidget(m_versionAddrLabel);
    handleLayout->addWidget(m_versionAddrEdit);
    handleLayout->addWidget(m_userNameLabel);
    handleLayout->addWidget(m_userNameEdit);
    handleLayout->addWidget(m_passwordLabel);
    handleLayout->addWidget(m_passwordEdit);

    handleLayout->addWidget(m_updateButton);
    handleLayout->addWidget(m_syncButton);
    handleLayout->addStretch();

    bodyLayout->addWidget(m_table);

    mainLayout->addLayout(handleLayout);
    mainLayout->addLayout(bodyLayout);

    m_mainWidget = new QWidget();
    m_mainWidget->setLayout(mainLayout);
    setCentralWidget(m_mainWidget);

    connect(m_updateButton, SIGNAL(clicked(bool)), this, SLOT(showChangeList()));
    connect(m_syncButton, SIGNAL(clicked(bool)), this, SLOT(syncClusterCode()));
}

Ant::~Ant()
{

}

void Ant::timerEvent(QTimerEvent *event)
{

}

QList<VersionEntry> Ant::getChangeList(QString &path)
{
    AntVersion *version;
    VersionType type = getVersionType(path);
    switch (type) {
    case VersionTypeGit:
        version = new AntGit(path);
        break;
    case VersionTypeSvn:
        version = new AntSvn(path);
        break;
    default:
        break;
    }
    //free memory
    return version->getchangeList();
}

void Ant::setHorizontalHeader(QStringList header)
{
    m_table->setColumnCount(header.count());
    QStringList::Iterator begin = header.begin();
    QStringList::iterator end   = header.end();

    int i = 0;
    while(begin != end)
    {
        QTableWidgetItem *item = new QTableWidgetItem();
        item->setText(*begin);

        m_table->setHorizontalHeaderItem(i, item);
        begin++;
        i++;
    }
}

void Ant::setHorizontalHeaderWidth(QList<int> width)
{
    QList<int>::Iterator begin = width.begin();
    QList<int>::Iterator end   = width.end();

    int i = 0;
    while(begin != end)
    {
        m_table->setColumnWidth(i, *begin);
        begin++;
        i++;
    }
}

void Ant::showChangeList()
{
    loadClusterInfo();
    QString clusterIP = getMaster(), username = getUsername(), password = getPassword();
    //get code location
    m_versionPath = m_versionAddrEdit->text();
    QList<VersionEntry> changeList = getChangeList(m_versionPath);

    int count = changeList.count();
    setTableRowCount(count);
    QTableWidgetItem *item;
    //m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);


    if(clusterIP.isEmpty())
    {
        QMessageBox::warning(this, "Ant告警", "集群IP缺失");
        return;
    }
    if(commander == NULL)
    {
        commander = new AntCommander(clusterIP, username, password);
    }
    if(m_diff == NULL)
    {
        m_diff = new AntDiff();
    }
    AntFinder finder(commander);
    m_table->clearContents();
    for(int i=0; i < count; i++)
    {
        for(int j = 0; j <6; j++)
        {
            item = new QTableWidgetItem();
            m_table->setItem(i, j, item);
        }
        //check box
        m_table->item(i, VersionCheck)->setCheckState(Qt::Checked);

        //name
        item = m_table->item(i, VersionName);
        item->setText(changeList.at(i).name);
        item->setFlags(item->flags() & (~Qt::ItemIsEditable));

        //local path
        item = m_table->item(i, VersionLocalPath);
        item->setText(changeList.at(i).path);
        item->setFlags(item->flags() & (~Qt::ItemIsEditable));

        QFileInfo fileInfo(changeList.at(i).path);

        //find suitable des file
        if(fileInfo.exists())
        {
            //remoute path
            QStringList desList = finder.find(changeList.at(i).name);

            QString desFilePath = m_diff->findLatestDiff(changeList.at(i).path, desList, commander);
            if(! desFilePath.isEmpty())
            {
                item = m_table->item(i, VersionRemotePath);
                item->setText(desList.at(0));
            }
            else
            {
                m_table->item(i, VersionCheck)->setCheckState(Qt::Unchecked);
            }

            //last modify time
            item = m_table->item(i, VersionModifyTime);
            item->setText(fileInfo.lastModified().toString("HH:mm:ss MM-dd-yyyy"));
            item->setFlags(item->flags() & (~Qt::ItemIsEditable));
        }
        else {
            for(int j = 1; j <= 5; j++)
            {
                m_table->item(i, j)->setBackgroundColor(QColor(200, 200, 200));
            }
        }

        //version status
        item = m_table->item(i, VersionStatus);
        item->setText(AntCommon::getEntryStatusString(changeList.at(i).status));
        item->setFlags(item->flags() & (~Qt::ItemIsEditable));
    }
}

void Ant::syncClusterCode()
{
    if(!loadClusterInfo())
    {
        fprintf(stderr, "load cluster info failed");
        return;
    }
    QList<SyncEntry> fileList = getSyncFileList();

    QString master = getMaster(), username = getUsername(), password = getPassword();
    AntSync syncer(master, username, password);
    if(! syncer.syncToCluster(fileList))
    {
        QMessageBox::warning(this, "代码同步", "同步代码到集群各主机失败");
        return;
    }
}

void Ant::setTableRowCount(int count)
{
    m_table->setRowCount(count);
}

QList<SyncEntry> Ant::getSyncFileList()
{
    QList<SyncEntry> list;
    //get file num to sync
    int fileCount = m_table->rowCount();

    QString localPath, remoutePath;
    for(int i = 0; i < fileCount; i++)
    {
        //get check status
        if(Qt::Unchecked == m_table->item(i, 0)->checkState())
        {
            continue;
        }
        //get local file path
        localPath = m_table->item(i, VersionLocalPath)->text();
        //get remote file path
        remoutePath = m_table->item(i, VersionRemotePath)->text();
        if(remoutePath.isEmpty())
        {
            continue;
        }

        SyncEntry entry;
        entry.src = localPath;
        entry.des = remoutePath;

        list.push_back(entry);
    }

    return list;
}

bool Ant::loadClusterInfo()
{
    m_master = m_addrEdit->text();
    m_userName = m_userNameEdit->text();
    m_password = m_passwordEdit->text();

    return true;
}

QString Ant::getMaster()
{
    return m_master;
}

QString Ant::getUsername()
{
    return m_userName;
}

QString Ant::getPassword()
{
    return m_password;
}

VersionType Ant::getVersionType(QString path)
{
    return VersionTypeGit;
}


































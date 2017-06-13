
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
#include <QContextMenuEvent>
#include <QAction>
#include <QMenu>
#include <QComboBox>
#include <QFileDialog>
#include <QMenuBar>
#include <QDir>
#include <QDesktopServices>
#include "AntDiff.h"
#include "AntSync.h"

#include "AntSetting.h"
#include "AntSettingDialog.h"

Ant::Ant(QWidget *parent) :
    QMainWindow(parent),
    commander(NULL),
    m_diff(NULL)
{   
    m_curNode = NULL;

    initActions();
    initWidgets();
    initConnect();
    initClusterInfo();
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


    m_updateButton      = new QPushButton("更新列表");
    m_syncButton        = new QPushButton("同步");

    m_codeAddrLabel  = new QLabel("代码目录");
    m_codeAddrEdit   = new QLineEdit();
    m_codeAddrEdit->setFixedWidth(400);

    handleLayout->addWidget(m_clusterLabel);
    handleLayout->addWidget(m_clusterList);
    handleLayout->addWidget(m_codeAddrLabel);
    handleLayout->addWidget(m_codeAddrEdit);
    handleLayout->addWidget(m_verAddrSelect);

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

    initMenuBar();
}

Ant::~Ant()
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
    ClusterNode currentNode = getCurrentNode();
    QString codeDir = getCodeAddr();

    QList<VersionEntry> changeList = getChangeList(codeDir);

    if(commander == NULL && !currentNode.ip.isEmpty())
    {
        commander = new AntCommander(currentNode.ip, currentNode.username, currentNode.password);
        if(!commander->testConnect())
        {
            commander = NULL;
        }
    }
    if(m_diff == NULL)
    {
        m_diff = new AntDiff();
    }
    AntFinder finder(commander);
    m_table->clearContents();
    int count = changeList.count();
    setTableRowCount(count);
    QTableWidgetItem *item = NULL;
    for(int i=0; i < count; i++)
    {
        for(int j = 0; j <6; j++)
        {
            item = new QTableWidgetItem();
            m_table->setItem(i, j, item);
        }
        //check box
        m_table->item(i, VersionCheck)->setCheckState(Qt::Checked);
        m_table->item(i, VersionCheck)->setFlags(item->flags() & (~Qt::ItemIsEditable));

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
        if(fileInfo.exists() && commander != NULL)
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
        }
        else {
            for(int j = 1; j <= 5; j++)
            {
                m_table->item(i, j)->setBackgroundColor(QColor(200, 200, 200));
            }
            m_table->item(i, VersionCheck)->setCheckState(Qt::Unchecked);
        }
         //last modify time
        item = m_table->item(i, VersionModifyTime);
        item->setFlags(item->flags() & (~Qt::ItemIsEditable));
        if(fileInfo.exists())
        {
            item->setText(fileInfo.lastModified().toString("HH:mm:ss MM-dd-yyyy"));
        }

        //version status
        item = m_table->item(i, VersionStatus);
        item->setText(AntCommon::getEntryStatusString(changeList.at(i).status));
        item->setFlags(item->flags() & (~Qt::ItemIsEditable));
    }
}

void Ant::syncClusterCode()
{
    ClusterNode currentNode = getCurrentNode();

    QList<SyncEntry> fileList = getSyncFileList();
    AntSync syncer(currentNode.ip, currentNode.username, currentNode.password);
    if(! syncer.syncToCluster(fileList))
    {
        QMessageBox::warning(this, "代码同步", "同步代码到集群各主机失败");
        return;
    }
}

void Ant::showSettingDialog()
{
    AntSettingDialog settingDialog(this);
    settingDialog.exec();
    //reload cluster setting
    initClusterInfo();
}

void Ant::setTableRowCount(int count)
{
    m_table->setRowCount(count);
}

QString Ant::getCodeAddr()
{
    return m_codeAddrEdit->text();
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

ClusterNode Ant::getCurrentNode()
{
    //get select node
    QString nodeName = m_clusterList->currentText();
    ClusterNode currentNode;

    QList<ClusterNode> list;

    AntSetting setting;
    if(! setting.getNodeList(list))
    {
        fprintf(stderr, "get cluster setting failed");
        return currentNode;
    }

    QList<ClusterNode>::iterator begin = list.begin();
    QList<ClusterNode>::iterator end = list.end();
    while(begin != end)
    {
        if(begin->name.compare(nodeName) == 0)
        {
            currentNode = *begin;
            break;
        }
        begin++;
    }
    return currentNode;
}

void Ant::contextMenuEvent(QContextMenuEvent *event)
{
    QPoint pos = event->globalPos();
    m_mousePoint = pos;
    fprintf(stdout, "global x:%d,y:%d", pos.x(), pos.y());
    //QPoint centerPos = mapFrom(this, pos);
    pos = m_table->mapFromGlobal(pos);
    fprintf(stdout, "map x:%d,y:%d", pos.x(), pos.y());

    QTableWidgetItem *item = m_table->itemAt(pos);
    if(item == NULL)
    {
        return QMainWindow::contextMenuEvent(event);
    }
    int colum = item->column();
    if(colum < VersionCheck || colum > VersionStatus)
    {
        return QMainWindow::contextMenuEvent(event);
    }
    QMenu menu;
    switch (colum) {
    case VersionCheck:
        break;
    case VersionLocalPath:
        menu.addAction(m_editFileAction);
        menu.addAction(m_openDirAction);
        menu.addSeparator();
        menu.addAction(m_selectAllAction);
        menu.addAction(m_selectNoneAction);
        menu.addSeparator();
        menu.addAction(m_patchAction);
        menu.addAction(m_diffAction);
        menu.addAction(m_blameAction);
        break;
    case VersionRemotePath:
        break;
    case VersionModifyTime:
        break;
    case VersionStatus:
        break;
    default:
        break;
    }
    if(menu.isEmpty())
    {
        return QMainWindow::contextMenuEvent(event);
    }
    fprintf(stdout, "map x:%d,y:%d", QCursor::pos().x(), QCursor::pos().y());
    menu.exec(QCursor::pos());
}

void Ant::selectVersionDir()
{
    QString dir = QFileDialog::getExistingDirectory(this,
                                                    "选择代码路径",
                                                    ".",
                                                    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    m_codeAddrEdit->setText(dir);
}

void Ant::initClusterInfo()
{
    if(m_clusterList->count() > 0)
    {
        QString curCluster = m_clusterList->currentText();
        if(!curCluster.isEmpty())
        {
            m_curCluster = curCluster;
        }
    }
    m_clusterList->clear();
    //get setting info
    QList<ClusterNode> list;
    AntSetting setting;
    if(! setting.getNodeList(list))
    {
        fprintf(stderr, "get cluster setting failed");
        return;
    }
    //get old select location
    int listCount = list.count(), curIndex = -1;
    for(int i = 0; i < listCount; i++)
    {
        m_clusterList->addItem(list.at(i).name);
        if(! m_curCluster.isEmpty()
                && m_curCluster.compare(list.at(i).name) == 0
                && curIndex < 0)
        {
            curIndex = i;
        }
    }
    //set old select
    if(curIndex < 0 && list.count() > 0)
    {
        curIndex = 0;
    }
    if(curIndex >= 0)
    {
        m_clusterList->setCurrentIndex(curIndex);
    }
}

void Ant::openVersionFileLoc()
{
    QPoint pos = m_table->mapFromGlobal(m_mousePoint);
    QTableWidgetItem *item = m_table->itemAt(pos);

    QString path = item->text();
    if(! path.isEmpty())
    {
        QString dir = QFileInfo(path).path();
        if(! QDesktopServices::openUrl(dir))
        {
            fprintf(stderr, "open file dir failed");
            return;
        }
    }
}

void Ant::editVersionFile()
{
    QPoint pos = m_table->mapFromGlobal(m_mousePoint);
    QTableWidgetItem *item = m_table->itemAt(pos);

    QString path = item->text();
    if(! path.isEmpty())
    {
        if(! QDesktopServices::openUrl(path))
        {
            fprintf(stderr, "open file failed");
            return;
        }
    }
}

void Ant::selectAllVersion()
{
    int listCount = m_table->rowCount();
    for(int i = 0; i < listCount; i++)
    {
        m_table->item(i, VersionCheck)->setCheckState(Qt::Checked);
    }
}

void Ant::selectNoneVersion()
{
    int listCount = m_table->rowCount();
    for(int i = 0; i < listCount; i++)
    {
        m_table->item(i, VersionCheck)->setCheckState(Qt::Unchecked);
    }
}

void Ant::initActions()
{

    m_editFileAction    = new QAction("编辑文件");
    m_openDirAction     = new QAction("打开文件目录");

    m_selectAllAction   = new QAction("全选");
    m_selectNoneAction  = new QAction("全不选");

    m_patchAction       = new QAction("Patch");
    m_diffAction        = new QAction("Diff");
    m_blameAction       = new QAction("Blame");

    m_patchAction->setEnabled(false);
    m_diffAction->setEnabled(false);
    m_blameAction->setEnabled(false);

    m_settingAction     = new QAction("设置");

}

void Ant::initWidgets()
{
    m_clusterLabel = new QLabel("集群信息");
    m_clusterList = new QComboBox();

    m_verAddrSelect = new QPushButton("选择...");
    m_codeAddrLabel = new QLabel("代码路径");
    m_codeAddrEdit  = new QLineEdit();
    m_codeAddrEdit->setFixedWidth(220);

}

void Ant::initConnect()
{
    connect(m_verAddrSelect, SIGNAL(clicked(bool)), this, SLOT(selectVersionDir()));
    connect(m_settingAction, SIGNAL(triggered(bool)), this, SLOT(showSettingDialog()));

    connect(m_editFileAction, SIGNAL(triggered(bool)), this, SLOT(editVersionFile()));
    connect(m_openDirAction, SIGNAL(triggered(bool)), this, SLOT(openVersionFileLoc()));

    connect(m_selectAllAction, SIGNAL(triggered(bool)), this, SLOT(selectAllVersion()));
    connect(m_selectNoneAction, SIGNAL(triggered(bool)), this, SLOT(selectNoneVersion()));

}

void Ant::initDeploy()
{

}

void Ant::initMenuBar()
{
    m_toolMenu = new QMenu(tr("工具"));
    m_toolMenu->addAction(m_settingAction);

    menuBar()->addMenu(m_toolMenu);
}

VersionType Ant::getVersionType(QString path)
{
    //return VersionTypeGit;
    QDir dir(path);
    if(dir.exists(".git"))
    {
        return VersionTypeGit;
    }
    else if(dir.exists(".svn"))
    {
        return VersionTypeSvn;
    }
    else {
        return VersionTypeErr;
    }
}


































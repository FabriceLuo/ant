
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


Ant::Ant(QWidget *parent) :
    QMainWindow(parent),
    commander(NULL)
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

    handleLayout->addWidget(m_addrLabel);
    handleLayout->addWidget(m_addrEdit);
    handleLayout->addWidget(m_versionAddrLabel);
    handleLayout->addWidget(m_versionAddrEdit);
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
    //get code location
    m_versionPath = m_versionAddrEdit->text();
    m_versionPath = "D:\\Code\\ant";
    QList<VersionEntry> changeList = getChangeList(m_versionPath);

    int count = changeList.count();
    setTableRowCount(count);
    QTableWidgetItem *item;
    //m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QString clusterIP = m_addrEdit->text();
    clusterIP = "192.168.122.132";
    if(clusterIP.isEmpty())
    {
        QMessageBox::warning(this, "Ant告警", "集群IP缺失");
        return;
    }
    if(commander == NULL)
    {
        commander = new AntCommander(clusterIP, "mike", "081126");
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
        m_table->item(i, 0)->setCheckState(Qt::Checked);

        //name
        item = m_table->item(i, 1);
        item->setText(changeList.at(i).name);
        item->setFlags(item->flags() & (~Qt::ItemIsEditable));

        //local path
        item = m_table->item(i, 2);
        item->setText(changeList.at(i).path);
        item->setFlags(item->flags() & (~Qt::ItemIsEditable));

        //find suitable des file
        QStringList desList = finder.find(changeList.at(i).name);
        if(desList.count() > 0)
        {
            //remoute path
            item = m_table->item(i, 3);
            item->setText(desList.at(0));
            item->setFlags(item->flags() | Qt::ItemIsEditable);

        }

        //last modify time
        QFileInfo fileInfo(changeList.at(i).path);
        if(fileInfo.exists())
        {
            item = m_table->item(i, 4);
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
        m_table->item(i, 5)->setText(AntCommon::getEntryStatusString(changeList.at(i).status));
    }
}

void Ant::setTableRowCount(int count)
{
    m_table->setRowCount(count);
}

VersionType Ant::getVersionType(QString path)
{
    return VersionTypeGit;
}


































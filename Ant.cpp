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
#include "AntLog.h"

Ant::Ant(QWidget *parent)
    : QMainWindow(parent)
{
    m_headerTitle << "" << "文件名" << "本地路径" << "远程路径" << "修改时间";
    m_headerWidth << 24 << 150 << 420 << 420 << 154;
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

    m_addrLabel         = new QLabel("集群地址：");
    m_updateButton      = new QPushButton("更新列表");
    m_syncButton        = new QPushButton("同步");

    handleLayout->addWidget(m_addrLabel);
    handleLayout->addWidget(m_addrEdit);
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
    logger.linfo("xxxxxxxxxxxxxxxxxx");
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
    logger.linfo("xxxxxxxxxxxxxxxxxx");
    m_versionPath = m_addrEdit->text();
    QList<VersionEntry> changeList = getChangeList(m_versionPath);

    int count = changeList.count();
    setTableRowCount(count);
    QTableWidgetItem *item;
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    AntCommander *commander = new AntCommander("192.168.122.132", "mike", "081126");

    for(int i=0; i < count; i++)
    {
        item = new QTableWidgetItem();
        item->setCheckState(Qt::Checked);
        m_table->setItem(i, 0, item);

        m_table->setItem(i, 1, new QTableWidgetItem(changeList.at(i).name));
        m_table->setItem(i, 2, new QTableWidgetItem(changeList.at(i).path));

        //find suitable des file
        AntFinder finder(commander);
        QStringList desList = finder.find(changeList.at(i).name);
        if(desList.count() > 0)
        {
            m_table->setItem(i, 3, new QTableWidgetItem(desList.at(0)));
        }

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


































#include "AntSearchSettingDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

AntSearchSettingDialog::AntSearchSettingDialog()
{
    initWidget();
    initDeploy();
    initConnect();
    loadSetting();
}

void AntSearchSettingDialog::saveSetting()
{
    QStringList list;
    int listCount = m_searchList->rowCount();

    for(int i = 0; i < listCount; i++)
    {
        QString searchDir = m_searchList->item(i, SearchDir)->text();
        if(! searchDir.isEmpty())
        {
            list.push_back(searchDir);
        }
    }

    AntSetting setting = AntSetting();
    if(! setting.setGlobalSearchDir(list))
    {
        fprintf(stderr, "save search setting failed");
    }
}

void AntSearchSettingDialog::initWidget()
{
    m_searchLabel = new QLabel(tr("代码搜索路径"));
    m_addBtn = new QPushButton(tr("新增"));
    m_deleteBtn = new QPushButton(tr("删除"));

    m_searchList = new QTableWidget();
    m_searchHeader << "" << "搜索路径";
    setHorizontalHeader(m_searchHeader);
    m_searchHeaderWidth << 24 << 450;
    setHorizontalHeaderWidth(m_searchHeaderWidth);
}

void AntSearchSettingDialog::initConnect()
{
    connect(m_addBtn, SIGNAL(clicked(bool)), this, SLOT(addSearchDir()));
    connect(m_deleteBtn, SIGNAL(clicked(bool)), this, SLOT(deleteSearchDir()));
}

void AntSearchSettingDialog::initDeploy()
{
    QVBoxLayout *leftLayout = new QVBoxLayout();
    leftLayout->addWidget(m_addBtn);
    leftLayout->addWidget(m_deleteBtn);

    QHBoxLayout *bodyLayout = new QHBoxLayout();
    bodyLayout->addWidget(m_searchList);
    bodyLayout->addLayout(leftLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addWidget(m_searchLabel);
    mainLayout->addLayout(bodyLayout);


    setLayout(mainLayout);
}

void AntSearchSettingDialog::loadSetting()
{
    QStringList searchList;
    AntSetting setting = AntSetting();
    if(!setting.getGlobalSearchDir(searchList) || searchList.isEmpty())
    {
        return;
    }
    int listCount = searchList.count();
    m_searchList->clearContents();
    m_searchList->setRowCount(listCount);
    QTableWidgetItem *item = NULL;

    for(int i = 0; i < listCount; i++)
    {
        item = new QTableWidgetItem();
        item->setCheckState(Qt::Unchecked);
        m_searchList->setItem(i, SearchCheck, item);

        item = new QTableWidgetItem();
        item->setText(searchList.at(i));
        m_searchList->setItem(i, SearchDir, item);
    }
}

void AntSearchSettingDialog::setHorizontalHeaderWidth(QList<int> width)
{
    QList<int>::Iterator begin = width.begin();
    QList<int>::Iterator end   = width.end();

    int i = 0;
    while(begin != end)
    {
        m_searchList->setColumnWidth(i, *begin);
        begin++;
        i++;
    }
}

void AntSearchSettingDialog::setHorizontalHeader(QStringList header)
{
    m_searchList->setColumnCount(header.count());
    QStringList::Iterator begin = header.begin();
    QStringList::iterator end   = header.end();

    int i = 0;
    while(begin != end)
    {
        QTableWidgetItem *item = new QTableWidgetItem();
        item->setText(*begin);

        m_searchList->setHorizontalHeaderItem(i, item);
        begin++;
        i++;
    }
}

void AntSearchSettingDialog::addSearchDir()
{
    int listCount = m_searchList->rowCount();
    m_searchList->setRowCount(listCount + 1);
    QTableWidgetItem *item = NULL;

    item = new QTableWidgetItem();
    item->setCheckState(Qt::Unchecked);
    m_searchList->setItem(listCount, SearchCheck, item);

    item = new QTableWidgetItem();
    item->setText("");
    m_searchList->setItem(listCount, SearchDir, item);
}

void AntSearchSettingDialog::deleteSearchDir()
{
    QStringList remainList;
    int listCount = m_searchList->rowCount();
    QTableWidgetItem *item = NULL;
    for(int i = 0; i < listCount; i++)
    {
        item = m_searchList->item(i, SearchCheck);
        if(item->checkState() != Qt::Checked)
        {
            remainList.push_back(m_searchList->item(i, SearchDir)->text());
        }
    }

    listCount = remainList.count();
    m_searchList->clearContents();
    m_searchList->setRowCount(listCount);

    for(int i = 0; i < listCount; i++)
    {
        item = new QTableWidgetItem();
        item->setCheckState(Qt::Unchecked);
        m_searchList->setItem(i, SearchCheck, item);

        item = new QTableWidgetItem();
        item->setText(remainList.at(i));
        m_searchList->setItem(i, SearchDir, item);
    }
}

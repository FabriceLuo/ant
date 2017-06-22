#ifndef ANTSEARCHSETTINGDIALOG_H
#define ANTSEARCHSETTINGDIALOG_H
#include "AntCommon.h"
#include <QWidget>

#include <QLabel>
#include <QPushButton>
#include <QListWidget>
#include "AntSetting.h"
#include <QTableWidget>
#include <QTableWidgetItem>

enum SearchTableLoc {
    SearchTableLocBegin,
    SearchCheck = SearchTableLocBegin,
    SearchDir,
    SearchTableLocEnd = SearchDir
};

class AntSearchSettingDialog : public QWidget
{
    Q_OBJECT
public:
    AntSearchSettingDialog();
private:
    QPushButton *m_addBtn, *m_deleteBtn;
    QLabel *m_searchLabel;
    QTableWidget *m_searchList;
    QList<int> m_searchHeaderWidth;
    QStringList m_searchHeader;

    void initWidget();
    void initAction();
    void initConnect();
    void initDeploy();

    void loadSetting();

    void setHorizontalHeaderWidth(QList<int> width);
    void setHorizontalHeader(QStringList header);
public slots:
    void saveSetting();
private slots:
    void addSearchDir();
    void deleteSearchDir();
};

#endif // ANTSEARCHSETTINGDIALOG_H

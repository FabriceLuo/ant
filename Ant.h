#ifndef ANT_H
#define ANT_H

#include <QMainWindow>
#include <QString>
#include "AntCommon.h"

class QTableWidget;
class QHeaderView;
class QTimerEvent;
class QPushButton;
class QLineEdit;
class QLabel;
class QWidget;
class VersionEntry;

enum SvnEntryStatus
{
    SvnEntryError,
    SvnEntryAdd,
    SvnEntryModify,
    SvnEntryDelete
};

class SvnEntry
{
public:
    QString path;
    QString name;
    int64_t size;
    SvnEntryStatus status;
};

class Ant : public QMainWindow
{
    Q_OBJECT

public:
    Ant(QWidget *parent = 0);
    QTableWidget *m_table;
    QHeaderView *m_headerView;
    int m_timerID;
    void timerEvent(QTimerEvent *event);
    ~Ant();

private:
    QStringList m_headerTitle = {
        "",
        "文件名",
        "本地路径",
        "远程路径",
        "修改时间"
    };
    QList<int> m_headerWidth = {
        24,
        150,
        420,
        420,
        154
    };
    //private var for components
    QString m_versionPath;

    QPushButton *m_updateButton, *m_syncButton;
    QLineEdit  *m_addrEdit;
    QLabel     *m_addrLabel;
    QWidget    *m_mainWidget;

    QString m_svnAddr;

    QList<VersionEntry> getChangeList(QString &path);
    void setHorizontalHeader(QStringList header);
    void setHorizontalHeaderWidth(QList<int> width);
    VersionType getVersionType(QString path);


    void setTableRowCount(int count);

private slots:
    void showChangeList();
};


#endif // ANT_H

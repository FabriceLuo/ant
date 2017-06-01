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
class AntCommander;
class AntDiff;

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
    int size;
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
    QStringList m_headerTitle;

    QList<int> m_headerWidth;
	
    //private var for components
    QString m_versionPath;
    QString m_masterAddr;

    QPushButton *m_updateButton, *m_syncButton;
    QLineEdit  *m_addrEdit;
    QLabel     *m_addrLabel;
    QWidget    *m_mainWidget;

    QLabel     *m_versionAddrLabel;
    QLineEdit  *m_versionAddrEdit;

    QString m_svnAddr;

    AntCommander *commander;
    AntDiff *m_diff;

    QList<VersionEntry> getChangeList(QString &path);
    void setHorizontalHeader(QStringList header);
    void setHorizontalHeaderWidth(QList<int> width);
    VersionType getVersionType(QString path);


    void setTableRowCount(int count);
    QList<SyncEntry> getSyncFileList();

    QString m_master, m_userName, m_password;
    bool loadClusterInfo();
    QString getMaster();
    QString getUsername();
    QString getPassword();

    QLineEdit *m_userNameEdit, *m_passwordEdit;
    QLabel *m_userNameLabel, *m_passwordLabel;


private slots:
    void showChangeList();
    void syncClusterCode();
};


#endif // ANT_H

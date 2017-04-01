#ifndef ANT_H
#define ANT_H

#include <QMainWindow>
#include <QString>

class CheckboxTable;
class CheckboxHeaderView;
class QTimerEvent;
class QPushButton;
class QLineEdit;
class QLabel;
class QWidget;

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
    CheckboxTable *m_table;
    CheckboxHeaderView *m_headerView;
    int m_timerID;
    void timerEvent(QTimerEvent *event);
    ~Ant();

private:
    QStringList m_headerTitle = {
        "文件名",
        "本地路径",
        "远程路径",
        "修改时间"
    };
    QList<int> m_headerWidth = {
        150,
        420,
        420,
        154
    };
    QPushButton *m_updateButton, *m_syncButton;
    QLineEdit  *m_addrEdit;
    QLabel     *m_addrLabel;
    QWidget    *m_mainWidget;

    QString m_svnAddr;

    QList<SvnEntry> getSvnChangeList(QString &path);

private slots:
    void syncSvnChangeFile();

};


#endif // ANT_H

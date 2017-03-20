#ifndef ANT_H
#define ANT_H

#include <QMainWindow>

class CheckboxTable;
class CheckboxHeaderView;
class QTimerEvent;

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
};

#endif // ANT_H

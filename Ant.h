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
};

#endif // ANT_H

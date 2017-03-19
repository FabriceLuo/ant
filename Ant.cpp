#include "Ant.h"

#include "CheckboxTable.h"
#include "CheckboxHeaderView.h"
#include <QTimerEvent>
#include <QIcon>
#include <QStringList>

Ant::Ant(QWidget *parent)
    : QMainWindow(parent)
{
    m_table = new CheckboxTable(10, 4);

    setCentralWidget(m_table);
    setFixedSize(1200, 800);

    setWindowIcon(QIcon(":/icon/icon/ant_128px.ico"));

}

Ant::~Ant()
{

}

void Ant::timerEvent(QTimerEvent *event)
{

}

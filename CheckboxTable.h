#ifndef CHECKBOXTABLE_H
#define CHECKBOXTABLE_H

#include <QTableWidget>

#include "CheckboxHeaderView.h"
#include <QList>

class CheckboxHeaderView;
class QCheckBox;

class CheckboxTable : public QTableWidget
{
Q_OBJECT
public:
    CheckboxTable(int rows, int columns, int checkIndex = 0, QWidget *parent = Q_NULLPTR);
private:
    CheckboxHeaderView *m_tableHeader;
public slots:
    void setCheckboxsStatus(CheckboxStatus status);
    void updateHeaderCheckStatus(int status);
private:
    int m_checkIndex;
    int m_rows;
    int m_columns;

    QList<QCheckBox*> m_checkList;
};

#endif // CHECKBOXTABLE_H

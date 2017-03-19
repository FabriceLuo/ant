#include "CheckboxTable.h"
#include "CheckboxHeaderView.h"
#include <QCheckBox>
#include <QHBoxLayout>

CheckboxTable::CheckboxTable(int rows, int columns, int checkIndex, QWidget *parent):
    QTableWidget(rows, columns, parent), m_checkIndex(checkIndex), m_rows(rows), m_columns(columns)
{
    m_tableHeader = new CheckboxHeaderView(Qt::Horizontal, m_checkIndex);
    //m_tableHeader->set
    setHorizontalHeader(m_tableHeader);

    setColumnWidth(m_checkIndex, 32);
    for(int i =0; i< m_rows; i++)
    {
        QCheckBox *box = new QCheckBox();
        QHBoxLayout *layout = new QHBoxLayout();
        layout->addWidget(box, 0, Qt::AlignHCenter);

        QWidget *widget = new QWidget();
        widget->setLayout(layout);

        setCellWidget(i, m_checkIndex, widget);
        setRowHeight(i, 32);

        m_checkList.push_back(box);

        connect(box, SIGNAL(stateChanged(int)), this, SLOT(updateHeaderCheckStatus(int)));
    }

    connect(m_tableHeader, SIGNAL(checkStatusChanged(CheckboxStatus)), this, SLOT(setCheckboxsStatus(CheckboxStatus)));
}

void CheckboxTable::setCheckboxsStatus(CheckboxStatus status)
{
    QList<QCheckBox*>::Iterator begin = m_checkList.begin();
    QList<QCheckBox*>::iterator end   = m_checkList.end();

    while(begin != end)
    {
        if(status == CheckboxNoneChecked)
        {
            (*begin)->setCheckState(Qt::Unchecked);
        }
        else {
            (*begin)->setCheckState(Qt::Checked);
        }
        begin++;
    }
}

void CheckboxTable::updateHeaderCheckStatus(int status)
{
    QList<QCheckBox*>::Iterator begin = m_checkList.begin();
    QList<QCheckBox*>::iterator end   = m_checkList.end();
    int checkedCount = 0;
    while(begin != end)
    {
        if((*begin)->isChecked())
        {
            checkedCount++;
        }
        begin++;
    }
    CheckboxStatus headerBoxStatus;
    if(checkedCount == m_rows)
    {
        headerBoxStatus = CheckboxAllChecked;
    }
    else if(checkedCount == 0)
    {
        headerBoxStatus = CheckboxNoneChecked;
    }
    else {
        headerBoxStatus = CheckboxSomeChecked;
    }
    m_tableHeader->setCheckStatus(headerBoxStatus);
}



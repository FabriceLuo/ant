#include "CheckboxHeaderView.h"
#include <QHeaderView>
#include <QPoint>
#include <QPainter>
#include <QColor>
#include <QBrush>
#include <QPen>

CheckboxHeaderView::CheckboxHeaderView(Qt::Orientation orientation, int checkIndex, QWidget *parent):
    QHeaderView(orientation, parent), m_checkIndex(checkIndex), m_checkStatus(CheckboxNoneChecked), m_checkSize(16)
{
    setSectionsClickable(true);
    connect(this, SIGNAL(sectionClicked(int)), this, SLOT(updateSection(int)));
}

void CheckboxHeaderView::paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const
{
    if(logicalIndex == m_checkIndex)
    {
        switch (m_checkStatus) {
        case CheckboxNoneChecked:
            paintNoneChecked(painter, rect);
            break;
        case CheckboxSomeChecked:
            paintSomeChecked(painter, rect);
            break;
        case CheckboxAllChecked:
            paintAllChecked(painter, rect);
            break;
        default:
            break;
        }
    }
}

void CheckboxHeaderView::paintAllChecked(QPainter *painter, const QRect &rect) const
{
    QPoint center(rect.left() /2 + rect.right() /2, rect.top() /2 + rect.bottom() /2);

    QPoint firstPoint, secondPoint, thirdPoint;
    //画框
    QRect checkBoxRect(center.x() - m_checkSize /2, center.y() - m_checkSize /2, m_checkSize, m_checkSize);
    painter->drawRect(checkBoxRect);

    QPen tickPen(Qt::SolidLine);
    tickPen.setWidth(2);
    tickPen.setCapStyle(Qt::SquareCap);
    tickPen.setJoinStyle(Qt::MiterJoin);

    painter->setRenderHint(QPainter::Antialiasing, true);

    painter->setPen(tickPen);
    firstPoint.setX(checkBoxRect.left() + 4);
    firstPoint.setY(center.y() + 2);

    secondPoint.setX(center.x() -1);
    secondPoint.setY(checkBoxRect.bottom() -2);

    thirdPoint.setX(checkBoxRect.right() - 2);
    thirdPoint.setY(checkBoxRect.top() + 6);

    painter->drawLine(firstPoint, secondPoint);
    painter->drawLine(secondPoint, thirdPoint);
}

void CheckboxHeaderView::paintSomeChecked(QPainter *painter, const QRect &rect) const
{
    QPoint center(rect.left() /2 + rect.right() /2, rect.top() /2 + rect.bottom() /2);

    QRect checkBoxRect(center.x() - m_checkSize /2, center.y() - m_checkSize /2, m_checkSize, m_checkSize);
    QRect checkBoxCenter(center.x() - m_checkSize /2 + 3, center.y() - m_checkSize /2 + 3, m_checkSize - 6, m_checkSize -6);

    painter->setBrush(QBrush(Qt::SolidPattern));
    painter->drawRect(checkBoxCenter);
    painter->setBrush(QBrush(Qt::NoBrush));
    painter->drawRect(checkBoxRect);
}

void CheckboxHeaderView::paintNoneChecked(QPainter *painter, const QRect &rect) const
{
    QPoint center(rect.left() /2 + rect.right() /2, rect.top() /2 + rect.bottom() /2);

    QRect checkBoxRect(center.x() - m_checkSize /2, center.y() - m_checkSize /2, m_checkSize, m_checkSize);
    painter->drawRect(checkBoxRect);
}

void CheckboxHeaderView::updateSection(int logicalIndex)
{
    if(m_checkIndex == logicalIndex)
    {
        if(m_checkStatus == CheckboxAllChecked)
        {
            m_checkStatus = CheckboxNoneChecked;
        }
        else {
            m_checkStatus = CheckboxAllChecked;
        }
        emit checkStatusChanged(m_checkStatus);
        update();
    }
}

void CheckboxHeaderView::setCheckStatus(CheckboxStatus status)
{
    if(status != m_checkStatus)
    {
        m_checkStatus = status;
        repaint();
    }

}

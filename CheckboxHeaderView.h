#ifndef CHECKBOXHEADERVIEW_H
#define CHECKBOXHEADERVIEW_H
#include <QHeaderView>


enum CheckboxStatus {
    CheckboxNoneChecked,
    CheckboxSomeChecked,
    CheckboxAllChecked
};
class CheckboxHeaderView : public QHeaderView
{
    Q_OBJECT
public:
    CheckboxHeaderView(Qt::Orientation orientation, int checkIndex, QWidget *parent = Q_NULLPTR);
    void setCheckStatus(CheckboxStatus status);
protected:
    void paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const;
    void paintAllChecked(QPainter *painter, const QRect &rect) const;
    void paintSomeChecked(QPainter *painter, const QRect &rect) const;
    void paintNoneChecked(QPainter *painter, const QRect &rect) const;

protected slots:
    void updateSection(int logicalIndex);
private:
    int m_checkIndex;
    CheckboxStatus m_checkStatus;
    int m_checkSize;

signals:
    void checkStatusChanged(CheckboxStatus status);
};



#endif // CHECKBOXHEADERVIEW_H

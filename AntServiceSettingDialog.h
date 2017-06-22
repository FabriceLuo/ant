#ifndef ANTSERVICESETTINGDIALOG_H
#define ANTSERVICESETTINGDIALOG_H

#include <QWidget>
#include "AntCommon.h"

class QTableWidget;
class QPushButton;
class QLabel;

enum ServiceTableLoc {
    ServiceTableLocBegin,
    ServiceCheck = ServiceTableLocBegin,
    ServiceName,
    ServiceDescription,
    ServiceTableLocEnd = ServiceDescription
};

class AntServiceSettingDialog : public QWidget
{
    Q_OBJECT
public:
    explicit AntServiceSettingDialog(QWidget *parent = 0);
private:
    QTableWidget *m_serviceList;
    QPushButton *m_addBtn, *m_deleteBtn, *m_modifyBtn;
    QLabel *m_serviceLabel;
    QStringList m_serviceHeader;
    QList<int> m_serviceHeaderWidth;
    QList<ClusterService> m_list;
    void initWidget();
    void initAction();
    void initConnect();
    void initDeploy();

    void loadSetting();

    void setHorizontalHeaderWidth(QList<int> width);
    void setHorizontalHeader(QStringList header);

    void showService();
private slots:
    void addService();
    void modifyService();
    void deleteService();
public slots:
    void saveSetting();
};

#endif // ANTSERVICESETTINGDIALOG_H

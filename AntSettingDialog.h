#ifndef ANTSETTINGDIALOG_H
#define ANTSETTINGDIALOG_H

#include <QWidget>
#include <QTabWidget>
#include <QDialog>
#include "AntClusterSettingDialog.h"
#include "AntSearchSettingDialog.h"
#include "AntServiceSettingDialog.h"
#include "AntCommon.h"
class AntSettingDialog : public QDialog
{
    Q_OBJECT
public:
    AntSettingDialog(QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
private:
    QTabWidget *m_settingTab;

    const QString CLUSTER_TAB_NAME, SEARCH_TAB_NAME, SERVICE_TAB_NAME;
    AntClusterSettingDialog *m_clusterDialog;
    AntSearchSettingDialog  *m_searchDialog;
    AntServiceSettingDialog *m_serviceDialog;

    QPushButton *m_okBtn, *m_applyBtn;

    void initWidget();

    void initAction();
    void initDepoly();
    void initConnect();
private slots:
    void saveSettingAndExit();
    void saveSetting();
};

#endif // ANTSETTINGDIALOG_H

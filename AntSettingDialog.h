#ifndef ANTSETTINGDIALOG_H
#define ANTSETTINGDIALOG_H

#include <QWidget>
#include <QTabWidget>
#include "AntClusterSettingDialog.h"
#include "AntSearchSettingDialog.h"
#include "AntCommon.h"
class AntSettingDialog : public QWidget
{
    Q_OBJECT
public:
    AntSettingDialog(QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
private:
    QTabWidget *m_settingTab;

    const QString CLUSTER_TAB_NAME, SEARCH_TAB_NAME;
    AntClusterSettingDialog *m_clusterDialog;
    AntSearchSettingDialog  *m_searchDialog;

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

#ifndef ANTCLUSTERSETTINGDIALOG_H
#define ANTCLUSTERSETTINGDIALOG_H
#include <QWidget>

#include <QLabel>
#include <QComboBox>
#include <QTableWidget>
#include <QPushButton>
#include <QHeaderView>
#include <QStringList>


enum NodeTableLoc {
    NodeTableLocBegin,
    NodeCheck = NodeTableLocBegin,
    NodeName,
    NodeUserName,
    NodePassword,
    NodeIP,
    NodeTableLocEnd = NodeIP
};

class AntClusterSettingDialog : public QWidget
{
    Q_OBJECT
public:
    AntClusterSettingDialog();

private:
    QLabel      *m_listLabel;
    QPushButton *m_addBtn;
    QPushButton *m_deleteBtn;

    QTableWidget    *m_clusterList;
    QStringList     m_clusterHeader;
    QList<int>      m_clusterHeaderWidth;

    void initWidget();
    void initAction();
    void initConnect();
    void initDeploy();

    void setHorizontalHeader(QStringList header);
    void setHorizontalHeaderWidth(QList<int> width);

    void loadSetting();
public:
    void saveSetting();

private slots:
    void addClusterNode();
    void deleteClusterNode();
};

#endif // ANTCLUSTERSETTINGDIALOG_H

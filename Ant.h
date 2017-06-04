#ifndef ANT_H
#define ANT_H

#include <QMainWindow>
#include <QString>
#include "AntCommon.h"

class QTableWidget;
class QHeaderView;
class QTimerEvent;
class QPushButton;
class QLineEdit;
class QLabel;
class QWidget;
class VersionEntry;
class AntCommander;
class AntDiff;
class QAction;
class QComboBox;
class QTabWidget;
class QMenuBar;
class QMenu;



class Ant : public QMainWindow
{
    Q_OBJECT

public:
    Ant(QWidget *parent = 0);
    QTableWidget *m_table;
    QHeaderView *m_headerView;
    ~Ant();

private:
    QStringList                 m_headerTitle;
    QList<int>                  m_headerWidth;
	
    //private var for components
    QString                     m_versionPath;
    QString                     m_masterAddr;

    QPushButton *m_updateButton, *m_syncButton;

    QWidget    *m_mainWidget;

    AntCommander *commander;
    AntDiff *m_diff;

    VersionType getVersionType(QString path);

    void setHorizontalHeader(QStringList header);
    void setHorizontalHeaderWidth(QList<int> width);


    QList<VersionEntry> getChangeList(QString &path);
    QList<SyncEntry> getSyncFileList();

    //
    ClusterNode *m_curNode;
    ClusterNode getCurrentNode();

    //reiplemented event
    void contextMenuEvent(QContextMenuEvent *event);

    void setTableRowCount(int count);

    //context actions
    //file op
    QAction *m_openDirAction;
    QAction *m_editFileAction;
    //select
    QAction *m_selectAllAction;
    QAction *m_selectNoneAction;
    //version control
    QAction *m_patchAction;
    QAction *m_diffAction;
    QAction *m_blameAction;

    //cluster info
    QComboBox *m_clusterList;
    QLabel *m_clusterLabel;

    //version info
    QLabel     *m_codeAddrLabel;
    QLineEdit  *m_codeAddrEdit;
    QPushButton *m_verAddrSelect;
    QString getCodeAddr();

    //setting dialog
    QTabWidget *settingWidget;
    QWidget *clusterSettingWidget;
    QWidget *versionSettingWidget;
    QWidget *searchSettingWidget;

    QAction *m_settingAction;


private slots:
    void selectVersionDir();
private:
    void initActions();
    void initWidgets();

    void initConnect();

    void initDeploy();
    void initMenuBar();


    QMenuBar *m_menuBar;
    QMenu *m_toolMenu;


private slots:
    void showChangeList();
    void syncClusterCode();
    void showSettingDialog();
    void initClusterInfo();
};


#endif // ANT_H

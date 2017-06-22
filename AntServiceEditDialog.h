#ifndef ANTSERVICEEDITDIALOG_H
#define ANTSERVICEEDITDIALOG_H

#include <QDialog>
class QLabel;
class QLineEdit;
class QPushButton;

class AntServiceEditDialog : public QDialog
{
    Q_OBJECT
public:
    AntServiceEditDialog(const QString &name,
                         const QString &description,
                         const QString &startCmd,
                         const QString &stopCmd,
                         const QString &restartCmd,
                         QWidget *parent = Q_NULLPTR,
                         Qt::WindowFlags f = Qt::WindowFlags());

    QString getServiceName();
    QString getServiceDes();
    QString getServiceStartCmd();
    QString getServiceStopCmd();
    QString getServiceRestartCmd();

private:
    QLabel *m_nameLabel, *m_desLabel, *m_startLabel, *m_stopLabel, *m_restartLabel;
    QString m_name, m_des, m_start, m_stop, m_restart;
    QLineEdit *m_nameEdit, *m_desEdit, *m_startEdit, *m_stopEdit, *m_restartEdit;
    QPushButton *m_yesBtn, *m_noBtn;
    void initWidget();
    void initConnect();
    void initDeploy();

private slots:
    void confirmEdit();
    void cancelEdit();
};

#endif // ANTSERVICEEDITDIALOG_H

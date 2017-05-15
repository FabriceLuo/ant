#ifndef ANTCOMMANDER_H
#define ANTCOMMANDER_H

#include <QString>
#include <QStringList>
#include <libssh2.h>


class AntCommander
{
public:
    AntCommander();
    AntCommander(const QString &host, const QString &username, const QString &password);

    void init(const QString &host, const QString &username, const QString &password);

    int exec(const QString &cmd, const QStringList &param, QString &std_out, QString &std_err);
    bool scpTo(const QString &src, const QString &des);
    bool scpFrom(const QString &src, const QString &des);

private:
    LIBSSH2_SESSION *m_session;
    LIBSSH2_CHANNEL *m_channel;
    int m_socket;
    QString m_host, m_username, m_password;

    bool checkConnect();
    bool connect(const QString &host, const QString &username, const QString &password);
    int waitsocket(int socket_fd, LIBSSH2_SESSION *session);
};

#endif // ANTCOMMANDER_H

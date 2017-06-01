#ifndef ANTCOMMANDER_H
#define ANTCOMMANDER_H

#include <QString>
#include <QStringList>
#include <QMessageBox>

#include <libssh2.h>
#include "libssh2_config.h"

#ifdef HAVE_WINSOCK2_H
# include <winsock2.h>
#endif
#ifdef HAVE_WINSOCK2_H
# include <winsock2.h>
#endif
#ifdef HAVE_SYS_SOCKET_H
# include <sys/socket.h>
#endif
#ifdef HAVE_NETINET_IN_H
# include <netinet/in.h>
#endif
#ifdef HAVE_SYS_SELECT_H
# include <sys/select.h>
#endif
# ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef HAVE_ARPA_INET_H
# include <arpa/inet.h>
#endif
#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif
#include <sys/types.h>
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <ctype.h>
class AntCommander
{
public:
    AntCommander();
    AntCommander(const QString &host, const QString &username, const QString &password);

    void init(const QString &host, const QString &username, const QString &password);

    int exec(const QString &cmd, const QStringList &param, QString &stdOut, QString &stdErr);
    bool scpTo(const QString &src, const QString &des);
    bool scpFrom(const QString &src, const QString &des);

private:
    LIBSSH2_SESSION *m_session;
    LIBSSH2_CHANNEL *m_channel;
    int m_socket;
    QString m_host, m_username, m_password;

    bool checkConnect();
    bool hostConnect(const QString &host, const QString &username, const QString &password);
    int waitsocket(int socket_fd, LIBSSH2_SESSION *session);
};

#endif // ANTCOMMANDER_H

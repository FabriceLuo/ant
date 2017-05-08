#ifndef ANTFINDER_H
#define ANTFINDER_H

#include <libssh2.h>
#include <QString>

class AntFinder
{
public:
    AntFinder();

    bool connect(const QString &host, const QString &username, const QString &password);
    QString find(const QString &path);

private:
    LIBSSH2_SESSION *m_session;
    LIBSSH2_CHANNEL *m_channel;
    int m_socket;

    bool checkConnect();
};

#endif // ANTFINDER_H

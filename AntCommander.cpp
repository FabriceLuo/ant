#include "AntCommander.h"
#include <QStringList>
#include <QString>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>

AntCommander::AntCommander()
{

}

AntCommander::AntCommander(const QString &host, const QString &username, const QString &password):
    m_host(host), m_username(username), m_password(password)
{
}

void AntCommander::init(const QString &host, const QString &username, const QString &password)
{
    m_host      = host;
    m_username  = username;
    m_password  = password;
}

bool AntCommander::connect(const QString &host, const QString &username, const QString &password)
{
    int rc;
    //init session
    if(m_session == NULL)
    {
        m_session = libssh2_session_init();
        if(m_session == NULL)
        {
            printf("init libssh2 session error");
            return false;
        }
        libssh2_session_set_blocking(m_session, 0);
    }

    if(m_socket < 0)
    {
        //create socket for libssh2
        m_socket = socket(AF_INET, SOCK_STREAM, 0);
        if(m_socket < 0)
        {
            printf("create socket for libssh2 failed");
            return false;
        }
    }

    while ((rc = libssh2_session_handshake(m_session, m_socket)) == LIBSSH2_ERROR_EAGAIN);
    if(rc)
    {
        printf("establish ssh session to server failed");
        return false;
    }

    //auth
    while ((rc = libssh2_userauth_password(m_session, username.toLatin1().data(), password.toLatin1().data())) == LIBSSH2_ERROR_EAGAIN);
    if(rc)
    {
        printf("auth to server failed");
        return false;
    }

    //open channel
    while((m_channel = libssh2_channel_open_session(m_session)) == NULL && libssh2_session_last_error(m_session, NULL, NULL, 0) == LIBSSH2_ERROR_EAGAIN);
    if(m_channel == NULL)
    {
        printf("open exec channel to server failed");
        return false;
    }

    return true;
}

bool AntCommander::checkConnect()
{
    if(m_session == NULL)
    {
        return connect(m_host, m_username, m_password);
    }
    return true;
}

int AntCommander::exec(const QString &cmd, const QStringList &param, QString &std_out, QString &std_err)
{
    if(!checkConnect())
    {
        return -1;
    }
    int rc;
    QString cmd_str=cmd;
    cmd_str.append(param.join(' '));
    char *exec_cmd = cmd_str.toLatin1().data();

    while ((rc = libssh2_channel_exec(m_channel, exec_cmd)) == LIBSSH2_ERROR_EAGAIN) {
        waitsocket(m_socket, m_session);
    }
    if(rc != 0)
    {
        fprintf(stderr, "error\n");
        return -1;
    }

    char buffer[2048];
    //read std out
    while (1) {
        int rc;
        do
        {
            rc = libssh2_channel_read(m_channel, buffer, sizeof(buffer));
            if(rc > 0)
            {
                std_out.append(buffer);
            }
            else
            {
                if(rc != LIBSSH2_ERROR_EAGAIN)
                {
                    fprintf(stderr, "libssh2 channel returned");
                }
            }
        }
        while(rc > 0);

        if(rc == LIBSSH2_ERROR_EAGAIN)
        {
            waitsocket(m_socket, m_session);
        }
        else
        {
            break;
        }
    }

    //read std err
    while (1) {
        int rc;
        do
        {
            rc = libssh2_channel_read_stderr(m_channel, buffer, sizeof(buffer));
            if(rc > 0)
            {
                std_err.append(buffer);
            }
            else
            {
                if(rc != LIBSSH2_ERROR_EAGAIN)
                {
                    fprintf(stderr, "libssh2 channel returned");
                }
            }
        }
        while(rc > 0);

        if(rc == LIBSSH2_ERROR_EAGAIN)
        {
            waitsocket(m_socket, m_session);
        }
        else
        {
            break;
        }
    }

    while ((rc = libssh2_channel_close(m_channel)) == LIBSSH2_ERROR_EAGAIN) {
        waitsocket(m_socket, m_session);
    }
    if(rc == 0)
    {
        int exit_code = libssh2_channel_get_exit_status(m_channel);
        char *exit_sig = NULL;
        libssh2_channel_get_exit_signal(m_channel, &exit_sig, NULL, NULL, NULL, NULL, NULL);

        fprintf(stderr, "exit code:%d, exit signal:%s", exit_code, exit_sig);
    }

    libssh2_channel_free(m_channel);
    m_channel = NULL;

    return 0;
}

bool AntCommander::scpTo(const QString &src, const QString &des)
{
    return true;
}

bool AntCommander::scpFrom(const QString &src, const QString &des)
{
    return true;
}

int AntCommander::waitsocket(int socket_fd, LIBSSH2_SESSION *session)
{
    struct timeval timeout;
    int rc;
    fd_set fd;
    fd_set *writefd = NULL;
    fd_set *readfd = NULL;
    int dir;

    timeout.tv_sec = 10;
    timeout.tv_usec = 0;

    FD_ZERO(&fd);

    FD_SET(socket_fd, &fd);

    /* now make sure we wait in the correct direction */
    dir = libssh2_session_block_directions(session);

    if(dir & LIBSSH2_SESSION_BLOCK_INBOUND)
        readfd = &fd;

    if(dir & LIBSSH2_SESSION_BLOCK_OUTBOUND)
        writefd = &fd;

    rc = select(socket_fd + 1, readfd, writefd, NULL, &timeout);

    return rc;
}








































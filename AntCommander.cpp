#include "AntCommander.h"
#include <QStringList>
#include <QString>

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
        if(session == NULL)
        {
            printf("init libssh2 session error");
            return false;
        }
        libssh2_session_set_blocking(m_session, 0);
    }

    if(m_socket < 0)
    {
        //create socket for libssh2
        m_socket = socket(AF_INET, SOCK_STTREAM, 0);
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
    while ((rc = libssh2_userauth_password(m_session, username.data(), password.data())) == LIBSSH2_ERROR_EAGAIN);
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

int AntCommander::exec(const QString &cmd, const QStringList &param, QString &stdout, QString &stderr)
{
    if(!checkConnect())
    {
        return -1;
    }

    cmd.append(param.join(' '));
    char *exec_cmd = cmd.toLatin1().data();

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
                stdout.append(buffer);
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
                stderr.append(buffer);
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
        waitsocket(m_socket, session);
    }
    if(rc == 0)
    {
        int exit_code = libssh2_channel_get_exit_status(m_channel);
        char *exit_sig = NULL;
        libssh2_channel_get_exit_signal(m_channel, &exit_sig, NULL, NULL, NULL, NULL, NULL);

        fprintf(stderr, "exit code:%s, exit signal:%s", exit_code, exit_sig);
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








































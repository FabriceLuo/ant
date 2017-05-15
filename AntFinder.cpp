#include "AntFinder.h"
#include <sys/types.h>
#include <sys/socket.h>

AntFinder::AntFinder()
{

}

bool AntFinder::connect(const QString &host, const QString &username, const QString &password)
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






















#include "AntCommander.h"
#include <QStringList>
#include <QString>
#include <sys/types.h>
#include <WinSock2.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fstream>

AntCommander::AntCommander()
{
    m_session = NULL;
}

AntCommander::AntCommander(const QString &host, const QString &username, const QString &password):
    m_host(host), m_username(username), m_password(password), m_socket(-1)
{
    m_session = NULL;

    m_node.username = username;
    m_node.password = password;
    m_node.ip = host;
}

void AntCommander::init(const QString &host, const QString &username, const QString &password)
{
    m_host      = host;
    m_username  = username;
    m_password  = password;
}

bool AntCommander::hostConnect(const QString &host, const QString &username, const QString &password)
{
    int rc;
    struct sockaddr_in sin;
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
#ifdef WIN32
    WSADATA wsadata;
    int err;

    err = WSAStartup(MAKEWORD(2,0), &wsadata);
    if (err != 0) {
        fprintf(stderr, "WSAStartup failed with error: %d\n", err);
        return 1;
    }
#endif
    if(m_socket < 0)
    {

        //create socket for libssh2
        m_socket = socket(AF_INET, SOCK_STREAM, 0);
        if(m_socket < 0)
        {
            printf("create socket for libssh2 failed");
            return false;
        }

        //connect to sshd
        sin.sin_family = AF_INET;
        sin.sin_port = htons(22);
        sin.sin_addr.s_addr = inet_addr(host.toLatin1().data());

        if(::connect(m_socket, (struct sockaddr*)&sin, sizeof(struct sockaddr_in)) != 0)
        {
            printf("connect to sshd failed");
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

    return true;
}

bool AntCommander::checkConnect()
{
    if(m_session == NULL)
    {
        return hostConnect(m_host, m_username, m_password);
    }
    return true;
}

int AntCommander::exec(const QString &cmd, const QStringList &param, QString &stdOut, QString &stdErr)
{
    if(!checkConnect())
    {
        return -1;
    }
    int rc;
    QStringList cmdList = param;
    cmdList.push_front(cmd);
    QString cmd_str = cmdList.join(' ');
    libssh2_session_set_blocking(m_session, 0);
    //open channel
    while((m_channel = libssh2_channel_open_session(m_session)) == NULL && libssh2_session_last_error(m_session, NULL, NULL, 0) == LIBSSH2_ERROR_EAGAIN);
    if(m_channel == NULL)
    {
        printf("open exec channel to server failed");
        return false;
    }
    while ((rc = libssh2_channel_exec(m_channel, cmd_str.toStdString().c_str())) == LIBSSH2_ERROR_EAGAIN) {
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
            memset(buffer, 0, sizeof(buffer));
            rc = libssh2_channel_read(m_channel, buffer, sizeof(buffer));
            if(rc > 0)
            {
                stdOut.append(buffer);
            }
            else
            {
                if(rc != LIBSSH2_ERROR_EAGAIN)
                {
                    fprintf(stderr, "libssh2 channel returned\n");
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
            memset(buffer, 0, sizeof(buffer));
            rc = libssh2_channel_read_stderr(m_channel, buffer, sizeof(buffer));
            if(rc > 0)
            {
                stdErr.append(buffer);
            }
            else
            {
                if(rc != LIBSSH2_ERROR_EAGAIN)
                {
                    fprintf(stderr, "libssh2 channel returned\n");
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
    int exit_code = -1;
    if(rc == 0)
    {
        exit_code = libssh2_channel_get_exit_status(m_channel);
        char *exit_sig = NULL;
        libssh2_channel_get_exit_signal(m_channel, &exit_sig, NULL, NULL, NULL, NULL, NULL);

        fprintf(stderr, "exit code:%d, exit signal:%s\n", exit_code, exit_sig);
    }

    libssh2_channel_free(m_channel);
    m_channel = NULL;

    return exit_code;
}

bool AntCommander::scpTo(const QString &src, const QString &des)
{
    if(!checkConnect())
    {
        return false;
    }
    bool success = true;
    //get send file info
    struct stat fileInfo;
    stat(src.toStdString().c_str(), &fileInfo);

    //open channel for send file
    libssh2_session_set_blocking(m_session, 1);
    m_channel = libssh2_scp_send(m_session, des.toStdString().c_str(), fileInfo.st_mode & 0777, (unsigned long)fileInfo.st_size);
    if(!m_channel)
    {
        char *errmsg;
                int errlen;
                int err = libssh2_session_last_error(m_session, &errmsg, &errlen, 0);
                fprintf(stderr, "Unable to open a session: (%d) %s\n", err, errmsg);
        fprintf(stderr, "open scp channel failed");
        return false;
    }

    //prepare read data
    char buffer[4096] = {0};
    FILE *fpRead = fopen(src.toStdString().c_str(), "rb");
    if(fpRead == NULL)
    {
        fprintf(stderr, "read src file failed");
        return false;
    }
    //read and send
    int rc = 0;
    do
    {
        //read file data from src
        int readCount = 0;
        readCount = fread(buffer, sizeof(char), sizeof(buffer), fpRead);
        if(readCount > 0)
        {
            char *ptr = buffer;
            do
            {
                rc = libssh2_channel_write(m_channel, ptr, readCount);
                if(rc < 0)
                {
                    fprintf(stderr, "write file content failed");
                    break;
                }
                else {
                    readCount -= rc;
                    ptr += rc;
                }
            }
            while(readCount);
        }
        //got file end
        if(readCount < sizeof(buffer))
        {
            if(feof(fpRead))
            {
                break;
            }
            if(ferror(fpRead))
            {
                success = false;
                break;
            }
        }
    }
    while(1);

    //close resource
    libssh2_channel_send_eof(m_channel);
    libssh2_channel_wait_eof(m_channel);
    libssh2_channel_wait_closed(m_channel);
    if(fpRead != NULL)
    {
        fclose(fpRead);
        fpRead = NULL;
    }
    if(m_channel != NULL)
    {
        libssh2_channel_free(m_channel);
        m_channel = NULL;
    }

    return success;
}

bool AntCommander::scpFrom(const QString &src, const QString &des)
{
    if(!checkConnect())
    {
        return false;
    }
    /*
    //prepare local file
    std::ofstream desOut;
    desOut.open(des);

    //scp from remoute
    struct stat fileinfo;
    LIBSSH2_CHANNEL *channel = libssh2_scp_recv(m_session, src.toLatin1().data(), &fileinfo);
    if(!channel)
    {
        return false;
    }

    //read file data from channel
    int got = 0, rc;
    char buffer[1024];
    while (got < fileinfo.st_size) {
        int toRead = sizeof(buffer);
        if(fileinfo.st_size - got < toRead)
        {
            toRead = fileinfo.st_size - got;
        }
        //read data
        rc = libssh2_channel_read(channel, buffer, toRead);
        if(rc > 0)
        {
            //write to des file
            desOut.write(buffer, rc);
        }
        else if(rc < 0)
        {
            goto failed;
        }
        got += rc;
    }

    libssh2_channel_free(channel);
    return true;
failed:
    if(desOut.is_open())
    {
        desOut.close();
    }
    */
    return 1;
}

ClusterNode AntCommander::getCurrentNode()
{
    return m_node;
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








































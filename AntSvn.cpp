#include "AntSvn.h"
#include <QStringList>
#include <QString>
#include <QProcess>

AntSvn::AntSvn(QString &path):
    m_svnPath(path)
{

}

QStringList AntSvn::getChangeList()
{

}

void AntSvn::setSvnPath(QString &path)
{
    m_svnPath = path;
}



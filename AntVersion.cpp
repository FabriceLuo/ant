#include "AntVersion.h"

AntVersion::AntVersion(QString dir)
    :m_dir(dir)
{

}

void AntVersion::setWorkDir(QString dir)
{
    m_dir = dir;
}

QString AntVersion::getWorkDir()
{
    return m_dir;
}

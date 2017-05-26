#include "AntSvn.h"

AntSvn::AntSvn(QString dir)
    :AntVersion(dir)
{

}

QList<VersionEntry> AntSvn::getchangeList()
{
    QList<VersionEntry> list;
    return list;
}

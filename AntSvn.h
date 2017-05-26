#ifndef ANTSVN_H
#define ANTSVN_H
#include "AntVersion.h"


class AntSvn : public AntVersion
{
public:
    AntSvn(QString dir);
    virtual QList<VersionEntry> getchangeList();
};

#endif // ANTSVN_H

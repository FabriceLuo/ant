#ifndef ANTGIT_H
#define ANTGIT_H
#include "AntVersion.h"

class AntGit : public AntVersion
{
public:
    AntGit(QString dir);
    virtual QList<VersionEntry> getchangeList();
};

#endif // ANTGIT_H

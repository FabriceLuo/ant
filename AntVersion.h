#ifndef ANTVERSION_H
#define ANTVERSION_H

#include "AntCommon.h"
#include <QList>

class AntVersion
{
public:
    AntVersion(QString dir);
    void setWorkDir(QString dir);
    QString getWorkDir();

    virtual QList<VersionEntry> getchangeList() = 0;
private:
    QString m_dir;
};

#endif // ANTVERSION_H

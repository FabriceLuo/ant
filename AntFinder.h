#ifndef ANTFINDER_H
#define ANTFINDER_H

#include <QString>
#include "AntCommander.h"
class QStringList;

class AntFinder
{
public:
    AntFinder(AntCommander *commander);
    QStringList find(const QString &filename);
    QString findDiffLatest(const QString &filename);
private:
    QStringList getFindDirList();
    int getFileDiffRate(const QString &local, const QString &remote);
    AntCommander *m_commander;
};

#endif // ANTFINDER_H

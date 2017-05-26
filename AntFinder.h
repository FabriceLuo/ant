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
private:
    QStringList getFindDirList();
    AntCommander *m_commander;
};

#endif // ANTFINDER_H

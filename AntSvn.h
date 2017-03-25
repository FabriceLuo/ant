#ifndef ANTSVN_H
#define ANTSVN_H

#include <QString>

class AntSvn
{
public:
    AntSvn();
    void setSvnPath(QString &path);
    QStringList getChangeList();
private:
    QString m_svnPath;

};

#endif // ANTSVN_H

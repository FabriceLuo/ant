#ifndef ANTDIFF_H
#define ANTDIFF_H
#include <QString>
class AntCommander;
class QStringList;

class AntDiff
{
public:
    AntDiff();
    QString findLatestDiff(const QString &path , QStringList &fileList, AntCommander *commander);
    int getFileDiffRate(const QString &file1, const QString &file2);
    void setTempDir(QString tempDir);
private:
    QString m_tempDir;

    QString genTempFilePath(const QString &path);

};

#endif // ANTDIFF_H

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
    float getRemouteFileDiffRate(const QString &file1, const QString &file2, AntCommander *commander);
    int getRemouteFileLineCount(const QString &path, AntCommander *commander);
    void setTempDir(QString tempDir);
    QString getTempDir();

private:

    QString m_tempDir;
    QString genTempFilePath(const QString &path);

};

#endif // ANTDIFF_H

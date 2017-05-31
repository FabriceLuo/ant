#include "AntDiff.h"
#include "AntCommander.h"

QString AntDiff::findLatestDiff(const QString &path, QStringList &fileList, AntCommander *commander)
{
    if(fileList.isEmpty())
    {
        return "";
    }
    QStringList::iterator begin = fileList.begin();
    QStringList::iterator end = fileList.end();
    QStringList::iterator latestDiff = NULL;
    int latestDiffRate = 0;
    while (begin != end) {
        QString tempFile = genTempFilePath(*begin);
        if(commander->scpFrom(*begin, tempFile))
        {
            int tempDiffRate = getFileDiffRate(path, *begin);
            if(tempDiffRate < latestDiffRate)
            {
                latestDiffRate = tempDiffRate;
                latestDiff = begin;
            }
        }
        begin++;
    }
    if(latestDiff != end)
    {
        return *latestDiff;
    }
    return "";
}

int AntDiff::getFileDiffRate(const QString &file1, const QString &file2)
{
    return 0;
}

void AntDiff::setTempDir(QString tempDir)
{
    m_tempDir = tempDir;
}

QString AntDiff::genTempFilePath(const QString &path)
{

}

AntDiff::AntDiff():
    m_tempDir("")
{

}

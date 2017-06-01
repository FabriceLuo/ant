#include "AntDiff.h"
#include "AntCommander.h"
#include <QFileInfo>
AntDiff::AntDiff()
{
    m_tempDir = "/tmp/ant.diff";
}
QString AntDiff::findLatestDiff(const QString &path, QStringList &fileList, AntCommander *commander)
{
    QString latestDiff;
    if(! fileList.count())
    {
        return latestDiff;
    }
    if( fileList.count() == 1)
    {
        //return fileList.at(0);
    }

    QStringList::iterator begin = fileList.begin();
    QStringList::iterator end = fileList.end();

    int latestDiffRate = -1;
    //mkdir temp dir
    QString tempDir = getTempDir();
    QString cmd = "mkdir", stdOut, stdErr;
    QStringList param;
    param << "-p" << tempDir;
    if(commander->exec(cmd, param, stdOut, stdErr) != 0)
    {
        return latestDiff;
    }

    //sync diff src to host
    QString tempFile = genTempFilePath(path);
    if(!commander->scpTo(path, tempFile))
    {
        //sprintf(stderr, "sync file(%s) to host failed", path.toStdString().c_str());
        return latestDiff;
    }

    while (begin != end) {
        int tempDiffRate = getRemouteFileDiffRate(tempFile, *begin, commander);
        if(latestDiffRate < 0 || tempDiffRate < latestDiffRate)
        {
            latestDiffRate = tempDiffRate;
            latestDiff = *begin;
        }
        begin++;
    }
    return latestDiff;
}

float AntDiff::getRemouteFileDiffRate(const QString &file1, const QString &file2, AntCommander *commander)
{
    int diffLineNum = 0, file1LineNum = 0, file2LineNum = 0;
    QString cmd = "diff";
    QStringList param;

    file1LineNum = getRemouteFileLineCount(file1, commander);
    file2LineNum = getRemouteFileLineCount(file2, commander);

    if(file1LineNum < 0 || file2LineNum < 0)
    {
        //sprintf(stderr, "file line num error");
        return -1;
    }

    QString stdOut, stdErr;
    param << file1 << file2 << "|wc -l";
    if(commander->exec(cmd, param, stdOut, stdErr) != 0)
    {
        //sprintf(stderr, "diff file(%s) and file(%s) failed", stdOut, stdErr);
        return -1;
    }
    diffLineNum = stdOut.toInt();
    float diffRate = diffLineNum / (file1LineNum + file2LineNum);
    return diffRate;
}

int AntDiff::getRemouteFileLineCount(const QString &path, AntCommander *commander)
{
    QString cmd = "cat";
    QString stdOut, stdErr;
    QStringList param;
    param << path <<  "|wc -l";
    if(commander->exec(cmd, param, stdOut, stdErr) != 0)
    {
        //sprintf(stderr, "diff file(%s) and file(%s) failed", stdOut, stdErr);
        return -1;
    }
    return stdOut.toInt();
}

void AntDiff::setTempDir(QString tempDir)
{
    m_tempDir = tempDir;
}

QString AntDiff::getTempDir()
{
    return m_tempDir;
}

QString AntDiff::genTempFilePath(const QString &path)
{
    if(path.isEmpty())
    {
        return "";
    }
    QString tempDir = getTempDir();
    QFileInfo fileInfo(path);
    QString tempPath = tempDir + "/" + fileInfo.fileName();

    return tempPath;
}



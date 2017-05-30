#include "AntFinder.h"
#include <QRegularExpression>

AntFinder::AntFinder(AntCommander *commander)
{
    m_commander = commander;
}

QStringList AntFinder::getFindDirList()
{
	QStringList list;
	list << "/home/mike";

    return list;
}

int AntFinder::getFileDiffRate(const QString &local, const QString &remote)
{

}

QStringList AntFinder::find(const QString &filename)
{
    if(NULL != m_commander)
    {

    }
    QStringList dirList = getFindDirList();

    QStringList::Iterator begin = dirList.begin();
    QStringList::Iterator end   = dirList.end();

    QString cmd("find");
    QString stdOut, stdErr;

    QStringList fileList;

    while (begin != end) {
		QStringList param;
		param << *begin << "-name" << filename;
        if(m_commander->exec(cmd, param, stdOut, stdErr) != 0)
        {

        }
        if(! stdOut.isEmpty())
        {
            QStringList lines = stdOut.split(QRegularExpression("\n"));
            int lineCount = lines.count();

            for(int i = 0; i < lineCount; i++)
            {
                fileList.push_back(lines.at(i));
            }
        }
        begin++;
    }
    return fileList;
}

QString AntFinder::findDiffLatest(const QString &filename)
{
    //get all file list of same name
    QStringList foundList = find(filename);
    QString latestDiff;
    int latestDiffRate = -1;
    if(foundList.count())
    {
        QStringList::iterator begin = foundList.begin();
        QStringList::iterator end = foundList.end();

        while(begin != end)
        {
            int tempDiffRate = getFileDiffRate()
            begin++;
        }
    }
    return latestDiff;
}























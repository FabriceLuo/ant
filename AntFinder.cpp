#include "AntFinder.h"
#include <QRegularExpression>
#include "AntSetting.h"

AntFinder::AntFinder(AntCommander *commander)
{
    m_commander = commander;
}

QStringList AntFinder::getFindDirList()
{
    AntSetting setting;
    QStringList list;
    if(! setting.getGlobalSearchDir(list))
    {
        fprintf(stderr, "get global search dir failed");
        list.clear();
    }

    return list;
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
            return fileList;
        }
        if(! stdOut.isEmpty())
        {
            QStringList lines = stdOut.split(QRegularExpression("\n"));
            int lineCount = lines.count();

            for(int i = 0; i < lineCount; i++)
            {
                if(!lines.at(i).isEmpty())
                {
                   fileList.push_back(lines.at(i));
                }
            }
        }
        begin++;
    }
    return fileList;
}























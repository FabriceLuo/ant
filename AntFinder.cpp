#include "AntFinder.h"
#include <QRegularExpression>

AntFinder::AntFinder(AntCommander *commander)
{
    m_commander = commander;
}

QStringList AntFinder::getFindDirList()
{
    QStringList list = {
        "/home/mike"
    };

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
    QString stdout, stderr;

    QStringList fileList;

    while (begin != end) {
        QStringList param = {
            *begin,
            "-name",
            filename
        };
        if(m_commander->exec(cmd, param, stdout, stderr) != 0)
        {

        }
        if(! stdout.isEmpty())
        {
            QStringList lines = stdout.split(QRegularExpression("\n"));
            int lineCount = lines.count();

            for(int i = 0; i < lineCount; i++)
            {
                QString temp;
                temp = *begin + lines.at(i);
                fileList.push_back(temp);
            }
        }
        begin++;
    }
    return fileList;
}























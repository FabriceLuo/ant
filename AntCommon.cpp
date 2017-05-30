#include "AntCommon.h"

AntCommon::AntCommon()
{

}

QString AntCommon::getEntryStatusString(EntryStatus status)
{
    QString str;
    switch (status) {
    case EntryAdd:
        str = "A";
        break;
    case EntryDelete:
        str = "D";
        break;
    case EntryModify:
        str = "M";
        break;
    case EntryUnversion:
        str = "U";
        break;
    default:
        str = "E";
        break;
    }
    return str;
}



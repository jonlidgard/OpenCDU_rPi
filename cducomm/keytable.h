#ifndef KEYTABLE_H
#define KEYTABLE_H

#include <QMap>


class KeyTable
{
public:
    KeyTable();
    QString nameForCode(uint code, bool ctrl);

private:
    QMap<int, QString> keyMap;
    QMap<int, QString> ctrlMap;
};

#endif // KEYTABLE_H

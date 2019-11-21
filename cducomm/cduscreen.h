#ifndef CDUSCREEN_H
#define CDUSCREEN_H

#include <QtWidgets>

#include "PMDG_NGX_SDK.h"

union CduData
{
    char *pScreen;
    PMDG_NGX_CDU_Screen screen;
};


class CduScreen //: public QObject
{
//    Q_OBJECT
public:
    CduScreen();
    CduScreen(const QByteArray &byteArray);

    QByteArray asByteArray();

    bool isPowered() { return screen.Powered != 0; }
    PMDG_NGX_CDU_Cell *pCellAt(int col, int row);
    void clear();
    void setSymbol(QChar c, int col, int row);
    void setFlags(int flags, int col, int row);
    void setColor(int color, int col, int row);
    void setPowered(bool powered) {screen.Powered = powered;}
    void setString(QString text, int col, int row, int color, bool small);
    void setStringCentre(QString text, int row, int color, bool small);
    void setStringRight(QString text, int row, int color, bool small);
    void setStringLeft(QString text, int row, int color, bool small) {setString(text,0,row,color,small);}
    QChar getSymbol(int col, int row);
    QString getStringLeft(int row);
    QString getStringRight(int row);


    QString symbol(int col, int row)  {return QString(screen.Cells[col][row].Symbol);}
    int flags(int col, int row) {return screen.Cells[col][row].Flags;}
    int color(int col, int row) {return screen.Cells[col][row].Color;}
private:
    PMDG_NGX_CDU_Screen screen;
};

QDataStream &operator<<(QDataStream &out, const CduScreen &screen);
QDataStream &operator>>(QDataStream &in, CduScreen &screen);

#endif // CDUSCREEN_H

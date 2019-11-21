#include "cduscreen.h"

/*
CduScreen::CduScreen(QObject *parent) : QObject(parent)
{

}
*/

void CduScreen::setSymbol(QChar c, int col, int row)
{
//    c = (c == '_') ? 0x5f : c;
    screen.Cells[col][row].Symbol = c.toLatin1();
}

void CduScreen::setFlags(int flags, int col, int row)
{
    screen.Cells[col][row].Flags = flags;
}

void CduScreen::setColor(int color, int col, int row)
{
    screen.Cells[col][row].Color = color;
}

CduScreen::CduScreen()
{
    clear();
    screen.Powered = true;
}

CduScreen::CduScreen(const QByteArray &byteArray)
{
    const char *p = byteArray.constData();
    memcpy((char*)&screen, p, sizeof(PMDG_NGX_CDU_Screen));
}


PMDG_NGX_CDU_Cell *CduScreen::pCellAt(int col, int row)
{
  return &screen.Cells[col][row];
}


QByteArray CduScreen::asByteArray()
{
    QByteArray byteArray;

    QDataStream stream(&byteArray, QIODevice::WriteOnly);
    stream.setVersion(QDataStream::Qt_4_5);

    stream.writeRawData((const char *)&screen, sizeof(PMDG_NGX_CDU_Screen));
    return byteArray;
}


QDataStream &operator<<(QDataStream &out, CduScreen &screen)
{
    out << screen.asByteArray();
    return out;
}

QDataStream &operator>>(QDataStream &in, CduScreen &newCduScreen)
{
    QByteArray byteArray;
    in >> byteArray;
    newCduScreen = CduScreen(byteArray);
    return in;
}

void CduScreen::clear()
{
    char *p = (char *)&screen;
    for(unsigned long i = 0; i < sizeof(PMDG_NGX_CDU_Screen);) {
            p[i] = ' ';
            p[i+1] = '\0';
            p[i+2] = '\0';
            i += sizeof(PMDG_NGX_CDU_Cell);
    }
}

void CduScreen::setString(QString text, int col, int row, int color, bool small)
{
    if (col < 0 || col >= CDU_COLUMNS || row < 0 || row >= CDU_ROWS)
        return;
    int endCol = col + text.length();
    endCol  = endCol < CDU_COLUMNS ? endCol : CDU_COLUMNS;
    for (int i=col, j = 0; i < endCol; i++)
    {
        QChar c = text.at(j++);
        int flags = small ? PMDG_NGX_CDU_FLAG_SMALL_FONT: 0;
        setSymbol(c, i, row);
        setColor(color, i, row);
        setFlags(flags, i, row);
    }
}

void CduScreen::setStringCentre(QString text, int row, int color, bool small)
{
    int leftCol = (CDU_COLUMNS - text.length()) / 2;
    setString(text,leftCol,row,color,small);
}

void CduScreen::setStringRight(QString text, int row, int color, bool small)
{
    int leftCol = CDU_COLUMNS - text.length();
    setString(text,leftCol,row,color,small);
}

QChar CduScreen::getSymbol(int col, int row)
{
    PMDG_NGX_CDU_Cell *cell = pCellAt(col, row);
    return cell->Symbol;
}

QString CduScreen::getStringLeft(int row)
{
    int col = 0;

    QString s = "";
    QChar c = getSymbol(col,row);
    while ( c != ' ')
    {
        s += c;
        c = getSymbol(++col,row);
    };
    return s;
}

QString CduScreen::getStringRight(int row)
{
    int col = CDU_COLUMNS-1;

    QString s = "";
    QChar c = getSymbol(col,row);
    while ( c != ' ')
    {
        s = c + s;
        c = getSymbol(--col,row);
    };
    return s;
}


//constData() used because const is desired; otherwise, prefer data() don't forgetting deep copy issues
//    const void* poTemp = (const void*)byteArray.constData();
//    const MyStruct* poStruct = static_cast< const MyStruct* >(poTemp);


//    const void* pTemp = (const void*)data.constData();
//    const quint32* pLength = static_cast< const  quint32* >(pTemp);
//   quint32 l = *pLength;
//   if ( l != sizeof(PMDG_NGX_CDU_Screen)) {
//       qDebug() << "Error wrong data length read...";
//       return;
//   }

//    pTemp += sizeof(quint32));
//    const CduData* cduData =  static_cast< const CduData* >(pTemp+4);
//    cdu = cduData;
//  //    stream.readBytes(static_cast<char*>(pTemp),length);
// void* pTemp = (void*)cduData->pScreen;


//    memcpy(cdu.pScreen, data.constData(), sizeof(PMDG_NGX_CDU_Screen));

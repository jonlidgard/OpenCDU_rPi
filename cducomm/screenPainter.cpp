/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtWidgets>
#include <QMutexLocker>

#include "screenPainter.h"
#include "cduscreen.h"

ScreenPainter::ScreenPainter(QWidget *parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_StaticContents);
//    xStep = this->width() / CDU_COLUMNS;
//    yStep = this->height() / CDU_ROWS;

}

void ScreenPainter::clearImage()

{
    image.fill(qRgb(0, 0, 0));
    update();
}

void ScreenPainter::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QRect dirtyRect = event->rect();
    painter.drawImage(dirtyRect, image, dirtyRect);
}

void ScreenPainter::resizeEvent(QResizeEvent *event)
//! [15] //! [16]
{
/*    if (width() > image.width() || height() > image.height()) {
        int newWidth = qMax(width() + 128, image.width());
        int newHeight = qMax(height() + 128, image.height());
        resizeImage(&image, QSize(newWidth, newHeight));
        xStep = this->width() / (CDU_COLUMNS+1);
        yStep = this->height() / (CDU_ROWS+1);
        xLeftMargin = xStep/2 + ;
        yTopMargin = yStep/2;
        update();
    }
 */

    xLeftMargin = 55 ;
    yTopMargin = 00;
    xFontAdjust = 0;
    yFontAdjust = 0;

    int newWidth = width();
    int newHeight = height();
    qDebug() << "Width: " << newWidth;

    qDebug() << "Height: " << newHeight;

    resizeImage(&image, QSize(newWidth, newHeight));
    xStep = (this->width()  - xLeftMargin) / (CDU_COLUMNS+1);
    yStep = (this->height() - yTopMargin) / (CDU_ROWS);
    update();




    QWidget::resizeEvent(event);
}

void ScreenPainter::resizeImage(QImage *image, const QSize &newSize)
//! [19] //! [20]
{
    if (image->size() == newSize)
        return;

    QImage newImage(newSize, QImage::Format_RGB32);
    newImage.fill(qRgb(0, 0, 0));
    QPainter painter(&newImage);
    painter.drawImage(QPoint(0, 0), *image);
    *image = newImage;
}


int ScreenPainter::setFontColor(QPainter *painter, int color)
{

    if (previousColor == color)
        return 0;

    previousColor = color;
    switch (color)
    {
    case PMDG_NGX_CDU_COLOR_WHITE:
        painter->setPen("white");
        break;
    case PMDG_NGX_CDU_COLOR_CYAN:
        painter->setPen("cyan");
        break;
    case PMDG_NGX_CDU_COLOR_GREEN:
        painter->setPen("green");
        break;
    case PMDG_NGX_CDU_COLOR_MAGENTA:
        painter->setPen("magenta");
        break;
    case PMDG_NGX_CDU_COLOR_AMBER:
        painter->setPen("yellow");
        break;
    case PMDG_NGX_CDU_COLOR_RED:
        painter->setPen("red");
        break;
    default:
        painter->setPen("white");
        break;
    }

    return 1;
}

int ScreenPainter::setFontFlags(QPainter *painter, int flags)
{
    static int previousFlags = 0xff; // force the first change

    // Do Font Size:

    int fontSizeFlag = flags & PMDG_NGX_CDU_FLAG_SMALL_FONT;

    int fontSizeChanged = fontSizeFlag ^ (previousFlags & PMDG_NGX_CDU_FLAG_SMALL_FONT);

    if (firstRun || fontSizeChanged) {
        if (fontSizeFlag)
        {

            myFont.setFamily("BoeingCDUSmall");
//            myFont.setBold(true);
            myFont.setPointSize(36); //88
            xFontAdjust = 5;
            yFontAdjust = -5;
            previousFlags = previousFlags | PMDG_NGX_CDU_FLAG_SMALL_FONT ;

        }
        else
        {
            myFont.setFamily("BoeingCDULarge");
//            myFont.setBold(true);
            myFont.setPointSize(36);// 70
            xFontAdjust = 0;
            yFontAdjust = 0;
            previousFlags = previousFlags &  ~PMDG_NGX_CDU_FLAG_SMALL_FONT ;
        }
    }


    // Do Reverse Video:

    int rVideoFlag = flags & PMDG_NGX_CDU_FLAG_REVERSE;

    int rVideoChanged = rVideoFlag ^ (previousFlags & PMDG_NGX_CDU_FLAG_REVERSE);

    if (firstRun || rVideoChanged) {
        if (rVideoFlag)
        {
            previousFlags = previousFlags | PMDG_NGX_CDU_FLAG_REVERSE;
        }
        else
        {
            previousFlags = previousFlags & ~PMDG_NGX_CDU_FLAG_REVERSE ;
        }
    }


    // Do Unused (Dimmed) Text:

    int unusedFlag = flags & PMDG_NGX_CDU_FLAG_UNUSED;

    int unusedChanged = unusedFlag ^ (previousFlags & PMDG_NGX_CDU_FLAG_UNUSED);

    if (firstRun || unusedChanged) {
        if (unusedFlag)
        {
            opacity = 0.5;
            previousFlags = previousFlags | PMDG_NGX_CDU_FLAG_UNUSED;
        }
        else
        {
            opacity = 1.0;
            previousFlags = previousFlags & ~PMDG_NGX_CDU_FLAG_UNUSED ;
        }
    }
    int changeFlags = fontSizeChanged | rVideoChanged | unusedChanged;

    bool updatePainter = (changeFlags | firstRun);
    firstRun = false;
    if (updatePainter)
    {
        painter->setFont(myFont);
    }
    return changeFlags;
}

void ScreenPainter::updateScreen(QByteArray data)
{

    QMutexLocker locker(&mMutex);
    CduScreen cduScreen(data);

    image.fill(qRgb(0, 0, 0));
    firstRun = true;
    previousColor = 0xff;

    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing, false);
    const PMDG_NGX_CDU_Cell *cell;
    unsigned char c;
    int changedFlags;
//    qDebug() << "here";

    if (!cduScreen.isPowered())
        return;
//    qDebug() << "here";

    for (int row=0, yPos = yTopMargin; row < CDU_ROWS; row++, yPos+=yStep)
    {
        for (int col=0, xPos = xLeftMargin; col < CDU_COLUMNS; col++, xPos+=xStep)
        {
            cell = cduScreen.pCellAt(col,row);
            setFontColor(&painter, cell->Color);

            changedFlags = setFontFlags(&painter, cell->Flags);
            c = cell->Symbol;
            c = (c == 0xea) ? 0x5f : c;
            painter.drawStaticText(xPos+xFontAdjust, yPos+yFontAdjust,QString(c));
//            painter.drawStaticText(xPos,yPos,QString(c));

//            qDebug() << "Drawing screen" << xPos << yPos << xStep <<yStep << yFontAdjust;

        }
    }
    update();
    //c++;
}

void ScreenPainter::debugOutput(QByteArray b)
{

    CduScreen cduScreen(b);
    const PMDG_NGX_CDU_Cell *cell;
    int changedFlags;

    static int n = 0;
    QString line;
    qDebug() << "Output: " << n++;
    for (int row=0, yPos = yTopMargin; row < CDU_ROWS; row++, yPos+=yStep)
    {
        line = "";
        for (int col=0, xPos = xLeftMargin; col < CDU_COLUMNS; col++, xPos+=xStep)
        {
            cell = cduScreen.pCellAt(col,row);
       //     setFontColor(&painter, cell->Color);

         //   changedFlags = setFontFlags(&painter, cell->Flags);
        //    painter.drawStaticText(xPos,yPos,QString(cell->Symbol));
            line.append(cell->Symbol);
        }
        qDebug() << line;
    }
    qDebug() << "________________________________";
}

void ScreenPainter::testScreen(QString s)
{
    CduScreen screen;

    screen.setPowered(true);


    for (int row=0; row < CDU_ROWS; row++)
    {
        for (int col=0; col < CDU_COLUMNS; col++)
        {
            screen.setColor(row % 6, col, row);
            screen.setFlags(0,col, row);
            screen.setSymbol('<', col, row);
        }
    }

    updateScreen(screen.asByteArray());
}


void ScreenPainter::splashScreen(QString addr, quint16 port)
{
    CduScreen screen;

    screen.setPowered(true);

    screen.setStringCentre("CDUCOMM V1.0",0,PMDG_NGX_CDU_COLOR_MAGENTA,0);
    screen.setStringCentre("JON LIDGARD (2017)",2,PMDG_NGX_CDU_COLOR_WHITE,true);
    screen.setStringLeft("SERVER",5,PMDG_NGX_CDU_COLOR_WHITE,true);
    screen.setStringLeft(addr,6,PMDG_NGX_CDU_COLOR_WHITE,false);
    screen.setStringLeft("PORT",7,PMDG_NGX_CDU_COLOR_WHITE,true);
    screen.setStringLeft(QString::number(port) ,8,PMDG_NGX_CDU_COLOR_WHITE,false);
    screen.setStringLeft("<CONNECT",11,PMDG_NGX_CDU_COLOR_WHITE,false);
    screen.setStringLeft("-------------------------",12,PMDG_NGX_CDU_COLOR_WHITE,false);
    screen.setStringRight("SELECT",5,PMDG_NGX_CDU_COLOR_WHITE,true);
    screen.setStringRight("<LEFT>",6,PMDG_NGX_CDU_COLOR_MAGENTA,false);
    screen.setStringRight("<RIGHT>",8,PMDG_NGX_CDU_COLOR_WHITE,false);

    updateScreen(screen.asByteArray());
}

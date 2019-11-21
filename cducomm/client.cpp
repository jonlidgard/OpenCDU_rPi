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
#include <QtNetwork>

#include "client.h"


QByteArray IntToArray(qint32 source) //Use qint32 to ensure that the number have 4 bytes
{
    //Avoid use of cast, this is the Qt way to serialize objects
    QByteArray temp;
    QDataStream data(&temp, QIODevice::ReadWrite);
    data << source;
    return temp;
}


Client::Client(QObject *parent) : QObject(parent)
{

    socket = new QTcpSocket(this);

    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(socket, SIGNAL(bytesWritten(qint64)), this, SLOT(bytesWritten(qint64)));

    in.setDevice(socket);
    in.setVersion(QDataStream::Qt_4_0);

 /*
    QNetworkConfigurationManager manager;
    if (manager.capabilities() & QNetworkConfigurationManager::NetworkSessionRequired) {
        // Get saved network configuration
        QSettings settings(QSettings::UserScope, QLatin1String("QtProject"));
        settings.beginGroup(QLatin1String("QtNetwork"));
        const QString id = settings.value(QLatin1String("DefaultNetworkConfiguration")).toString();
        settings.endGroup();

        // If the saved network configuration is not currently discovered use the system default
        QNetworkConfiguration config = manager.configurationFromIdentifier(id);
        if ((config.state() & QNetworkConfiguration::Discovered) !=
            QNetworkConfiguration::Discovered) {
            config = manager.defaultConfiguration();
        }

        networkSession = new QNetworkSession(config, this);
        connect(networkSession, &QNetworkSession::opened, this, &Client::sessionOpened);

        networkSession->open();

    }
*/
}

bool Client::connectToHost(QString host, qint16 port)
{
    qDebug() << host << port;
//    emit newAnnunciatorData("+X");
    socket->abort();
    socket->connectToHost(host, port);

    if(!socket->waitForConnected(1000))
    {
        qDebug() << "Connect Error: " << socket->errorString();
        QTimer::singleShot(100, this, SLOT(disconnected()));
        return false;
    }
    return true;
}

void Client::connected()
{

    qDebug() << "Connected!";
    emit connectedToServer();
    //    socket->write("HEAD / HTTP/1.0\r\n\r\n\r\n\r\n");
}

void Client::disconnected()
{
    qDebug() << "Disconnected!";
    emit disconnectedFromServer();
}

void Client::bytesWritten(qint64 bytes)
{
//    qDebug() << "We wrote: " << bytes;
}

void Client::readyRead()
{
//    qDebug() << "Reading...";
    readData();
  //  qDebug() <<
    socket->readAll();
}


bool Client::writeString(QString s)
{
    if(socket->state() == QAbstractSocket::ConnectedState)
    {
        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_0);
        out.writeBytes(s.toUtf8(), s.length());
//        out << s;
//        block = s.toUtf8();
//        qDebug() << block  << s << s.length() << block.length() << block.size();
        socket->write(block); //write the data itself
//        socket->write(block.data(), block.size()); //write the data itself
        return socket->waitForBytesWritten();
    }
    else
        return false;
}

bool Client::writeData(QByteArray data)
{
    if(socket->state() == QAbstractSocket::ConnectedState)
    {
        socket->write(IntToArray(data.size())); //write size of data
        socket->write(data); //write the data itself
        return socket->waitForBytesWritten();
    }
    else
        return false;
}

void Client::readData()
{
    in.startTransaction();

    QByteArray cduData;
    in >> cduData;

    if (!in.commitTransaction())
        return;

    if (cduData.length()  < 100 ) {
        QString text = QTextCodec::codecForMib(106)->toUnicode(cduData);
        // do annunciator stuff
        if (text.left(5) == "0001:")
            emit newAnnunciatorData(text.mid(5));
    }
    else {
        // do cdu screen stuff
        emit newScreenData(cduData);
    }
}

void Client::displayError(QAbstractSocket::SocketError socketError)
{
 /*   switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        QMessageBox::information(this, tr("CDUComm Client"),
:                                 tr("The host was not found. Please check the "
                                    "host name and port settings."));
        break;
    case QAbstractSocket::ConnectionRefusedError:
        QMessageBox::information(this, tr("CDUComm Client"),
                                 tr("The connection was refused by the peer. "
                                    "Make sure the CDUComm server is running, "
                                    "and check that the host name and port "
                                    "settings are correct."));
        break;
    default:
        QMessageBox::information(this, tr("CDUComm Client"),
                                 tr("The following error occurred: %1.")
                                 .arg(socket->errorString()));
    }
*/
}

void Client::sessionOpened()
{
    // Save the used configuration
    QNetworkConfiguration config = networkSession->configuration();
    QString id;
    if (config.type() == QNetworkConfiguration::UserChoice)
        id = networkSession->sessionProperty(QLatin1String("UserChoiceConfiguration")).toString();
    else
        id = config.identifier();

    QSettings settings(QSettings::UserScope, QLatin1String("CDUComm"));
    settings.beginGroup(QLatin1String("QtNetwork"));
    settings.setValue(QLatin1String("DefaultNetworkConfiguration"), id);
    settings.endGroup();

}

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>

class Settings : public QObject
{
    Q_OBJECT
public:
    explicit Settings(QObject *parent = 0);
    void load(void);
    void save(void);

    void setFileName(QString value) {fileName = value; load();}
    QString getFileName(void) {return fileName;}

    void setInputDevice(QString value) {inputDevice = value;}
    void setHost(QString value) {host = value;}
    void setPort(quint16 value) {port = value;}
    QString getInputDevice(void) {return inputDevice;}
    QString getHost(void) {return host;}
    quint16 getPort(void) {return port;}

signals:

public slots:

private:
    QString fileName;
    QString host;
    qint16 port;
    QString inputDevice;

};

#endif // SETTINGS_H

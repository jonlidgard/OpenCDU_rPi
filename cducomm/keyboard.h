#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <QObject>
#include <QProcess>
#include <QSocketNotifier>
#include "keytable.h"

class Keyboard : public QObject
{
    Q_OBJECT
public:
    explicit Keyboard(QObject *parent = 0);
    ~Keyboard();
    bool connectToDevice(QString device);

signals:
    void sendKey(QString key);

public slots:
    void setAnnunciatorLight(QString value);
    void ExecOn();
    void ExecOff();

private slots:
    void readyReadKey();

private:
    KeyTable *keyTable;
    int fd;
    QProcess *proc;

    QSocketNotifier *m_sn;

    int getleds(int tty, char *cur_leds);
    int setleds(char cur_leds);
    int getflags(int tty, char *flags);

};

#endif // KEYBOARD_H

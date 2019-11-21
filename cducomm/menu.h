#ifndef MENU_H
#define MENU_H

#include <QObject>

class Menu : public QObject
{
    Q_OBJECT
public:
    explicit Menu(QObject *parent = 0);

signals:

public slots:
    bool keyPressed(QString s);
};

#endif // MENU_H

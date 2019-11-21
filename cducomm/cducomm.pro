QT += widgets
QT += network widgets
qtHaveModule(printsupport): QT += printsupport


HEADERS       = mainwindow.h \
                client.h \
                PMDG_NGX_SDK.h \
    settings.h \
    keytable.h \
    keyboard.h \
    screenPainter.h \
    cduscreen.h \
    menu.h

SOURCES       = main.cpp \
                mainwindow.cpp \
                client.cpp \
    settings.cpp \
    keytable.cpp \
    keyboard.cpp \
    screenPainter.cpp \
    cduscreen.cpp \
    menu.cpp

# install
INSTALLS = target
target.files = cducomm
target.path = /home/pi

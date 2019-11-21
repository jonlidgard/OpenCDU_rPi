#include "keyboard.h"
#include <QtWidgets>

#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

//#include <sys/termio.h>
#include <errno.h>

#include <linux/input.h>
#include <linux/kd.h>
#include <QProcess>


#define CDU_TTY 0

Keyboard::Keyboard(QObject *parent) : QObject(parent)
{
    keyTable = new KeyTable;
    proc = new QProcess();
}

Keyboard::~Keyboard()
{
    delete keyTable;
    delete proc;
}

void Keyboard::ExecOn()
{
    proc->start("setCap", QStringList() << "+");
}


void Keyboard::ExecOff()
{
    proc->start("setCap", QStringList() << "-");
}

bool Keyboard::connectToDevice(QString device)
{
//        fd = open(device.toLocal8Bit(), O_NONBLOCK | O_RDONLY);
// To Test -try with individual flags set to see what works
    fd = open(device.toLocal8Bit(), O_RDONLY, O_NOCTTY, O_CLOEXEC);
        if(fd <=0)
        {
            qDebug()<<"[WARN]Can't open device!";
            return false;
        }
// To test - should stop keys going to the terminal
        if (ioctl(fd,EVIOCGRAB, 1)) {
            const int saved_errno = errno;
            close (fd);
            return errno = (saved_errno) ? errno : EACCES;
        }
//
        m_sn= new QSocketNotifier(fd, QSocketNotifier::Read);
        m_sn->setEnabled(true);
        connect(m_sn, SIGNAL(activated(int)), this, SLOT(readyReadKey()));
        return true;
}

void Keyboard::setAnnunciatorLight(QString value)
{
    qDebug() << value;

    if ( value.contains("X") )
            ExecOn();
    else if ( value.contains("x"))
            ExecOff();
}


void Keyboard::readyReadKey()
{
    QString keyName = "";
    struct input_event ev[64];
    uint size = sizeof(struct input_event);

    read(m_sn->socket(),ev, size*64);

    if (ev[1].type == EV_KEY && ev[1].value == 1)
    {
        if (ev[1].code == 29)
        { // ctrl
            keyName = keyTable->nameForCode(ev[3].code, true);
        }
        else
        {
            keyName = keyTable->nameForCode(ev[1].code, false);
        }
        if (keyName != "")
        {
            qDebug() << keyName;
            emit sendKey(keyName);
        }
    }

}


int Keyboard::getleds(int tty, char *cur_leds) {
    if (ioctl(fd, KDGETLED, cur_leds)) {
        qDebug() << "Error: getleds: " << tty << KDGETLED << *cur_leds;
        return -1;
    }
    qDebug() << "Success: getleds: " << *cur_leds;
    return 0;
}

int Keyboard::setleds(char cur_leds) {
    qDebug() << "LED:" << cur_leds;
    if (ioctl(CDU_TTY, KDSETLED, cur_leds)) {
        qDebug() << "Error: setleds: " << cur_leds;
    return -1;
    }
    return 0;
    qDebug() << "Success: setleds: " << cur_leds;
}

int Keyboard::getflags(int tty, char *flags) {
    if (ioctl(fd, KDGKBLED, flags)) {
        qDebug() << "Error: getflags: " << tty << KDGETLED << *flags;
        return -1;
    }
    qDebug() << "Success: getflags: " << *flags;
    return 0;
}


/*


//    char oleds, nleds, oflags, nflags, odefflags, ndefflags;


//    proc->start("/home/pi/setCap", QStringList() << "+caps");


//    setlocale(LC_ALL, "");
//    bindtextdomain(PACKAGE_NAME, LOCALEDIR);
//    textdomain(PACKAGE_NAME);

    //getflags(&oflags);

//    for(int i=0; i < 256; i++)
//        getflags(0, &oflags);
//        getleds(0, &oleds);

//    odefflags = ndefflags = ((oflags >> 4) & 7);
//    oflags = nflags = (oflags & 7);

//    int nval = 0;
//    int ndef = 0;




/*
        {
//            nval |= LED_CAP;
            qDebug() << "+CAPS" << nval;
            proc->start("/home/pi/setCap", QStringList() << "+");
        } else
        {
            qDebug() << "-CAPS" << nval;
        }
//        ndef |= LED_CAP;
*/



    // Try one or the other (see Linux:setleds source)
    // Option -F Set Flags (& maybe the kbd ctrlr wil set the lights according to the flags)
/*            nflags = ((oflags & ~ndef) | nval);
            if (ioctl(CDU_TTY, KDSKBLED, (ndefflags << 4) | nflags))
            {
                qDebug() << "Error: option F: " << KDSKBLED << ndefflags << nflags << CDU_TTY;
            }

    // Option -L Set the leds directly & leave flags untouched
    //        nleds = (oleds & ~ndef) | nval;
//            setleds(nleds);




#include <linux/kd.h>
#include "nls.h"
#include "version.h"



struct led {
    char *name;
    int bit;
    int sunbit;
} leds[] = {
    { "scroll", LED_SCR, LED_SCRLCK },
    { "num",    LED_NUM, LED_NLOCK },
    { "caps",   LED_CAP, LED_CLOCK },
};

static void
getleds(char *cur_leds) {
    if (ioctl(0, KDGETLED, cur_leds)) {
    perror("KDGETLED");
    fprintf(stderr,
      _("Error reading current led setting. Maybe stdin is not a VT?\n"));
    exit(1);
    }
}
static int
setleds(char cur_leds) {
    if (ioctl(0, KDSETLED, cur_leds)) {
    perror("KDSETLED");
    return -1;
    }
    return 0;
}
static void
getflags(char *flags) {
    if (ioctl(0, KDGKBLED, flags)) {
    perror("KDGKBLED");
    fprintf(stderr,
          _("Error reading current flags setting. "
        "Maybe you are not on the console?\n"));
    exit(1);
    }
}


int

main(int argc, char **argv) {
    int optL = 0, optD = 0, optF = 0, verbose = 0;
    char oleds, nleds, oflags, nflags, odefflags, ndefflags;
    char nval, ndef, sign;
    struct led *lp;


    getflags(&oflags);
    getleds(&oleds);
      optL = 1;
      optF = 1;

      odefflags = ndefflags = ((oflags >> 4) & 7);
    oflags = nflags = (oflags & 7);
    if (!optL)
    optF = 1;
    nval = 0;
    ndef = 0;

    sign = 1;		// by default: set
    else if(*ap == '-') {
        sign = 0;

        if(sign) {
          nval |= lp->bit;
        }
        ndef |= lp->bit;
        goto nxtarg;
        }
    }
    fprintf(stderr, _("unrecognized argument: _%s_\n\n"), ap);
    usage();
      nxtarg: ;
    }
    if (optF) {
        nflags = ((oflags & ~ndef) | nval);
        if (ioctl(0, KDSKBLED, (ndefflags << 4) | nflags)) {
            perror("KDSKBLED");
            exit(1);
        }
    }
    if (optL) {
        nleds = (oleds & ~ndef) | nval;
        if (setleds(nleds))
            exit(1);
    }
    exit(0);
}




*/

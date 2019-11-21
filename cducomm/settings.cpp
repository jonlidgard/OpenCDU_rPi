#include "settings.h"
#include <QSettings>

const QString deviceId = "/dev/input/by-id/usb-Code_Mercenaries_TEKWorx_Limited_interfaceIT_CDU-event-kbd";
const QString defaultHost = "192.168.200.2";
const QString defaultPort = "8005";

Settings::Settings(QObject *parent) : QObject(parent)
{
}



void Settings::load(void)
{
    QSettings settings(fileName, QSettings::NativeFormat);
    setInputDevice(settings.value("inputDevice", deviceId).toString());
    setHost(settings.value("host", defaultHost).toString());
    setPort(settings.value("port", defaultPort).toUInt());

    save();
}

void Settings::save(void)
{
    QSettings settings(fileName, QSettings::NativeFormat);
    settings.setValue("inputDevice", getInputDevice());
    settings.setValue("host", getHost());
    settings.setValue("port", getPort());
}

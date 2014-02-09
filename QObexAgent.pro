#-------------------------------------------------
#
# Project created by QtCreator 2013-12-30T17:36:43
#
#-------------------------------------------------

QT       += dbus core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QObexAgent
TEMPLATE = app

DBUS_ADAPTORS += dbus/org.bluez.obex.Agent1.xml
DBUS_INTERFACES += \
    dbus/org.bluez.obex.AgentManager1.xml \
    dbus/org.freedesktop.DBus.Properties.xml \
    dbus/org.bluez.obex.Session1.xml \
    dbus/org.bluez.obex.Transfer1.xml \
    dbus/org.bluez.obex.Client1.xml

SOURCES += src/main.cpp\
    src/obexagent.cpp \
    src/pushdialog.cpp \
    src/qxdgdir.cpp

HEADERS  += \
    src/obexagent.h \
    src/utils.h \
    src/pushdialog.h \
    src/qxdgdir.h

FORMS    += \
    ui/pushdialog.ui

RESOURCES += \
    resources.qrc

OTHER_FILES += \
    dbus/org.bluez.obex.Agent1.xml \
    dbus/org.bluez.obex.AgentManager1.xml \
    dbus/org.freedesktop.DBus.Properties.xml \
    dbus/org.bluez.obex.Session1.xml \
    dbus/org.bluez.obex.Transfer1.xml \
    dbus/org.bluez.obex.Client1.xml

#include "utils.h"
#include "obexagent.h"
#include "session1_interface.h"
#include "transfer1_interface.h"
#include "properties_interface.h"
#include <iostream>
#include <QMessageBox>
#include <QDBusConnection>
#include <QDBusInterface>
#include <QPixmap>
#include <QPainter>
#include <QFile>

#define SYSTRAY_ICON_SIZE 32

void debug_print_dbus_properties(QString service, QString path, QString interface){
    org::freedesktop::DBus::Properties p(service, path, QDBusConnection::sessionBus());

    std::cout << "Properties for " << qPrintable(path) << " (" << qPrintable(interface) << "):" << std::endl;
    QDBusPendingReply<QVariantMap> reply = p.GetAll(interface);
    check_reply(&reply, -99);
    QVariantMap props = reply.value();
    QMapIterator<QString, QVariant> i(props);
    while(i.hasNext()){
        i.next();
        std::cout << "\t" << qPrintable(i.key()) << "\t : "
                  << qPrintable(i.value().toString()) << std::endl;
    }
    std::cout << "." << std::endl;
}


ObexAgent::ObexAgent(QObject *parent) :
    QSystemTrayIcon(parent)
{
    new Agent1Adaptor(this);
    this->base_pixmap = QIcon(":/bluez/obex/agent/icon").pixmap(SYSTRAY_ICON_SIZE);

    this->setProgressIcon(0,100);
    //    this->show();

    this->current_session_path = "";
    this->dlg = new PushDialog();
}

void ObexAgent::test()
{
    this->setProgressIcon(50,100);
    this->show();
}

void ObexAgent::TransferUpdate(const QString &interface_name, const QVariantMap &changed_properties, const QStringList &invalidated_properties)
{
    if (changed_properties.value("Status","None").toString() == "complete")
    {

        // std::cout << "rename " << qPrintable(this->cache_path) << " , " << qPrintable(this->dest_path) << std::endl;
        QFile::rename(this->cache_path,this->dest_path);
        this->hide();
    }
    if (changed_properties.value("Status","None").toString() == "error")
    {
        this->hide();
    }
    this->setProgressIcon(
            changed_properties.value("Transferred",0).toULongLong(),
            this->file_size
    );
}

void ObexAgent::Release()
{
    // std::cout << "Release ()" << std::endl;
    this->dlg->hide();
    this->hide();
}

void ObexAgent::Cancel()
{
    // std::cout << "Cancel ()" << std::endl;
    this->dlg->hide();
    this->hide();
}

QString ObexAgent::AuthorizePush(const QDBusObjectPath &transfer)
{
    // std::cout << "ObexAgent::AuthorizePush (" << qPrintable(transfer.path()) << ")" << std::endl;

    this->show();

    this->tobj = new org::bluez::obex::Transfer1("org.bluez.obex", transfer.path(),QDBusConnection::sessionBus(), this);
    this->tprops = new org::freedesktop::DBus::Properties("org.bluez.obex", transfer.path(),QDBusConnection::sessionBus(), this);

    QString session_path = this->tobj->session().path();

    org::bluez::obex::Session1 *sobj = new org::bluez::obex::Session1("org.bluez.obex", session_path, QDBusConnection::sessionBus(), this);

    // get session object, to find root path.
    // TODO: trovare perchè non funziona recuperando l'oggetto sessione e leggendo la proprietà root()
    org::freedesktop::DBus::Properties session_prpos("org.bluez.obex", session_path, QDBusConnection::sessionBus(), this);
    QDBusPendingReply<QVariant> reply = session_prpos.Get(org::bluez::obex::Session1::staticInterfaceName(), "Root");
    check_reply(&reply, -10);
    QString root_path = reply.value().toString();

    //debug_print_dbus_properties("org.bluez.obex", transfer.path(), org::bluez::obex::Transfer1::staticInterfaceName());
    //debug_print_dbus_properties("org.bluez.obex", session_path, org::bluez::obex::Session1::staticInterfaceName());


    this->file_size = this->tobj->size();

    if (this->current_session_path != transfer.path()) {
        // nuova sessione,
        // collego gli eventi
        QObject::connect(this->tprops, &org::freedesktop::DBus::Properties::PropertiesChanged,
                         this, &ObexAgent::TransferUpdate);
        // richiediamo il permesso

        this->dlg->setText( QString("Richiesta di trasferimento del file ") + this->tobj->name() +
                            QString(" da ") + sobj->source() );
        QString iconname = "gnome-mime-" + this->tobj->type().replace("/","-");
        QString genericicon = this->tobj->type().split("/").at(0)+"-x-generic";
        this->dlg->setIcon( QIcon::fromTheme(iconname,
                                             QIcon::fromTheme(genericicon,
                                                              QIcon(":/bluez/obex/agent/icon"))
                                             )
                            );

        this->dlg->exec();

        switch (this->dlg->getButton()) {
        case PushDialog::Save:
            this->current_session_path = "";
            break;
        case PushDialog::SaveAll:
            this->current_session_path = transfer.path();
            break;
        default:
            // cancel
            this->tobj->Cancel();
            this->setProgressIcon(0,100);
            this->hide();
            this->current_session_path = "";
            // std::cout << "cancel" << std::endl;
            return QString("");
            break;
        }
    }

    this->cache_path = QDir(root_path).filePath(this->tobj->name());
    this->dest_path = this->dlg->save_path.filePath(this->tobj->name());
    // std::cout << "cache to: " << qPrintable(this->cache_path) << std::endl;
    // std::cout << "dest  to: " << qPrintable(this->dest_path) << std::endl;
    return this->cache_path;

}

void ObexAgent::setProgressIcon(quint64 val, quint64 max)
{

    int h = (int)( (val/(float)max) * (SYSTRAY_ICON_SIZE-2) );

    QPixmap icon(SYSTRAY_ICON_SIZE,SYSTRAY_ICON_SIZE);
    icon.setMask(this->base_pixmap.mask());

    QPainter painterIcon(&icon);
    painterIcon.drawPixmap(-2,0, this->base_pixmap);
    //painterIcon.fillRect(13,15,2,-h, QColor::fromRgb(44,84,141));
    painterIcon.setPen(QColor::fromRgb(44,84,141));
    painterIcon.drawLine(SYSTRAY_ICON_SIZE-3,SYSTRAY_ICON_SIZE-1,SYSTRAY_ICON_SIZE-3,SYSTRAY_ICON_SIZE-2-h);
    painterIcon.setPen(QColor::fromRgb(167,192,220));
    painterIcon.drawLine(SYSTRAY_ICON_SIZE-2,SYSTRAY_ICON_SIZE-1,SYSTRAY_ICON_SIZE-2,SYSTRAY_ICON_SIZE-2-h);
    painterIcon.end();

    this->setIcon( QIcon(icon) );
}

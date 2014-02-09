#ifndef OBEXAGENT_H
#define OBEXAGENT_H

#define AGENT_PATH "/com/kirgroup/obex/agent"
#define AGENT_SERVICE "com.kirgroup.obex.agent"

#include "agent1_adaptor.h"
#include "transfer1_interface.h"
#include "properties_interface.h"
#include "pushdialog.h"
#include <QObject>
#include <QDBusObjectPath>
#include <QDBusInterface>
#include <QSystemTrayIcon>
#include <QIcon>
#include <QBitmap>

class ObexAgent : public QSystemTrayIcon
{
    Q_OBJECT
public:
    explicit ObexAgent(QObject *parent = 0);
    void test();

signals:

public Q_SLOTS:
    void TransferUpdate(const QString &interface_name, const QVariantMap &changed_properties, const QStringList &invalidated_properties);
    void Release();
    void Cancel();
    QString AuthorizePush(const QDBusObjectPath &transfer);


private:
    org::freedesktop::DBus::Properties *tprops;
    org::bluez::obex::Transfer1 *tobj;
    void setProgressIcon(quint64 val, quint64 max);
    QPixmap base_pixmap;
    QString current_session_path;
    QString cache_path;
    QString dest_path;
    quint64 file_size;
    PushDialog *dlg;
};

#endif // OBEXAGENT_H

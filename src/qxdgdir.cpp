#include "qxdgdir.h"
#include <QProcessEnvironment>
#include <QDir>
#include <QFile>
#include <QTextStream>

#include <iostream>


QXDGDir::QXDGDir() : QDir()
{
}

QXDGDir::QXDGDir(const QString path) : QDir(path)
{
}


QXDGDir QXDGDir::fromXDG(QString name)
{
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    QString confpath = env.value("XDG_CONFIG_HOME");
    QString homepath = env.value("HOME");

    QFile data(QDir(confpath).filePath("user-dirs.dirs"));
    data.open(QFile::ReadOnly);
    QTextStream stream(&data);

    QString line;
    QString path;
    QStringList kv;


    const QChar COMMENT_CHR('#');
    const QChar SEPARAT_CHR('=');
    const QChar COMMA_CHR('"');

    QString requestName = "XDG_" + name + "_DIR";

    do {
        line = stream.readLine();

        if (!line.isNull()) {
            line = line.trimmed();
            if (line.at(0)!=COMMENT_CHR) {
                kv = line.split(SEPARAT_CHR);
                if (kv.at(0)==requestName){
                    path = kv.at(1);
                    path = path.remove(COMMA_CHR);
                    path = path.replace("$HOME", homepath);
                    data.close();
                    return QXDGDir(path);
                }
            }

        }
    } while (!line.isNull());
    data.close();
    return QXDGDir();
}

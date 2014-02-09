#ifndef QXDGDIR_H
#define QXDGDIR_H

#include <QDir>



class QXDGDir : public QDir
{
public:
    explicit QXDGDir();
    explicit QXDGDir(const QString path);
    static QXDGDir fromXDG(QString name);


};

#endif // QXDGDIR_H

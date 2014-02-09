#ifndef PUSHDIALOG_H
#define PUSHDIALOG_H

#include "qxdgdir.h"
#include <QDialog>
#include <QAbstractButton>
#include <QIcon>
namespace Ui {
class PushDialog;
}

class PushDialog : public QDialog
{
    Q_OBJECT
    Q_ENUMS(Buttons)
public:
    explicit PushDialog(QWidget *parent = 0);
    ~PushDialog();

    QXDGDir save_path;
    void setText(const QString text);
    void setIcon(QIcon icon);
    int getButton(){ return this->stdButton; }

    enum { Save=0x00000800, SaveAll=0x00001000, Cancel=0x00400000 };


public slots:
    void showBrowseDir();
    void buttonClicked(QAbstractButton* button);



private:
    Ui::PushDialog *ui;
    int stdButton;
};

#endif // PUSHDIALOG_H

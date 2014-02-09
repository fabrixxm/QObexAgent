#include "pushdialog.h"
#include "ui_pushdialog.h"

#include <QDir>
#include <QProcessEnvironment>
#include <QFileDialog>

PushDialog::PushDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PushDialog)
{
    ui->setupUi(this);
    this->save_path = QXDGDir::fromXDG("DOWNLOAD");
    this->ui->txt_filename->setText(this->save_path.path());
    this->stdButton = -1;
}

PushDialog::~PushDialog()
{
    delete ui;
}

void PushDialog::setText(const QString text)
{
    this->ui->label_question->setText(text);
}

void PushDialog::setIcon(QIcon icon)
{
    this->ui->mime_icon->setPixmap( icon.pixmap(48) );
}

void PushDialog::showBrowseDir()
{
    QString path = QFileDialog::getExistingDirectory(
                    this,
                    "Select folder",
                    this->ui->txt_filename->text()
                );
    this->ui->txt_filename->setText(path);
    this->save_path = QXDGDir(path);
}

void PushDialog::buttonClicked(QAbstractButton *button)
{
    this->stdButton = this->ui->buttonBox->standardButton(button);
}

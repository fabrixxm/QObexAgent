#ifndef OBEXAGENTDIALOG_H
#define OBEXAGENTDIALOG_H

#include <QDialog>

namespace Ui {
class ObexAgentDialog;
}

class ObexAgentDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ObexAgentDialog(QWidget *parent = 0);
    ~ObexAgentDialog();

private:
    Ui::ObexAgentDialog *ui;
};

#endif // OBEXAGENTDIALOG_H

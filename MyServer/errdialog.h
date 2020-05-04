//errDialog - класс для вывода сообщения об ошибке
#ifndef ERRDIALOG_H
#define ERRDIALOG_H

#include <QDialog>

namespace Ui {
class errDialog;
}

class errDialog : public QDialog
{
    Q_OBJECT

public:
    explicit errDialog(QString str, QWidget *parent = nullptr);
    ~errDialog();

private:
    Ui::errDialog *ui;
};

#endif // ERRDIALOG_H

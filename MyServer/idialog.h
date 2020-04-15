#ifndef IDIALOG_H
#define IDIALOG_H

#include <QDialog>

namespace Ui {
class idialog;
}

class idialog : public QDialog
{
    Q_OBJECT

public:
    explicit idialog(QString strLbl, QWidget *parent = nullptr);
    ~idialog();

private:
    Ui::idialog *ui;
};

#endif // IDIALOG_H

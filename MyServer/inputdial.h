//inputDial - класс для ввода данных пользователем

#ifndef INPUTDIAL_H
#define INPUTDIAL_H

#include <QDialog>

namespace Ui {
class inputDial;
}

class inputDial : public QDialog
{
    Q_OBJECT

public:
    explicit inputDial(QString str, QWidget *parent = nullptr);
    ~inputDial();
    QString sendString();

private slots:
    void on_lineText_textChanged(const QString &arg1);

private:
    Ui::inputDial *ui;
};

#endif // INPUTDIAL_H

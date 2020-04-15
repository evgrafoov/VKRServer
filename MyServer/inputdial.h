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
    explicit inputDial(QWidget *parent = nullptr);
    ~inputDial();

private:
    Ui::inputDial *ui;
};

#endif // INPUTDIAL_H

#include "inputdial.h"
#include "ui_inputdial.h"

inputDial::inputDial(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::inputDial)
{
    ui->setupUi(this);
}

inputDial::~inputDial()
{
    delete ui;
}

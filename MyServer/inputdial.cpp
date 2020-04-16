#include "inputdial.h"
#include "ui_inputdial.h"

inputDial::inputDial(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::inputDial)
{
    ui->setupUi(this);
    ui->btnOk->setShortcut(Qt::Key_Return);
    connect(ui->btnOk, SIGNAL( clicked() ), SLOT( accept() ) );
}

inputDial::~inputDial()
{
    delete ui;
}

QString inputDial::sendName()
{
    return ui->lineNewFile->text();
}

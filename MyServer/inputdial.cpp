#include "inputdial.h"
#include "ui_inputdial.h"

inputDial::inputDial(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::inputDial)
{
    ui->setupUi(this);
    ui->btnOk->setShortcut(Qt::Key_Return);
    connect(ui->btnOk, SIGNAL( clicked() ), SLOT( accept() ) );
    ui->btnOk->setEnabled(false);
}

inputDial::~inputDial()
{
    delete ui;
}

QString inputDial::sendName()
{
    return ui->lineNewFile->text();
}

void inputDial::on_lineNewFile_textChanged(const QString &arg1)
{
    if (arg1.trimmed() != "")
        ui->btnOk->setEnabled(true);
    else
        ui->btnOk->setEnabled(false);
}

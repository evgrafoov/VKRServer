#include "errdialog.h"
#include "ui_errdialog.h"

errDialog::errDialog(QString str, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::errDialog)
{
    ui->setupUi(this);
    ui->lblText->setText(str);
    connect(ui->btnOk, SIGNAL( clicked() ), SLOT( accept() ) );
}

errDialog::~errDialog()
{
    delete ui;
}

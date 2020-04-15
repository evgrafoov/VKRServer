#include "idialog.h"
#include "ui_idialog.h"

idialog::idialog(QString strLbl, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::idialog)
{
    ui->setupUi(this);
    ui->lblText->setText(strLbl);

    connect(ui->btnYes, SIGNAL( clicked() ), SLOT( accept() ) );
    connect(ui->btnNo, SIGNAL( clicked() ), SLOT( reject() ) );
}

idialog::~idialog()
{
    delete ui;
}

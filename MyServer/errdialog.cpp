// errDialog - класс для вывода сообщения об ошибке

#include "errdialog.h"
#include "ui_errdialog.h"

/*  errDialog - конструктор класса errDialog.
 *  Формальные параметры:
 *      str - строка с сообщением об ошибке;
 *      parent - объект для отображения интерфейса.
 */
errDialog::errDialog(QString str, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::errDialog)
{
    ui->setupUi(this);
    ui->lblText->setText(str);
    connect(ui->btnOk, SIGNAL( clicked() ), SLOT( accept() ) );
}

//  ~errDialog - деструктор класса errDialog.
errDialog::~errDialog()
{
    delete ui;
}

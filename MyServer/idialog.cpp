//idialog - класс для вывода сообщения для пользователя

#include "idialog.h"
#include "ui_idialog.h"

/*  idialog - конструктор класса idialog.
 *  Формальные параметры:
 *      strLbl - строка с сообщением для пользователя;
 *      parent - объект для отображения интерфейса.
 */
idialog::idialog(QString strLbl, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::idialog)
{
    ui->setupUi(this);
    ui->lblText->setText(strLbl);

    connect(ui->btnYes, SIGNAL( clicked() ), SLOT( accept() ) );
    connect(ui->btnNo, SIGNAL( clicked() ), SLOT( reject() ) );
}

//  ~idialog - деструктор класса idialog.
idialog::~idialog()
{
    delete ui;
}

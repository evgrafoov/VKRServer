/*  inputDial - класс для ввода данных пользователем
 * ***********************************************************
 *  Используемые методы:
 *
 *  sendString - функция, возвращающая введенную пользователем строку;
 *  on_lineText_textChanged - проверка на ввод данных пользователем.
 * ***********************************************************
 */
#include "inputdial.h"
#include "ui_inputdial.h"

/*  inputDial - конструктор класса inputDial.
 *  Формальные параметры:
 *      str - информация для ввода данных пользователем;
 *      parent - объект для отображения интерфейса.
 */
inputDial::inputDial(QString str, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::inputDial)
{
    ui->setupUi(this);
    ui->lblText->setText(str);
    ui->btnOk->setShortcut(Qt::Key_Return);
    connect(ui->btnOk, SIGNAL( clicked() ), SLOT( accept() ) );
    ui->btnOk->setEnabled(false);
}

//  ~inputDial - деструктор класса inputDial.
inputDial::~inputDial()
{
    delete ui;
}

//  sendString - функция, возвращающая введенную пользователем строку.
QString inputDial::sendString()
{
    return ui->lineText->text();
}

/*  on_lineText_textChanged - проверка на ввод данных пользователем.
 *  Формальный параметр:
 *      arg1 - текущее значение нового имени файла.
 */
void inputDial::on_lineText_textChanged(const QString &arg1)
{
    if (arg1.trimmed() != "")
        ui->btnOk->setEnabled(true);
    else
        ui->btnOk->setEnabled(false);
}

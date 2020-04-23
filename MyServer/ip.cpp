#include "ip.h"
#include "ui_ip.h"
#include <QRegExpValidator>
#include <QStringList>
#include <QKeyEvent>
#include <QDebug>
#include <QStyle>
#include <QStyleFactory>

/*  Ip - конструктор класса Ip.
 *  Формальный параметр:
 *      parent - объект для отображения интерфейса.
 *  Локальная переменная:
 *      ipValidator - установка ограничений на ввод IP-адреса.
 */
Ip::Ip(QWidget* parent) :
    QFrame(parent),
    ui(new Ui::Ip)
{
    ui->setupUi(this);
    QRegExpValidator *ipValidator =
            new QRegExpValidator(QRegExp("^(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])$"), this);
    ui->lineEdit_1->setValidator(ipValidator);
    ui->lineEdit_2->setValidator(ipValidator);
    ui->lineEdit_3->setValidator(ipValidator);
    ui->lineEdit_4->setValidator(ipValidator);

    ui->lineEdit_1->installEventFilter(this);
    ui->lineEdit_2->installEventFilter(this);
    ui->lineEdit_3->installEventFilter(this);
    ui->lineEdit_4->installEventFilter(this);
}

//  ~Ip - деструктор класса Ip.
Ip::~Ip()
{
    delete ui;
}

// setNightMode - установка темной темы.
void Ip::setNightMode()
{
    Ip::setStyleSheet("QFrame{border: 1px solid #333333;"
                      "background: #191919;}"
                      "QLineEdit{background: rgb(25, 25, 25);"
                      "border-radius: 3px;}"
                      "QLabel{border: 0px solid rgb(25, 25, 25);}");

}

// setLightMode - установка светлой темы.
void Ip::setLightMode()
{
    Ip::setStyleSheet("QFrame{border: 1px solid #ABABAB;"
                      "background: #FFFFFF;}"
                      "QFrame:!enabled{background: #EFEFEF;}"
                      "QLineEdit{background: #FFFFFF;}"
                      "QLineEdit:!enabled{background: #EFEFEF;}"
                      "QLabel{border: 0px solid white;"
                      "color: black;}");

}

/*  on_lineEdit_1_textChanged - изменение текста в первом поле ввода.
 *  Формальный параметр:
 *      arg1 - текущее значение первого поля ввода.
 */
void Ip::on_lineEdit_1_textChanged(const QString &arg1)
{
    if(arg1.length() == 3 || (arg1.toInt() > 25 && arg1.toInt() < 100))
    {
        ui->lineEdit_2->setFocus();
        ui->lineEdit_2->selectAll();
    }
}

/*  on_lineEdit_2_textChanged - изменение текста во втором поле ввода.
 *  Формальный параметр:
 *      arg1 - текущее значение второго поля ввода.
 */
void Ip::on_lineEdit_2_textChanged(const QString &arg1)
{
    if(arg1.length() == 3 || (arg1.toInt() > 25 && arg1.toInt() < 100))
    {
        ui->lineEdit_3->setFocus();
        ui->lineEdit_3->selectAll();
    }
    else if(arg1.length() == 0)
    {
        ui->lineEdit_1->setFocus();
        ui->lineEdit_1->selectAll();
    }
}

/*  on_lineEdit_3_textChanged - изменение текста в третьем поле ввода.
 *  Формальный параметр:
 *      arg1 - текущее значение третьего поля ввода.
 */
void Ip::on_lineEdit_3_textChanged(const QString &arg1)
{
    if(arg1.length()== 3 || (arg1.toInt() > 25 && arg1.toInt() < 100))
    {
        ui->lineEdit_4->setFocus();
        ui->lineEdit_4->selectAll();
    }
    else if(arg1.length() == 0)
    {
        ui->lineEdit_2->setFocus();
        ui->lineEdit_2->selectAll();
    }
}

/*  on_lineEdit_4_textChanged - изменение текста в четвертом поле ввода.
 *  Формальный параметр:
 *      arg1 - текущее значение четверого поля ввода.
 */
void Ip::on_lineEdit_4_textChanged(const QString &arg1)
{
    if(arg1.length() == 0)
    {
        ui->lineEdit_3->setFocus();
        ui->lineEdit_3->selectAll();
    }
}

//  getIP - получение значения IP-адреса.
QString Ip::getIP()
{
    if (ui->lineEdit_1->text().isEmpty() ||
            ui->lineEdit_2->text().isEmpty() ||
            ui->lineEdit_3->text().isEmpty() ||
            ui->lineEdit_4->text().isEmpty())
        return "1";
    else
        return ui->lineEdit_1->text() + "." +
                ui->lineEdit_2->text() + "." +
                ui->lineEdit_3->text() + "." +
                ui->lineEdit_4->text();
}

//  setEmpty - очистка всех полей ввода.
void Ip::setEmpty()
{
    ui->lineEdit_1->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();
    ui->lineEdit_4->clear();
}

/*  setOnlyRead - установка режима "только для чтения".
 *  Формальный параметр:
 *      checked - переменная для установки режима "только для чтения".
 */
void Ip::setOnlyRead(bool checked)
{
    ui->lineEdit_1->setReadOnly(checked);
    ui->lineEdit_2->setReadOnly(checked);
    ui->lineEdit_3->setReadOnly(checked);
    ui->lineEdit_4->setReadOnly(checked);
}

/*  setIp - установка значения IP-адреса.
 *  Формальный параметр:
 *      allIP - значение IP-адреса.
 */
void Ip::setIp(QString allIP)
{
    QStringList list = allIP.split('.');
    ui->lineEdit_1->setText(list[0]);
    ui->lineEdit_2->setText(list[1]);
    ui->lineEdit_3->setText(list[2]);
    ui->lineEdit_4->setText(list[3]);
}

/*  eventFilter - обработка нажатий клавиш.
 *  Формальные параметры:
 *      obj - объект-приёмник;
 *      event - обработчик событий;
 *  Локальные переменные:
 *      lineEdit - ;
 *      num - ;
 *      keyEvent - .
 */
bool Ip::eventFilter(QObject *obj, QEvent *event) {
    QLineEdit *lineEdit = findChild<QLineEdit *>(obj->objectName());
    if(event->type() == QEvent::KeyPress)
    {
        int num = obj->objectName().at(9).toLatin1();
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        switch(keyEvent->key())
        {
            case Qt::Key_Left:
            {
                if (num != 49 && lineEdit->cursorPosition() == 0)
                {
                    num = num - 1;
                    QLineEdit *lineEdit = findChild<QLineEdit *>("lineEdit_" + (QString)num);
                    lineEdit->setFocus();
                    lineEdit->selectAll();
                }
            }
            break;

            case Qt::Key_Right:
            {
                if (num != 52  && (lineEdit->cursorPosition() == lineEdit->text().length()))
                {
                    num = num + 1;
                    QLineEdit *lineEdit = findChild<QLineEdit *>("lineEdit_" + (QString)num);
                    lineEdit->setFocus();
                    lineEdit->selectAll();
                }
            }
            break;

            case Qt::Key_Backspace:
            {
                if (num != 49 && lineEdit->text().isEmpty())
                {
                    num = num - 1;
                    QLineEdit *lineEdit = findChild<QLineEdit *>("lineEdit_" + (QString)num);
                    lineEdit->setFocus();
                    lineEdit->selectAll();
                }
                break;
            }
        }
    }
    return false;
}



#include "ip.h"
#include "ui_ip.h"
#include <QRegExpValidator>
#include <QStringList>
#include <QKeyEvent>
#include <QDebug>
#include <QStyle>
#include <QStyleFactory>

Ip::Ip(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::Ip)
{
    ui->setupUi(this);

    QString ipRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
    QRegExp ipRegex ("^" + ipRange + "$");
    QRegExpValidator *ipValidator = new QRegExpValidator(ipRegex, this);
    ui->lineEdit_1->setValidator(ipValidator);
    ui->lineEdit_2->setValidator(ipValidator);
    ui->lineEdit_3->setValidator(ipValidator);
    ui->lineEdit_4->setValidator(ipValidator);

    ui->lineEdit_1->installEventFilter(this);
    ui->lineEdit_2->installEventFilter(this);
    ui->lineEdit_3->installEventFilter(this);
    ui->lineEdit_4->installEventFilter(this);
}

Ip::~Ip()
{
    delete ui;
}

void Ip::setNightMode()
{
    Ip::setStyleSheet("QFrame{border: 1px solid #333333;"
                      "background: #191919;}"
                      "QLineEdit{background: rgb(25, 25, 25);"
                      "border-radius: 3px;}"
                      "QLabel{border: 0px solid rgb(25, 25, 25);}");

}

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

void Ip::on_lineEdit_1_textChanged(const QString &arg1)
{
    if(arg1.length() == 3 || (arg1.toInt() > 25 && arg1.toInt() < 100))
    {
        ui->lineEdit_2->setFocus();
        ui->lineEdit_2->selectAll();
    }
}

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

void Ip::on_lineEdit_4_textChanged(const QString &arg1)
{
    if(arg1.length() == 0)
    {
        ui->lineEdit_3->setFocus();
        ui->lineEdit_3->selectAll();
    }
}

QString Ip::getIP()
{
    if (ui->lineEdit_1->text().isEmpty() || ui->lineEdit_2->text().isEmpty() || ui->lineEdit_3->text().isEmpty() || ui->lineEdit_4->text().isEmpty())
        return "1";
    else
        return ui->lineEdit_1->text() + "." + ui->lineEdit_2->text() + "." + ui->lineEdit_3->text() + "." + ui->lineEdit_4->text();
}

void Ip::setEmpty()
{
    ui->lineEdit_1->setText("");
    ui->lineEdit_2->setText("");
    ui->lineEdit_3->setText("");
    ui->lineEdit_4->setText("");
}

void Ip::setOnlyRead(bool checked)
{
    ui->lineEdit_1->setReadOnly(checked);
    ui->lineEdit_2->setReadOnly(checked);
    ui->lineEdit_3->setReadOnly(checked);
    ui->lineEdit_4->setReadOnly(checked);
}

void Ip::setIp(QString allIP)
{
    QStringList list = allIP.split('.');
    ui->lineEdit_1->setText(list[0]);
    ui->lineEdit_2->setText(list[1]);
    ui->lineEdit_3->setText(list[2]);
    ui->lineEdit_4->setText(list[3]);
}

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

bool Ip::InFocus()
{
    if (ui->lineEdit_1->hasFocus() || ui->lineEdit_2->hasFocus() ||
        ui->lineEdit_3->hasFocus() || ui->lineEdit_4->hasFocus())
        return true;
    return false;
}

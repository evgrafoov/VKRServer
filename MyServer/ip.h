#ifndef IP_H
#define IP_H

#include <QFrame>

namespace Ui {
class Ip;
}

class Ip : public QFrame
{
    Q_OBJECT

public:
    explicit Ip(QWidget *parent = 0);
    ~Ip();
    QString getIP();
    void setEmpty();
    void setOnlyRead(bool checked);
    void setIp(QString allIP);
    void setNightMode();
    void setLightMode();

private slots:
    void on_lineEdit_1_textChanged(const QString &arg1);

    void on_lineEdit_2_textChanged(const QString &arg1);

    void on_lineEdit_3_textChanged(const QString &arg1);

    void on_lineEdit_4_textChanged(const QString &arg1);

protected:
    virtual bool eventFilter(QObject *obj, QEvent *event);

private:
    Ui::Ip *ui;
};

#endif // IP_H

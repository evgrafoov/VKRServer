#ifndef MYSERVER_H
#define MYSERVER_H

#include <QMainWindow>
#include <QHostAddress>
#include <QTcpSocket>
#include <QByteArray>
#include <QDir>
#include <QListWidgetItem>
#include <QtXml>
#include <QStandardItemModel>
#include <QDateTime>

class QTcpServer;
class QTextEdit;
class QListWidget;
class QLineEdit;
class QTcpSocket;

QT_BEGIN_NAMESPACE
namespace Ui { class MyServer; }
QT_END_NAMESPACE

struct client
{
    QString name;
    QHostAddress ip;
    QTcpSocket* socket;
    bool state;
};

class MyServer : public QMainWindow
{
    Q_OBJECT

private:
    void sendToAllClients(                    const QString& str                      );
    void sendToSpecClient(QTcpSocket *pSender, QTcpSocket* pSocket, const QString& str);
    void sendToSpecClient(         QTcpSocket *pSocket, const QString& str            );
    void sendService     (                    QTcpSocket* pSocket                     );
    void sendListClient  (                                                            );
    void addArchChat     (        QString sender, QString receiver, QString str       );
    void openUserSett    (                                                            );
    void saveUserSett    (                                                            );

signals:
    void onFinishRecieved();

private:
    QTcpServer* m_ptcpServer;
    quint16 m_nNextBlockSize;
    QMap<quint16, client> clients;
    QMap<QString, QString> listC;
    QString fileName;
    quint64 fileSize;
    QFile target;
    QString sFromSave, rFromSave, mFromSave;
    QDate dateForChat;
    quint16 identity             = 0;
    QString filePath             = QDir::homePath() + "/Desktop/";
    QDir chatPath                = QDir::homePath() + "/Desktop/ServerSettings/ChatArchive/";
    bool checkBtn                = false; //show
    bool checkTheme              = true;
    QString savePath             = QDir::homePath() + "/Desktop/listClient.xml";
    QString fileUserSet          = QDir::homePath() + "/Desktop/ServerSettings/setting.xml";
    bool checkFile               = false;
    QStandardItemModel *model    = new QStandardItemModel;
    QStringList horizontalHeader = {"Отправитель", "Получатель", "Сообщение"};
    qint64 idArchMsg             = 0;

public:
    MyServer(int nPort, QWidget* pwgt = 0);
    ~MyServer();

public slots:
    virtual void newClient();
    void readClient();

private slots:

    void on_btnGoMsg_clicked();

    void on_listClient_currentItemChanged(QListWidgetItem *current);

    void on_btnAddClient_clicked();

    void on_actSave_triggered();

    void on_actExit_triggered();

    void on_btnDelClient_clicked();

    void on_actOpen_triggered();

    void on_actSaveAs_triggered();

    void on_btnHideInfo_clicked();

    void on_cmbSortClients_currentIndexChanged(const QString &arg1);

    void on_chkUnblock_toggled(bool checked);

    void on_btnClearLog_clicked();

    void on_btnExecProc_clicked();

    void on_btnChooseFile_clicked();

    void on_actMinimize_triggered();

    void on_btnClearChat_clicked();

    void on_actChangeTheme_triggered();

    void on_lineFilePath_editingFinished();

    void on_txtLogClient_textChanged();

    void on_txtChat_textChanged();

private:
    Ui::MyServer *ui;
};
#endif // MYSERVER_H

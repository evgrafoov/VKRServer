/*  Отчет по практике по профессиональному модулю «ПМ.01 Разработка программных
 *  модулей программного обеспечения для компьютерных систем».
 *  Название: Practica2.
 *  Разработал: Евграфов Павел Александрович, группа ТМП-81.
 *  Дата и номер версии: 13.02.2020 v2.0.
 *  Язык программирования: C++.
 *  Краткое описание:
 *      Данный модуль позволяет взаимодействовать клиентам с сервером.
 *  Задание:
 *      Разработать серверную часть в модели «клиент-сервер». Необходимо реализовать прием
 *      сообщений и файлов от клиентов.
 *  Используемые методы:
 *      setNightMode – установка темной темы;
 *      setLightMode – установка темной темы;
 *      slotNewConnection – подключение нового клиента;
 *	slotReadClient – обработка запросов клиента;
 *	sendToClient – отправка данных клиену;
 *	on_btnFilePath_clicked – выбор пути сохранения файлов;
 *	on_cmbTheme_currentIndexChanged – смена темы;
 *	on_btnGoMsg_clicked – отправка сообщения клиентам.
 *  Используемые переменные:
 *      name – имя клиента;
 *      ip – IP адрес клиента;
 *      state – состояние клиента;
 *	socket – сокет клиента;
 *	m_ptcpServer – управление сервером;
 *	m_nNextBlockSize – длина следующего полученного от сокета блока;
 *	clients – контейнер, содержащий данные о клиентах;
 *	identity – идентификатор для хранения данных о клиентах;
 *	filepath – путь сохранения файлов;
 *	pClientSocket – сокет текущего клиента;
 *	in – чтение данных из сокета;
 *	mode – тип сообщения;
 *	time – время отправки сообщения;
 *	str – сообщение от пользователя;
 *	strMessage – данные, отправляемые клиенту;
 *	fileName – имя файла;
 *	line – считывание очередной части файла из сокета;
 *	target – конечный путь сохранения файлов;
 *	arrBlock – блок для отправки данных клиенту;
 *	out – записть данных в сокет;
 *	dir – директория сохранения файлов;
 *	it – итератор для контейнера.
*/

#include "myserver.h"
#include "ui_myserver.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QTime>
#include <QDir>
#include <QFile>
#include <QMap>
#include <QtXml>
#include "switch.h"
#include <QStyleFactory>
#include <QStandardItem>
#include <QStandardItemModel>
#include <idialog.h>
#include <errdialog.h>


/*  MyServer - конструктор
 *  Формальные параметры:
 *      nPort - порт;
 *      pwgt - объект для отображения интерфейса.
 */
MyServer::MyServer(int nPort, QWidget* pwgt): QMainWindow(pwgt), m_nNextBlockSize(0), ui(new Ui::MyServer)
{
    ui->setupUi(this);
    QApplication::setFont(QFont("Verdana", 11));
    m_ptcpServer = new QTcpServer(this);
    if (!m_ptcpServer->listen(QHostAddress::Any, nPort))
    {
        errDialog err("Не удается запустить сервер: \n" + m_ptcpServer->errorString());
        err.setWindowTitle("Ошибка сервера");
        err.exec();
        m_ptcpServer->close();
        return;
    }
    connect(m_ptcpServer, SIGNAL (newConnection()), this, SLOT(newClient()));
    ui->btnGoMsg->setShortcut(Qt::Key_Return);

    ui->lineFilePath->setText(filePath);
    openUserSett();

    ui->btnAddClient->setToolTip("Добавить клиента");
    ui->btnDelClient->setToolTip("Удалить клиента");
    ui->btnExecProc->setToolTip("Запустить процесс");

    ui->txtChat->setReadOnly(true);
    ui->btnHideInfo->setText("Показать информацию");
    ui->chkUnblock->setDown(true);
    ui->vgroupInfo->setEnabled(false);
    ui->txtLogClient->setReadOnly(true);
    ui->txtClientInfo->setVisible(false);
    ui->txtClientInfo->setReadOnly(true);
    ui->toolBarLeft->setIconSize(QSize(40, 40));
    ui->toolBarRight->setIconSize(QSize(30, 20));
    ui->cmbSortClients->addItem("↑ Имя");
    ui->cmbSortClients->addItem("↓ Имя");
    ui->cmbSortClients->setCurrentIndex(-1);
    ui->listClient->setIconSize(QSize(35, 35));
    ui->lineInText->setPlaceholderText("Напишите сообщение...");
    ui->txtClientInfo->setFixedHeight(100);

    model->setHorizontalHeaderLabels(horizontalHeader);

    ui->lineIP->installEventFilter(this);
    ui->lineName->installEventFilter(this);
    ui->lineInText->installEventFilter(this);
    ui->lineFilePath->installEventFilter(this);
}

// ~MyServer - деструктор.
MyServer::~MyServer()
{
    delete ui;
}

/* slotNewConnection - подключение нового клиента.
 * Локальные переменные:
 *      pClientSocket - сокет текущего клиента.
 */
void MyServer::newClient()
{
    bool flag = false;
    QTcpSocket* pClientSocket = m_ptcpServer->nextPendingConnection();
    QMap <quint16, client>::iterator it = clients.begin();
    for (; it != clients.end(); it++)
    {
        if (pClientSocket->peerAddress().isEqual(it.value().ip))
        {
            it.value().socket = pClientSocket;

            connect (pClientSocket, SIGNAL(disconnected()), pClientSocket, SLOT(deleteLater()));
            connect (pClientSocket, SIGNAL(readyRead()), this, SLOT(readClient()));
            it.value().state = true;
            sendListClient();
            for(int i=0;i<ui->listClient->count();i++)
            {
                QListWidgetItem* item= ui->listClient->item(i);
                if(item->text()==it.value().name)
                {
                    ui->listClient->setCurrentItem(item);
                    QIcon ic(":/rec/img/accept.png");
                    item->setIcon(ic);
                }
            }
            ui->txtLogClient->append("Клиент " + it.value().name + " успешно подключился");
            flag = true;
            sendToSpecClient(pClientSocket, "Вы успешно подключились к серверу!");
            if(ui->txtClientInfo->isVisible())
            {
                ui->txtClientInfo->clear();
                ui->txtClientInfo->append("Имя клиента: " + it.value().name);
                ui->txtClientInfo->append("IP-адрес: " + it.value().ip.toString());
                ui->txtClientInfo->append("Состояние: Подключен");
            }
            return;
        }
    }
    if (!flag)
    {
        ui->txtLogClient->append("Ошибка! Клиент не подключился! Ip адрес: " + pClientSocket->peerAddress().toString());
    }
}

/* slotReadClient - обработка запросов клиента.
 * Локальные переменные:
 *      pClientSocket - сокет текущего клиента;
 *      in - чтение данных из сокета;
 *      mode - тип сообщения;
 *      time - время отправки сообщения;
 *      str - сообщение от пользователя;
 *      name - имя клиента;
 *      strMessage - данные, отправляемые клиенту;
 *      fileName - имя файла;
 *      line - считывание очередной части файла из сокета;
 *      target - конечный путь сохранения файлов.
 */
void MyServer::readClient()
{
    QTcpSocket* pClientSocket = (QTcpSocket*)sender();
    QDataStream in(pClientSocket);
    quint16 mode;
    in.setVersion(QDataStream::Qt_5_3);
    QTime time;
    QString str, name, clientReciever;

    for (;;)
    {
        if (!m_nNextBlockSize)
        {
            if (pClientSocket->bytesAvailable() < sizeof(quint32))
            {
                break;
            }
            in >> m_nNextBlockSize;
        }
        if (pClientSocket->bytesAvailable() < m_nNextBlockSize)
        {
            break;
        }
        in >> mode;             // Выбор типа взаимодействия

        QMap <quint16, client>::iterator it = clients.begin();
        for (; it != clients.end(); it++)
        {
            if (it.value().socket == pClientSocket)
            {
                name = it.value().name;
                break;
            }
        }
        switch (mode)
        {
        case 11: //Сообщение
        {
            in >> time >> str >> clientReciever;
            ui->txtChat->append(time.toString() + " " + name + ": " + str);
            m_nNextBlockSize = 0;
            if (clientReciever == "Все")
            {
                sendToAllClients(name + ": " + str);
            }
            else
            {
                QMap <quint16, client>::iterator it = clients.begin();
                for (; it != clients.end(); it++)
                {
                    if (it.value().name == clientReciever)
                    {
                        sendToSpecClient(pClientSocket, it.value().socket, name + ": " + str);
                        break;
                    }
                }
            }
            addArchChat(name, clientReciever, str);
        }
            break;
        case 22:
        {
            on_lineFilePath_editingFinished();
            in >> fileName >> fileSize;
            target.setFileName(filePath + fileName);
            while (target.exists())
            {
                idialog dial("Файл с таким именем уже существует! Удалить существующий файл?");
                dial.setWindowTitle("Информация");
                if (dial.exec() == QDialog::Accepted)
                {
                    target.remove();
                }
                else if (dial.exec() == QDialog::Rejected)
                {
                    bool flag;
                    QInputDialog inpPath;
                    QString text = inpPath.getText(0, tr("Ввод информации"), "Новое имя файла", QLineEdit::Normal, "Файл", &flag);
                    if (flag && !text.trimmed().isEmpty())
                    {
                        QString exp = fileName.split(".").last();
                        fileName = text + "." + exp;
                        target.setFileName(filePath + fileName);
                    }
                    else
                    {
                        sendToSpecClient(pClientSocket, "Ошибка! Повторите отправку файла " + fileName);
                        //Отправить, что нужно прервать отправку
                        break;
                    }
                }
                else
                {
                    sendToSpecClient(pClientSocket, "Ошибка! Повторите отправку файла " + fileName);
                    //Отправить, что нужно прервать отправку
                    break;
                }
            }
            if (!target.open(QIODevice::Append))
            {
                errDialog err("Невозможно открыть файл!");
                err.setWindowTitle("Ошибка файла");
                err.exec();
                return;
            }
            m_nNextBlockSize = 0;
            ui->txtChat->append("Клиент " + name + " начал отправку файла " + fileName);
            sendService(pClientSocket);
        }
            break;

        case 33:
        {
            QByteArray line = pClientSocket->read(63000);
            target.write(line);
            //emit onFinishRecieved();
            m_nNextBlockSize = 0;
            sendService(pClientSocket);
        }
            break;

        case 44:
        {
            if (fileSize == (quint64)target.size())
            {
                ui->txtChat->append("Файл " + fileName + " успешно получен!");
                //emit onFinishRecieved();
                m_nNextBlockSize = 0;
                sendToSpecClient(pClientSocket, "Файл " + fileName + " успешно отправлен");
            }
            else
            {
                ui->txtChat->append("Файл " + fileName + " получен некорректно. Он будет удален");
                target.remove();
                //emit onFinishRecieved();
                m_nNextBlockSize = 0;
                sendToSpecClient(pClientSocket, "Файл " + fileName + " отправлен некорректно. Повторите отправку");
            }
            target.close();
        }
            break;
        case 55:
        {
            target.remove();
            ui->txtChat->append("Ошибка в отправке файла " + fileName);
            //emit onFinishRecieved();
            m_nNextBlockSize = 0;
            sendToSpecClient(pClientSocket, "Ошибка! Повторите отправку файла " + fileName);
        }
            break;
        case 66:
        {
            QMap <quint16, client>::iterator it = clients.begin();
            for (; it != clients.end(); it++)
            {
                if (pClientSocket == it.value().socket)
                {
                    QList<QListWidgetItem*> items_list; //Клиент, который отключился
                    items_list.append(ui->listClient->findItems(it.value().name, Qt::MatchFlag::MatchExactly));
                    for (int i = 0; i < items_list.size(); i++)
                    {
                        int itemRow = ui->listClient->row(items_list[i]);
                        QListWidgetItem *item = ui->listClient->item(itemRow);
                        QIcon icon = QIcon(":/rec/img/decline.png");
                        item->setIcon(icon);
                        ui->txtLogClient->append("Клиент " + it.value().name + " отключился");
                        if(ui->txtClientInfo->isVisible())
                        {
                            ui->txtClientInfo->clear();
                            ui->txtClientInfo->append("Имя клиента: " + it.value().name);
                            ui->txtClientInfo->append("IP-адрес: " + it.value().ip.toString());
                            ui->txtClientInfo->append("Состояние: Отключен");
                        }
                        it.value().state = false;
                    }
                    break;
                }
            }
            pClientSocket->disconnectFromHost();
        }
            break;
        }
    }
}

/* sendToClient - отправка данных клиенту.
 * Формальные параметры:
 *      pSocket - сокет клиента;
 *      str - строка, отправляемая клиенту.
 * Локальные переменные:
 *      arrBlock - блок для отправки данных клиенту;
 *      out - запить данных в сокет.
 */
void MyServer::sendToAllClients(const QString &str)
{
    quint16 typeMsg = 0;
    QByteArray arrBlock;
    QDataStream out (&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_3);
    out << quint16(0) << typeMsg << QTime::currentTime() << str;
    out.device() -> seek(0);
    out << (quint16)(arrBlock.size() - sizeof(quint16));
    QMap <quint16, client>::iterator it = clients.begin();
    for (; it != clients.end(); it++)
    {
        if (it.value().state)
        {
            it.value().socket->write(arrBlock);
        }
    }
}

void MyServer::sendToSpecClient(QTcpSocket *pSender, QTcpSocket *pReciever, const QString &str)
{
    quint16 typeMsg = 0;
    QByteArray arrBlock;
    QDataStream out (&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_3);
    out << quint16(0) << typeMsg << QTime::currentTime() << str;
    out.device() -> seek(0);
    out << (quint16)(arrBlock.size() - sizeof(quint16));
    QMap <quint16, client>::iterator it = clients.begin();
    for (; it != clients.end(); it++)
    {
        if (it.value().socket == pSender && it.value().state)
            pSender->write(arrBlock);
        else if (it.value().socket == pReciever && it.value().state)
            pReciever->write(arrBlock);
    }
}

void MyServer::sendToSpecClient(QTcpSocket *pSender, const QString &str)
{
    quint16 typeMsg = 0;
    QByteArray arrBlock;
    QDataStream out (&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_3);
    out << quint16(0) << typeMsg << QTime::currentTime() << str;
    out.device() -> seek(0);
    out << (quint16)(arrBlock.size() - sizeof(quint16));
    QMap <quint16, client>::iterator it = clients.begin();
    for (; it != clients.end(); it++)
    {
        if (it.value().socket == pSender && it.value().state)
            pSender->write(arrBlock);
    }
}

void MyServer::sendService(QTcpSocket* pSocket)
{
    quint16 typeMsg = 1;
    QByteArray arrBlock;
    QDataStream out (&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_3);
    out << quint16(0) << typeMsg;
    out.device() -> seek(0);
    out << (quint16)(arrBlock.size() - sizeof(quint16));
    QMap <quint16, client>::iterator it = clients.begin();
    for (; it != clients.end(); it++)
    {
        if (it.value().socket == pSocket && it.value().state)
        {
            pSocket->write(arrBlock);
            return;
        }
    }
}

void MyServer::sendListClient()
{

    quint16 typeMsg = 3;
    QByteArray arrBlock;
    QDataStream out (&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_3);
    out << quint16(0) << typeMsg << listC;
    out.device() -> seek(0);
    out << (quint16)(arrBlock.size() - sizeof(quint16));
    QMap <quint16, client>::iterator it = clients.begin();
    for (; it != clients.end(); it++)
    {
        if (it.value().state)
            it.value().socket->write(arrBlock);
    }
}

void MyServer::addArchChat(QString sender, QString receiver, QString str)
{
    QList<QString> strings = {sender, receiver, str};
    for(int row=0; row <=2; row++)
    {
        QStandardItem* item = new QStandardItem(QString(strings[row]));
        model->setItem(idArchMsg, row, item);
    }
    ui->tblArchChat->setModel(model);
    ui->tblArchChat->resizeRowsToContents();
    ui->tblArchChat->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->tblArchChat->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->tblArchChat->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    idArchMsg++;
}

/* on_btnGoMsg_clicked - отправка сообщения клиентам.
 * Локальные переменные:
 *      strMessage - данные, отправляемые клиентам;
 *      it - итератор для контейнера.
 */
void MyServer::on_btnGoMsg_clicked()
{
    QString strMessage = ui->lineInText->text();
    if ( !strMessage.trimmed().isEmpty())
    {
        ui->txtChat->append(QTime::currentTime().toString() + " СЕРВЕР: " + strMessage);
        sendToAllClients("СЕРВЕР: " + strMessage);
        ui->lineInText->setText("");
        addArchChat("Сервер", "Все", strMessage);
    }
    else
    {
        ui->lineInText->clear();
    }
}

void MyServer::on_listClient_currentItemChanged(QListWidgetItem *current)
{
    if (ui->listClient->count() > 1)
    {
        ui->txtClientInfo->clear();
        QMap <quint16, client>::iterator it = clients.begin();
        for (; it != clients.end(); it++)
        {
            if (it.value().name == current->text())
            {
                ui->txtClientInfo->append("Имя клиента: " + it.value().name);
                ui->txtClientInfo->append("IP-адрес: " + it.value().ip.toString());
                ui->txtClientInfo->append("Состояние:" + (QString)(it.value().state == true ? "Подключен" : "Отключен"));
                return;
            }
        }
    }
}

void MyServer::on_btnAddClient_clicked()
{
    if (ui->lineIP->getIP().trimmed() != "" && ui->lineName->text().trimmed() != "")
    {
        if (ui->lineIP->getIP() != "1")
        {
            if (!listC.values().contains(ui->lineIP->getIP()) && !listC.keys().contains(ui->lineName->text()))
            {
                identity++;
                clients[identity].name = ui->lineName->text();
                clients[identity].ip = (QHostAddress)ui->lineIP->getIP();
                clients[identity].state = false;
                ui->listClient->addItem(new QListWidgetItem(QIcon(":/rec/img/decline.png"), ui->lineName->text()));
                listC[ui->lineName->text()] = ui->lineIP->getIP();
                ui->lineIP->setEmpty();
                ui->lineName->setText("");
                on_cmbSortClients_currentIndexChanged(ui->cmbSortClients->currentText());
                sendListClient();
            }
            else
            {
                errDialog err("Введите уникальные значения!");
                err.setWindowTitle("Ошибка");
                err.exec();
            }
        }
        else
        {
            errDialog err("Заполните корректно поле IP адреса!");
            err.setWindowTitle("Ошибка");
            err.exec();
        }
    }
    else
    {
        errDialog err("Заполните все поля!");
        err.setWindowTitle("Ошибка");
        err.exec();
    }
}

void MyServer::on_actSave_triggered()
{
    idialog dial("Вы действительно хотите сохранить список клиентов?");
    dial.setWindowTitle("Сохранение");
    if (dial.exec() == QDialog::Accepted)
    {
        QFile file(savePath);
        if (file.open(QIODevice::WriteOnly))
        {
            QXmlStreamWriter xmlWriter(&file);
            xmlWriter.setAutoFormatting(true); // Устанавливаем автоформатирование текста
            xmlWriter.writeStartDocument();     // Запускаем запись в документ
            xmlWriter.writeStartElement("listclients");   // Записываем первый элемент с его именем.
            QMap <quint16, client>::iterator it = clients.begin();
            for (; it != clients.end(); it++)
            {
                xmlWriter.writeStartElement("client");

                xmlWriter.writeStartElement("name");
                xmlWriter.writeCharacters(it.value().name);
                xmlWriter.writeEndElement();

                xmlWriter.writeStartElement("ip");
                xmlWriter.writeCharacters(it.value().ip.toString());
                xmlWriter.writeEndElement();

                xmlWriter.writeEndElement();
            }
            xmlWriter.writeEndElement();
            xmlWriter.writeEndDocument();
            file.close();
        }
        else
        {
            errDialog err("Не удалось открыть файл!");
            err.setWindowTitle("Ошибка");
            err.exec();
        }
    }
}

void MyServer::on_actSaveAs_triggered()
{
    savePath = QFileDialog::getSaveFileName(this,tr("Выберите директорию для сохранения"));
    if (savePath.trimmed() != "")
    {
        QFile file(savePath);
        if (file.open(QIODevice::WriteOnly))
        {
            QXmlStreamWriter xmlWriter(&file);
            xmlWriter.setAutoFormatting(true); // Устанавливаем автоформатирование текста
            xmlWriter.writeStartDocument();     // Запускаем запись в документ
            xmlWriter.writeStartElement("listclients");   // Записываем первый элемент с его именем.
            QMap <quint16, client>::iterator it = clients.begin();
            for (; it != clients.end(); it++)
            {
                xmlWriter.writeStartElement("client");

                xmlWriter.writeStartElement("name");
                xmlWriter.writeCharacters(it.value().name);
                xmlWriter.writeEndElement();

                xmlWriter.writeStartElement("ip");
                xmlWriter.writeCharacters(it.value().ip.toString());
                xmlWriter.writeEndElement();

                xmlWriter.writeEndElement();
            }
            xmlWriter.writeEndElement();
            xmlWriter.writeEndDocument();
            file.close();
        }
        else
        {
            errDialog err("Не удалось открыть файл!");
            err.setWindowTitle("Ошибка");
            err.exec();
        }
    }
}

void MyServer::on_actExit_triggered()
{
    idialog dial("Вы действительно хотите закрыть программу?");
    dial.setWindowTitle("Выход из программы");
    if (dial.exec() == QDialog::Accepted)
    {
        saveUserSett();
        QMap <quint16, client>::iterator it = clients.begin();
        for (; it != clients.end(); it++)
        {
            if(it.value().state)
                it.value().socket->close();
        }
        clients.clear();
        listC.clear();
        QApplication::quit();
    }
}

void MyServer::on_btnDelClient_clicked()
{
    if (ui->listClient->count() > 0)
    {
        if (ui->listClient->currentRow() != -1)
        {
            QMap <quint16, client>::iterator it = clients.begin();
            for (; it != clients.end(); it++)
            {
                if (ui->listClient->currentItem()->text() == it.value().name)
                {
                    if (it.value().state)
                        it.value().socket->close();
                    ui->listClient->takeItem(ui->listClient->currentRow());
                    listC.remove(it.value().name);
                    clients.remove(it.key());
                    return;
                }
            }
            on_cmbSortClients_currentIndexChanged(ui->cmbSortClients->currentText());
            sendListClient();
        }
        else
        {
            errDialog err("Выберите пользователя, которого необходимо удалить.");
            err.setWindowTitle("Ошибка");
            err.exec();
        }
    }
    else
    {
        errDialog err("Список пользователей пуст!");
        err.setWindowTitle("Ошибка");
        err.exec();
    }
}

void MyServer::on_actOpen_triggered()
{
    QString fileDir = QFileDialog::getOpenFileName(this,tr("Выберите файл для сохранения"));
    if (fileDir.trimmed() != "")
    {
        idialog dial("Открытие списка приведет к потере уже добавленных клиентов!\n Вы действительно хотите открыть файл?");
        dial.setWindowTitle("Открыть список клиентов");
        if (dial.exec() == QDialog::Accepted)
        {
            clients.clear();
            ui->listClient->clear();
            listC.clear();
            identity = 0;
            QFile xmlFile(fileDir);
            if (!xmlFile.exists() || !xmlFile.open(QFile::ReadOnly | QFile::Text)) {
                ui->txtLogClient->append("Невозможно открыть файл с информацией о клиентах!");
                return;
            }
            QDomDocument domDocument;
            domDocument.setContent(&xmlFile);
            QDomElement topElement = domDocument.documentElement();
            QDomNode domNode = topElement.firstChild();
            QString nameCurClient;
            while (!domNode.isNull())
            {
                identity++;
                QDomElement domElement = domNode.toElement();
                if (!domElement.isNull())
                {
                    if (domElement.tagName() == "client")
                    {
                        QDomNode node = domElement.firstChild();
                        while (!node.isNull())
                        {
                            QDomElement element = node.toElement();
                            if (!element.isNull())
                            {
                                const QString tagName(element.tagName());
                                if (tagName == "name")
                                {
                                    ui->listClient->addItem(new QListWidgetItem(QIcon(":/rec/img/decline.png"), element.text()));
                                    clients[identity].name = element.text();
                                    clients[identity].state = false;        //статус - "Не подключен"
                                    nameCurClient = element.text();
                                } else if (tagName == "ip")
                                {
                                    listC[nameCurClient] = element.text();
                                    clients[identity].ip = (QHostAddress)element.text();
                                }
                            }
                            node = node.nextSibling();
                        }
                    }
                }
                domNode = domNode.nextSibling();
            }
            sendListClient();
            on_cmbSortClients_currentIndexChanged(ui->cmbSortClients->currentText());
        }
    }
}

void MyServer::on_btnHideInfo_clicked()
{
    if (!checkBtn)
    {
        if (ui->listClient->currentRow() != -1)
        {
            ui->txtClientInfo->setVisible(true);
            ui->btnHideInfo->setText("Скрыть информацию");
            QMap <quint16, client>::iterator it = clients.begin();
            for (; it != clients.end(); it++)
            {
                if (ui->listClient->currentItem()->text() == it.value().name && ui->txtClientInfo->isVisible())
                {
                    ui->txtClientInfo->clear();
                    ui->txtClientInfo->append("Имя клиента: " + it.value().name);
                    ui->txtClientInfo->append("IP-адрес: " + it.value().ip.toString());
                    ui->txtClientInfo->append("Состояние: " + QString(it.value().state ? "Подключен" : "Отключен"));
                    break;
                }
            }
        }
        else
        {
            errDialog err("Выберите пользователя из списка!");
            err.setWindowTitle("Ошибка");
            err.exec();
        }
    }
    else
    {
        ui->txtClientInfo->setVisible(false);
        ui->btnHideInfo->setText("Показать информацию");
    }
    checkBtn = !checkBtn;
}

void MyServer::on_cmbSortClients_currentIndexChanged(const QString &arg1)
{
    if (arg1 == "↑ Имя")
    {
        ui->listClient->sortItems(Qt::AscendingOrder);
    }
    else if (arg1 == "↓ Имя")
    {
        ui->listClient->sortItems(Qt::DescendingOrder);
    }
}

void MyServer::on_chkUnblock_toggled(bool checked)
{
    ui->vgroupInfo->setEnabled(checked);
}

void MyServer::on_btnClearLog_clicked()
{
    idialog dial("Вы действительно хотите очистить поле логов?");
    dial.setWindowTitle("Очистка поля");
    if (dial.exec() == QDialog::Accepted)
    {
        ui->txtLogClient->clear();
    }
}

void MyServer::on_btnExecProc_clicked()
{
    if (ui->listClient->currentRow() != -1)
    {
        QMap <quint16, client>::iterator it = clients.begin();
        for (; it != clients.end(); it++)
        {
            if (ui->listClient->currentItem()->text() == it.value().name && it.value().state)
            {
                bool flag;
                QInputDialog inpPath;
                QString text = inpPath.getText(0, tr("Ввод информации"), ui->listClient->currentItem()->text(), QLineEdit::Normal, "Введите путь", &flag);
                if (flag && !text.isEmpty())
                {
                    quint16 typeMsg = 2;
                    QByteArray arrBlock;
                    QDataStream out (&arrBlock, QIODevice::WriteOnly);
                    out.setVersion(QDataStream::Qt_5_3);
                    out << quint16(0) << typeMsg << QTime::currentTime() << text;
                    out.device() -> seek(0);
                    out << (quint16)(arrBlock.size() - sizeof(quint16));
                    it.value().socket->write(arrBlock);
                }
                else
                {
                    errDialog err("Выбранный пользователь не в сети!");
                    err.setWindowTitle("Ошибка");
                    err.exec();
                }
                break;
            }
        }
    }
    else
    {
        errDialog err("Выберите пользователя из списка!");
        err.setWindowTitle("Ошибка");
        err.exec();
    }
}

void MyServer::on_btnChooseFile_clicked()
{
    QString saveClients = QFileDialog::getExistingDirectory(this,tr("Выберите директорию для сохранения"));
    if (saveClients.trimmed() != "")
    {
        filePath = saveClients;
        ui->lineFilePath->setText(filePath);
        on_lineFilePath_editingFinished();
    }
}

void MyServer::on_actChangeTheme_triggered()
{
    if (checkTheme)
    {
        for (int i = 0; i < 2; i++)
        {
            QPalette darkPalette;
            qApp->setStyle(QStyleFactory::create("Fusion"));
            darkPalette.setColor(QPalette::Window, QColor(53, 53, 53));
            darkPalette.setColor(QPalette::WindowText, Qt::white);
            darkPalette.setColor(QPalette::Base, QColor(25, 25, 25));
            darkPalette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
            darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
            darkPalette.setColor(QPalette::ToolTipText, Qt::white);
            darkPalette.setColor(QPalette::Text, Qt::white);
            darkPalette.setColor(QPalette::Button, QColor(53, 53, 53));
            darkPalette.setColor(QPalette::ButtonText, Qt::white);
            darkPalette.setColor(QPalette::BrightText, Qt::red);
            darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
            darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
            darkPalette.setColor(QPalette::HighlightedText, Qt::black);
            darkPalette.setColor(QPalette::Disabled, QPalette::Text, Qt::darkGray);
            darkPalette.setColor(QPalette::Disabled, QPalette::ButtonText, Qt::darkGray);
            qApp->setStyleSheet("QToolTip { color: #FFFFFF; background-color: #2A82DA; border: 1px solid white; }");
            qApp->setPalette(darkPalette);
        }
        ui->lineIP->setNightMode();
        ui->actChangeTheme->setIcon(QIcon(":/rec/img/lightmode.png"));
        ui->actChangeTheme->setText("Светлая тема");
        ui->actChangeTheme->setToolTip("Светлая тема");
        ui->actExit->setIcon(QIcon(":/rec/img/exitDarkMode.png"));
        ui->actMinimize->setIcon(QIcon(":/rec/img/minimizeDarkMode.png"));
        ui->chkUnblock->setStyleSheet("color: white;");
    }
    else
    {
        ui->lineIP->setLightMode();
        qApp->setPalette(style()->standardPalette());
        qApp->setStyleSheet("QToolTip { color: #FFFFFF; background-color: #2A82DA; border: 1px solid black; }"
                            "QGroupBox::!active {color: black;}"
                            "QLabel::!active {color: black;}");
        ui->actChangeTheme->setText("Темная тема");
        ui->actChangeTheme->setToolTip("Темная тема");
        ui->actChangeTheme->setIcon(QIcon(":/rec/img/nightmode.png"));
        ui->actExit->setIcon(QIcon(":/rec/img/exit.png"));
        ui->actMinimize->setIcon(QIcon(":/rec/img/minimize.png"));
        ui->chkUnblock->setStyleSheet("color: black;");
    }
    checkTheme = !checkTheme;
}

void MyServer::on_actMinimize_triggered()
{
    showMinimized();
}

void MyServer::on_btnClearChat_clicked()
{
    idialog dial("Вы действительно хотите очистить чат?");
    dial.setWindowTitle("Очистка чата");
    if (dial.exec() == QDialog::Accepted)
    {
        ui->txtChat->clear();
    }
}

void MyServer::on_btnSaveChat_clicked()
{
    idialog dial("Вы действительно хотите сохранить чат?");
    dial.setWindowTitle("Сохранение");
    if (dial.exec() == QDialog::Accepted)
    {
        if (!chatPath.exists())
            chatPath.mkpath(".");
        QString path = chatPath.path() + "/" + QDateTime::currentDateTime().toString("dd-MM-yyyy-hh-mm-ss") + ".txt";
        QFile chatFile(path);
        if (!chatFile.open(QIODevice::WriteOnly))
            QMessageBox::critical(0, "Ошибка", "Не удалось открыть файл");
        QByteArray log = ui->txtChat->toPlainText().toUtf8();
        chatFile.write(log);
        chatFile.close();
        ui->txtChat->append(QTime::currentTime().toString() + " СИСТЕМА: Журнал сохранен в " + path);
    }
}

void MyServer::openUserSett()
{
    QFile xmlFileSet(fileUserSet);
    if (!xmlFileSet.exists() || !xmlFileSet.open(QFile::ReadOnly | QFile::Text))
    {
        ui->txtLogClient->append("Невозможно настроить интерфейс!");
        return;
    }
    QDomDocument domDocument;
    domDocument.setContent(&xmlFileSet);
    QDomElement topElement = domDocument.documentElement();
    QDomNode domNode = topElement.firstChild();
    QDomElement domElement = domNode.toElement();
    if (!domElement.isNull())
    {
        if (domElement.tagName() == "settings")
        {
            QDomNode node = domElement.firstChild();
            while (!node.isNull())
            {
                QDomElement element = node.toElement();
                if (!element.isNull())
                {
                    const QString tagName(element.tagName());
                    if (tagName == "theme")
                    {
                        checkTheme = element.text() == "d" ? true : false;
                        on_actChangeTheme_triggered();
                    } else if (tagName == "pathToFile")
                    {
                        filePath = element.text();
                        ui->lineFilePath->setText(filePath);
                    }
                }
                node = node.nextSibling();
            }
        }
    }
}

void MyServer::saveUserSett()
{
    if (!chatPath.exists())
        chatPath.mkpath(".");
    QFile file(fileUserSet);
    if (file.open(QIODevice::WriteOnly))
    {
        QXmlStreamWriter xmlWriter(&file);
        xmlWriter.setAutoFormatting(true); // Устанавливаем автоформатирование текста
        xmlWriter.writeStartDocument();     // Запускаем запись в документ
        xmlWriter.writeStartElement("server");
        xmlWriter.writeStartElement("settings");

        xmlWriter.writeStartElement("theme");
        xmlWriter.writeCharacters(checkTheme ? "l" : "d");
        xmlWriter.writeEndElement();

        xmlWriter.writeStartElement("pathToFile");
        xmlWriter.writeCharacters(filePath);
        xmlWriter.writeEndElement();

        xmlWriter.writeEndElement();
        xmlWriter.writeEndElement();
        xmlWriter.writeEndDocument();
        file.close();
    }
}

void MyServer::on_lineFilePath_editingFinished()
{
    QString strFilePath = ui->lineFilePath->text();
    if (strFilePath[strFilePath.length()-1] != "/")
    {
        strFilePath += "/";
        ui->lineFilePath->setText(strFilePath);
    }
    QDir dir(strFilePath);

    if (!dir.exists())
    {
        dir.mkpath(".");
    }
    filePath = strFilePath;
}

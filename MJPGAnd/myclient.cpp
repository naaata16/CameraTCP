#include <QtNetwork>
#include <QtGui>
#include <QWidget>
#include <QMessageBox>
#include <QTextEdit>
#include <QtWidgets>
#include <stdio.h>
#include "client.h"

MyClient::MyClient(const QString& strHost,int nPort, QWidget* pwgt):QWidget(pwgt),nextBlockSize(0)
{

    tcpSocket = new QTcpSocket(this);
    //tcpSocket->connectToHost(strHost,nPort);//Устанавливает связь с сервером
    //==============================================================================================
    connect(tcpSocket,SIGNAL(connected()),SLOT(slotConnected()));//Соединяем сигнал и слот при создании соединения
    connect(tcpSocket,SIGNAL(readyRead()),SLOT(slotReadyRead())); //Соединяет слот и синал при готовности к передачи данных
    //connect(tcpSocket,SIGNAL(readChannelFinished()),SLOT(slotReadChannelFinished()));
    connect(tcpSocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(slotError(QAbstractSocket::SocketError)));//Слот и сигнал при выдачи ощибки
    //==============================================================================================
    txtInfo = new QTextEdit; //Для отображения текста
    txtIp = new QLineEdit;//Для вставки и редактирования текста
    txtPort = new QLineEdit;//Для вставки и редактирования текста
    txtInfo->setReadOnly(true);
    QPushButton* pcmd = new QPushButton("&GO!");
    //============================================================================
    connect(pcmd,SIGNAL(clicked()),SLOT(slotSendToServer()));
    //connect(txtInput,SIGNAL(returnPressed()),SLOT(slotSendToServer()));
    //============================================================================
    QHBoxLayout* res = new QHBoxLayout;
    lb = new QLabel;
    lb->setFixedSize(1080,720);
    //QImage img("C:\\Users\\Danil-PC\\Pictures\\069.jpg");
    //lb->setPixmap(QPixmap::fromImage(img));
    QVBoxLayout* pvbxLayout = new QVBoxLayout;
    pvbxLayout->addWidget(new QLabel("<H1>Client</H1>"));
    pvbxLayout->addWidget(txtIp);
    pvbxLayout->addWidget(txtPort);
    pvbxLayout->addWidget(txtInfo);
    pvbxLayout->addWidget(pcmd);
    res->addWidget(lb);
    res->addItem(pvbxLayout);
    setLayout(res);
    txtIp->setText(strHost);
    QString strPort = "";
    strPort.setNum(nPort);
    txtPort->setText(strPort);
}
/*
void MyClient::slotReadyRead()
{
    QByteArray arr = tcpSocket->readAll();
    txt.append(arr);
    getPicture();
}
void MyClient::getPicture()
{
    char search1[] = "Content-Length: ";
    int pol1 = txt.indexOf(search1);
    if(pol1 != -1)
    {
        pol1 += 25;
        char search2[] = "--myboundary";
        int pol2 = txt.indexOf(search2,pol1);
        if(pol2 != -1)
        {
            QByteArray picture = txt.mid(pol1,pol2-pol1-2);
            int s = picture.size();
            QByteArray r = picture.mid(s-2,2);
            QImage img;
            img.loadFromData(picture,"jpg");
            lb->setPixmap(QPixmap::fromImage(img));
            txt.remove(0,pol2);
        }
    }
}
*/
void MyClient::slotReadyRead()
{
    QByteArray arr = "";
    if(Size == 0)
    {
        char search1[] = "Content-Length: ";
        int pol1 = 0;
        QByteArray size;
        do
        {
            arr = tcpSocket->readLine();
            pol1 = arr.indexOf(search1);
            if(pol1 != -1)
            {
                break;
            }
            arr = "";
        }
        while(true);
        pol1 =+ 16;
        while(arr.at(pol1)!='\r')
        {
            size.append(arr.at(pol1));
            pol1++;
        }
        Size = size.toInt();
        txt = "";
        arr = tcpSocket->readLine();
    }
    arr = "";
    arr = tcpSocket->read(Size);
    txt.append(arr);
    int i = arr.size();
    Size = Size - i;
    if(Size == 0)
    {
        QImage img;
        img.loadFromData(txt,"jpg");
        lb->setPixmap(QPixmap::fromImage(img));
        lb->setScaledContents(true);
    }
}

void MyClient::slotError(QAbstractSocket::SocketError err)
{
    QString strError = "Error: " + (err == QAbstractSocket::HostNotFoundError ? "The host was not found." : err == QAbstractSocket::RemoteHostClosedError ? "The remote host is closed." : err == QAbstractSocket::ConnectionRefusedError ? "The connection was refused. " : QString(tcpSocket->errorString()));
    txtInfo->append(strError);
}
void MyClient::slotSendToServer()
{
    QString ip = txtIp->text();
    int port = (txtPort->text()).toInt();
    tcpSocket->connectToHost(ip,port);
    //const char request[] =  "GET /axis-cgi/jpg/image.cgi?resolution=320x240 HTTP/1.1\r\nHost: 10.168.0.185\r\n\r\n";
    QByteArray request;
    request.append("GET /axis-cgi/mjpg/video.cgi?resolution=320x240 HTTP/1.1\r\nHost: " + ip + "\r\n\r\n");
    //const char request[] = "GET /axis-cgi/mjpg/video.cgi?resolution=320x240 HTTP/1.1\r\nHost: 10.168.0.185\r\n\r\n";
    //tcpSocket->write(request, strlen(request));
    tcpSocket->write(request);
    txtInfo->append("Send!");
    //->setText("Send!");
}
void MyClient::slotConnected()
{
    txtInfo->append("Received the connected() signals");
}

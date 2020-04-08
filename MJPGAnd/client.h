#pragma once
#include <QWidget>
#include <QTcpSocket>
#include <QLabel>
class QTextEdit;
class QLineEdit;

class MyClient : public QWidget
{
    Q_OBJECT
private:
    QTcpSocket* tcpSocket;// Сокет для управления клиентом
    QTextEdit* txtInfo;
    QLabel *lb;
    QLineEdit* txtIp;
    QLineEdit* txtPort;
    quint16 nextBlockSize;//Размер блока
    QByteArray txt;
    int Size = 0;
    void getPicture();
public:
    MyClient(const QString & strHost,int nPort,QWidget* pwgt = 0);

private slots:
    void slotReadyRead();//Слот для чтения данных
    void slotError(QAbstractSocket::SocketError);//слот соединен с сигналом отправляющем ощибку
    void slotSendToServer();
    void slotConnected(); // слот для подключения
    //void slotReadChannelFinished();
};

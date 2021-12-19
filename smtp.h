#ifndef STMP_H
#define STMP_H
#include<QByteArray>
#include<QString>
#include<QTcpSocket>

class smtp
{
public:
    smtp(QByteArray username,QByteArray password);
    ~smtp();
    public:
        void send(QByteArray recvaddr,QString subject,QString content);
    public:
        QTcpSocket * clientsocket;
        QByteArray username;
        QByteArray password;
        QByteArray recvaddr;
        QByteArray mailfrom = "mail from:<";
        QByteArray rcptto = "rcpt to:<";
        QByteArray prefrom = "from:";
        QByteArray preto = "to:";
        QByteArray presubject ="subject:";
        QString subject;                //主题
        QString content;                //发送内容
        QByteArray recvdata;            //接收到的数据
};

#endif // STMP_H

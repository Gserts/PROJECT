#include "mythread.h"

//MyThread是为了实现服务器多线程工作而编写的

MyThread::MyThread(QTcpSocket *s)
{
    socket=s;
}

void MyThread::run()
{
    connect(socket,&QTcpSocket::readyRead,this,&MyThread::ClientInfoSlot);
}

void MyThread::ClientInfoSlot(){

    //目前到这一步没问题
    QByteArray d=socket->readAll();  //这里出错，这是一个流，请不要重复读取，否则在后面值会为空

    emit sendMsg(d);
    //注意，不可以在其他类里面操作界面，因此，需要用到自定义槽函数。这里暂时只能debug
}

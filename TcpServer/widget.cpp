#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    server = new QTcpServer;

    server->listen(QHostAddress::AnyIPv4,PORT);//监听ip和port

    connect(server,&QTcpServer::newConnection,this,&Widget::newClientHandler); //监听的槽函数
}

Widget::~Widget()
{
    delete ui;

}


void Widget::newClientHandler()
{
    //建立TCP连接
    QTcpSocket *socket =server->nextPendingConnection();//监听到一个对象，如果需要监听多个对象的话，应该把这个socket对象存进一个向量或者数组,同时进行监听.
    ui->ShowIP->setText(socket->peerAddress().toString());//获取客户端ip
    ui->ShowPort->setText(QString::number(socket->peerPort())); //获取客户端端口

    //connect(socket,&QTcpSocket::readyRead,this,&Widget::RecvInfo);
    //启动线程
    MyThread * t= new MyThread(socket);
    t->QThread::start();  //开启线程
    ui->textBrowser->setText("线程启动");

    connect(t,&MyThread::sendMsg,this,&Widget::threadRead);
}


//void Widget::RecvInfo(){
//    QTcpSocket *s= (QTcpSocket *)sender();
//    ui->textBrowser->setText(QString(s->readAll()));
//}

void Widget::threadRead(QByteArray ReciData)
{
    ui->textBrowser->append(QString::fromUtf8(ReciData)); //设置text
}







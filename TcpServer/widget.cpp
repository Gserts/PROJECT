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


void Widget::clientDisconnected(){ //用户断开方法
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    if (socket) {
        // 从列表中移除套接字
        QList<QListWidgetItem*> items= ui->infoList->findItems(socket->peerAddress().toString(),Qt::MatchExactly);
        if(!items.isEmpty()){
            delete ui->infoList->takeItem(ui->infoList->row(items.first())); //在列表中删除
        }

        ClientsocketList.removeAll(socket);
        socket->deleteLater();  // 确保安全地删除套接字

        // 从界面上移除 IP 地址
        // 这里需要您根据自己的界面设计来编写代码，例如更新一个列表控件
        // ui->InfoList->removeItem(...)  // 伪代码，根据实际情况修改********************


        // 打印或记录日志
        qDebug() << "Client disconnected: " << socket->peerAddress().toString();
    }
}



void Widget::newClientHandler()
{
    //建立TCP连接
    QTcpSocket* socket = server->nextPendingConnection();
    //监听到一个对象，如果需要监听多个对象的话，应该把这个socket对象存进一个向量或者数组,同时进行监听.
    ClientsocketList.push_back(socket); //用向量记录每一个socket
    //读取socket中存储的用户名和密码
    QByteArray data = socket->readAll();
    QString username,password;
    QDataStream in (&data,QIODevice::ReadOnly);
    in >> username >>password;

    //连接到数据库，注意这里以后需要修改！！！！！！！！！！！！！！！！！！！！！！！！
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost"); // 或您的数据库服务器地址
    db.setDatabaseName("mydatabase");
    db.setUserName("root"); // 数据库用户名
    db.setPassword("85621362"); // 数据库密码

    if (!db.open()) {
        qDebug() << "Database connection failed:" << db.lastError().text();
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT password FROM userinfo WHERE username = :username");
    query.bindValue(":username", username);
    query.exec();

    if (query.next()) {
        // 用户名存在
        QString storedPassword = query.value(0).toString();
        if (password == storedPassword) {
            // 密码正确
            QByteArray confirm="Success";
            SendSocket->write(confirm);
            SendSocket->waitForBytesWritten();
            ui->infoList->addItem(socket->peerAddress().toString());  //将ip地址添加到ip列表
            ui->ShowIP->setText(socket->peerAddress().toString());//获取客户端ip
            ui->ShowPort->setText(QString::number(socket->peerPort())); //获取客户端端口
            //connect(socket,&QTcpSocket::readyRead,this,&Widget::RecvInfo);
            //启动线程
            MyThread * t= new MyThread(socket);
            t->QThread::start();  //开启线程
            ui->textBrowser->setText("线程启动");
            connect(t,&MyThread::sendMsg,this,&Widget::threadRead);
        } else {
            // 密码错误
            // 发送错误信息到客户端
            QByteArray confirm="Failed";
            SendSocket->write(confirm);
            SendSocket->waitForBytesWritten();
        }
    } else {
        // 用户名不存在，插入新用户
        query.prepare("INSERT INTO userinfo (username, password) VALUES (:username, :password)");
        query.bindValue(":username", username);
        query.bindValue(":password", password); // 注意：实际应用中应存储密码的哈希，而非明文
        if (!query.exec()) {
            qDebug() << "Insert failed:" << query.lastError().text();
            // 可能需要发送错误信息到客户端
        } else {
            // 插入成功，执行后续操作
            QByteArray confirm="Success";
            SendSocket->write(confirm);
            SendSocket->waitForBytesWritten();

            ui->infoList->addItem(socket->peerAddress().toString());  //将ip地址添加到ip列表
            ui->ShowIP->setText(socket->peerAddress().toString());//获取客户端ip
            ui->ShowPort->setText(QString::number(socket->peerPort())); //获取客户端端口
            //connect(socket,&QTcpSocket::readyRead,this,&Widget::RecvInfo);
            //启动线程

            MyThread * t= new MyThread(socket);
            t->QThread::start();  //开启线程
            ui->textBrowser->setText("线程启动");
            connect(t,&MyThread::sendMsg,this,&Widget::threadRead);

        }
    }

    db.close();


    //-------------------------------------


    connect(socket,&QTcpSocket::disconnected,this,&Widget::clientDisconnected);

}


//void Widget::RecvInfo(){
//    QTcpSocket *s= (QTcpSocket *)sender();
//    ui->textBrowser->setText(QString(s->readAll()));
//}

void Widget::threadRead(QByteArray ReciData)
{
    ui->textBrowser->append(QString::fromUtf8(ReciData)); //设置text
}







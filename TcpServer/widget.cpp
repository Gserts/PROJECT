#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)//构造函数，
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    server = new QTcpServer;

    server->listen(QHostAddress::AnyIPv4,PORT);//监听ip和port

    connect(server,&QTcpServer::newConnection,this,&Widget::newClientHandler); //监听的槽函数

    //连接到数据库，注意这里以后需要修改！！！！！！！！！！！！！！！！！！！！！！！！
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost"); // 或您的数据库服务器地址
    db.setDatabaseName("userinfo");
    db.setUserName("root"); // 数据库用户名
    db.setPassword("85621362"); // 数据库密码
    if (!db.open()) {
        qDebug() << "Database connection failed:" << db.lastError().text();
    } else {
        // 数据库连接成功，显示消息框
        qDebug()<< "Database connected Successfully";
    }
}

Widget::~Widget()
{
    delete ui;
}


void Widget::clientDisconnected(){ //用户断开方法
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    if (socket) {
        // 从列表中移除套接字
        clientLoggedIn[socket]=false; //登陆状态更改
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
    if(!socket) return;

    ClientsocketList.push_back(socket); //用向量记录每一个socket
    clientLoggedIn[socket]=false;
    connect(socket,&QTcpSocket::readyRead,this,&Widget::readyRead);
    //读取socket中存储的用户名和密码，同时清空socket

    //数据库的查询，userinfo是要查询的表
    //-------------------------------------


}

void Widget::readyRead(){
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    if (socket == nullptr) return;

    // 确保有足够的数据可以读取
    if (socket->bytesAvailable() < (int)sizeof(quint16)) {
        return; // 如果数据不够，等待更多数据
    }

    QByteArray data = socket->readAll();
    QDataStream in(&data, QIODevice::ReadOnly);

    // 进行数据处理
    if(!clientLoggedIn[socket]){
        QString type, username, password;
        in >> type >> username >> password;
        if(type == "LOGIN"){
            qDebug()<<"用户登陆";
            QSqlQuery query;
            query.prepare("SELECT password FROM userinfo WHERE username = :username");
            query.bindValue(":username", username);
            query.exec();
            QByteArray response;
            if(query.next()){
                QString storedpassword = query.value(0).toString();
                if(password == storedpassword) {
                    response= "Success";
                }else{
                    response  = "Falied";
                    return;
                }
            }
            socket->write(response);
            clientLoggedIn[socket]=true;
            qDebug()<<"切换登陆状态";  //截止到这里不可以
            //确定为登陆状态

        }else if(type == "REGISTER"){
            qDebug()<<"用户注册";
            QSqlQuery checkQuery;
            checkQuery.prepare("SELECT username FROM userinfo WHERE username = :username");
            checkQuery.bindValue(":username", username);
            checkQuery.exec();
            QByteArray response;
            if(checkQuery.next()){
                //用户名已存在
                response="Failed";
                socket->write(response);
                return;
            }else{
                QSqlQuery insertQuery;
                insertQuery.prepare("INSERT INTO userinfo (username, password) VALUES (:username, :password)");
                insertQuery.bindValue(":username", username);
                insertQuery.bindValue(":password", password); // 应考虑存储密码哈希(暂时不弄)
                if (insertQuery.exec()) {
                    response = "Success";
                } else {
                    response = "Failed";
                    socket->write(response);
                    return;
                }
            }
            socket->write(response);

        }
    }else {
        MyThread * t= new MyThread(socket);
        t->QThread::start();  //开启线程
        ui->textBrowser->setText("线程启动");
        connect(t,&MyThread::sendMsg,this,&Widget::threadRead);
        connect(socket,&QTcpSocket::disconnected,this,&Widget::clientDisconnected);
    }
}

//void Widget::RecvInfo(){
//    QTcpSocket *s= (QTcpSocket *)sender();
//    ui->textBrowser->setText(QString(s->readAll()));
//}

void Widget::threadRead(QByteArray ReciData)
{
    ui->textBrowser->append(QString::fromUtf8(ReciData)); //设置text
    qDebug()<<"信息发送";
}







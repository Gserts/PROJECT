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
    db.setHostName(DB_Server); // 或您的数据库服务器地址
    db.setDatabaseName(UserInfoTable);
    db.setUserName(DB_USR); // 数据库用户名
    db.setPassword(DB_PWD); // 数据库密码
    db.setPort(28890);
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
        QList<QListWidgetItem*> items= ui->infoList->findItems(socket->peerAddress().toString(),Qt::MatchContains);
        if(!items.isEmpty()){
            delete ui->infoList->takeItem(ui->infoList->row(items.first())); //在列表中删除
        }
        ClientsocketList.removeAll(socket);
        socket->deleteLater();  // 确保安全地删除套接字
        // 打印或记录日志
        qDebug() << "Client disconnected: " << socket->peerAddress().toString();

        MyThread* thread = socketThreadMap.take(socket); // 从映射中移除线程
        if (thread) {
            thread->quit();
            thread->wait();
            thread->deleteLater();
        }
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
    connect(socket,&QTcpSocket::disconnected,this,&Widget::clientDisconnected);
    //如果连接成功，那么服务器列表显示连接IP

}
    //数据库的查询，userinfo是要查询的表
    //-------------------------------------




void Widget::readyRead(){
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    if (socket == nullptr) return;

    // 确保有足够的数据可以读取
    if (socket->bytesAvailable() < (int)sizeof(quint16)) {
        return; // 如果数据不够，等待更多数据
    }

    // 进行数据处理
    if(!clientLoggedIn[socket]){
        QByteArray data = socket->readAll();
        QDataStream in(&data, QIODevice::ReadOnly);
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
            QString clientIP= socket->peerAddress().toString();
            ui->infoList->addItem("IP&端口:"+clientIP+":"+QString::number(socket->peerPort())+" 用户:"+username);
            //列表更新
            qDebug()<<"登陆状态:"<<clientLoggedIn[socket];
            //确定为登陆状态
            //映射名字与socket
            usrMAPsocket(username,socket);

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
        //这里寻找socket对应的线程，如果有，则令thread为此线程，若无，则进入新建线程。
        MyThread* thread = socketThreadMap.value(socket, nullptr);
        if (!thread) {
            thread = new MyThread(socket);

            connect(thread, &MyThread::finished, thread, &QObject::deleteLater);
            connect(thread, &MyThread::sendTarget, this, &Widget::threadRead,Qt::QueuedConnection);
            connect(thread, &MyThread::sendTarget,this, &Widget::conveyToTarget);

            socketThreadMap.insert(socket, thread);
            thread->start();// 将线程存储在映射中，###############这个代码请勿再更改与槽函数的顺序#################
        }
         //发送规格：群or个人、id、时间、内容
        // 此处可以添加代码来处理已登录的socket，例如将数据传递给线程
    }
}

//void Widget::RecvInfo(){
//    QTcpSocket *s= (QTcpSocket *)sender();
//    ui->textBrowser->setText(QString(s->readAll()));
//}

void Widget::conveyToTarget(const QString & type,
                             const QString &from_id,
                             const QString &to_id,
                             const QString & time,
                            const QString& content){
    QTcpSocket* toSocket = usrMapSocket.value(to_id,nullptr);
    if(toSocket){
        qDebug()<<"检测socket通过，发送至对象";
        //toSocket->readAll();//清除socket内容
        QByteArray dataToSend;
        QDataStream out(&dataToSend, QIODevice::WriteOnly);
        //这里去除了对象（也就是接受者的id），仅仅保留发送者。
        out<<type<<from_id<<time<<content;
        toSocket->write(dataToSend);
    }else{
        qDebug()<<"检测对象所对应的socket不存在，发送失败";
    }
    //把从服务器读出的内容重新写入，发送给用户的socket中，内容保持一样。
}





void Widget::threadRead(QString type,QString from_id,QString to_id,QString time,QString content)
{
    ui->textBrowser->append(time);
    ui->textBrowser->append(from_id+" To "+to_id);
    ui->textBrowser->append(content);    //读取socket中的内容

    qDebug()<<"信息发送";
}







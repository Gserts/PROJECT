#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <basewidget.h>

//mainwindow是登陆界面

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->PasswordLineEdit,SIGNAL(returnPressed()),this,SLOT(on_Login_clicked()));

}

MainWindow::~MainWindow()
{
    delete ui;
    socket->disconnectFromHost(); //析构的时候，把socket退出连接
}

//从服务器接受回应
void MainWindow::onServerResponse() {
    QByteArray response = socket->readAll();
    if (response == "Success") {
        this->hide(); // 隐藏登录界面
        BaseWidget *w = new BaseWidget(socket); // 创建新界面并传递socket
        w->show(); // 显示新界面
    } else if (response == "Failed") {
        QMessageBox::warning(this, "登录失败", "用户名或密码错误");
    }else{
        QMessageBox::warning(this, "错误", "未知错误");
    }
}


void MainWindow::on_ExitButton_clicked()//退出窗口
{
    this->close();
}

void MainWindow::on_Login_clicked()
{
    QString l="LOGIN";
    Connect(l);//处理连接
    // 连接 readyRead 信号以接收服务器响应，回应Success则进入，Failed则返回错误
    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::onServerResponse);

    // 处理连接异常
    connect(socket, &QTcpSocket::disconnected, [this]() {
        ui->Condition->setText("未连接");
        QMessageBox::warning(this, "连接失败", "无法连接到服务器");
        this->show();
    });
}

void MainWindow::Connect(const QString &type){ //登陆或者注册使用的初始化连接
    const QString name = ui->NameLineEdit->text();
    const QString password = ui->PasswordLineEdit->text();
    qDebug()<<type;
    socket = new QTcpSocket(this);  // 初始化socket
    // 连接到服务器
    socket->connectToHost(QHostAddress(IP), port);
    // 连接成功时发送数据
    connect(socket, &QTcpSocket::connected, [this, type, name, password]() {
        QByteArray dataToSend;
        QDataStream out(&dataToSend, QIODevice::WriteOnly);
        out << type<< name << password;
        socket->write(dataToSend);
        socket->waitForBytesWritten();
    });
}

void MainWindow::regResponse(){
    QByteArray response=socket->readAll();
    if(response=="Success"){
        QMessageBox::information(this,"信息","注册成功");
    }else if(response == "Failed"){
        QMessageBox::information(this,"问题","注册失败，请检查帐号是否已经存在");
    }
}

void MainWindow::on_reg_clicked()
{
    QString r="REGISTER";
    Connect(r);
    connect(socket, &QTcpSocket::readyRead,this,&MainWindow::regResponse);

}


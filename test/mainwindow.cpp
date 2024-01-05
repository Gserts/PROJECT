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
        this->show(); // 显示登录界面
    }
}


void MainWindow::on_ExitButton_clicked()
{
    this->close();
}

void MainWindow::on_Login_clicked()
{
    const QString name = ui->NameLineEdit->text();
    const QString password = ui->PasswordLineEdit->text();

    QString IP = "127.0.0.1";  // 服务器IP
    quint16 port = 12345;       // 服务器端口
    socket = new QTcpSocket(this);  // 初始化socket

    // 连接到服务器
    socket->connectToHost(QHostAddress(IP), port);

    // 连接成功时发送数据
    connect(socket, &QTcpSocket::connected, [this, name, password]() {
        QByteArray dataToSend;
        QDataStream out(&dataToSend, QIODevice::WriteOnly);
        out << name << password;
        socket->write(dataToSend);
        socket->waitForBytesWritten();
    });

    // 连接 readyRead 信号以接收服务器响应
    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::onServerResponse);

    // 处理连接异常
    connect(socket, &QTcpSocket::disconnected, [this]() {
        ui->Condition->setText("未连接");
        QMessageBox::warning(this, "连接失败", "无法连接到服务器");
        this->show();
    });
}






#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <basewidget.h>

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
}


void MainWindow::on_ExitButton_clicked()
{
    this->close();
}


void MainWindow::on_Login_clicked()
{
    const QString name = ui->NameLineEdit->text();
    const QString password = ui->PasswordLineEdit->text();

    if(name == "admin" && password == "admin")
    {
        socket = new QTcpSocket;
        QString IP="127.0.0.1";
        QString port="12345";
        //开始连接服务器
        socket->connectToHost(QHostAddress(IP),port.toShort());
        //检测是否连接成功
        connect(socket,&QTcpSocket::connected,[this]()
                {
                    ui->Condition->setText("已连接");
                    //QMessageBox::information(this,"提示","连接服务器成功");   不想卡顿

                    this->hide();//隐藏登陆界面
                    BaseWidget *w=new BaseWidget(socket); //给新界面传递socket;new的原因是不能够被析构

                    w->show(); //展示新界面

                }
                );
        //连接异常
        connect(socket,&QTcpSocket::disconnected,[this]()
                {
                    ui->Condition->setText("未连接");
                    this->show();
                }
                );

    }
    else
    {
        QMessageBox::information(this,"提示","请输入正确的账号密码");
    }



}


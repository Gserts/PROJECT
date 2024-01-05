#include "basewidget.h"
#include "ui_basewidget.h"

//basewidget是聊天界面

BaseWidget::BaseWidget(QTcpSocket *s,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BaseWidget)
{
    ui->setupUi(this);;
    socket=s;
    connect(ui->plainTextEdit,SIGNAL(returnPressed()),this,SLOT(on_SendBtn_clicked));

    connect(socket,&QTcpSocket::disconnected,[this]()
            {
                QMessageBox::warning(this,"警告","连接断开");
                this->close();
            }
            );

}


BaseWidget::~BaseWidget()
{
    delete ui;
}

void BaseWidget::on_ClearBtn_clicked()//清除输入
{
    ui->plainTextEdit->clear();
}


void BaseWidget::on_SendBtn_clicked() //发送信息按钮
{
    QByteArray ar,btime;
    QString time= QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    //获得时间

    ar.append(ui->plainTextEdit->toPlainText().toUtf8());
    btime.append(time.toUtf8());
    socket->write(btime);
    socket->write(ar);
    ui->textBrowser->append(time);
    ui->textBrowser->append(QString(ar));
    ui->plainTextEdit->clear();
}




void BaseWidget::on_ClearDataBtn_clicked()//清理聊天记录
{
    ui->textBrowser->clear();
}


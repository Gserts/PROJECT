#include "basewidget.h"
#include "ui_basewidget.h"

BaseWidget::BaseWidget(QTcpSocket *s,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BaseWidget)
{
    ui->setupUi(this);
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
    QByteArray ar;

    ar.append(ui->plainTextEdit->toPlainText().toUtf8());
    socket->write(ar);
    ui->textBrowser->append(QString(ar));
    ui->plainTextEdit->clear();
}




void BaseWidget::on_ClearDataBtn_clicked()//清理聊天记录
{
    ui->textBrowser->clear();
}


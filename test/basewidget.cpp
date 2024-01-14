#include "basewidget.h"
#include "ui_basewidget.h"

//basewidget是聊天界面

BaseWidget::BaseWidget(QTcpSocket *s,const QString &name,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BaseWidget)
{
    ui->setupUi(this);;
    socket=s;
    this->name=name;
    qDebug()<<"socket登陆后的内容："<<socket->readAll();
    connect(socket,&QTcpSocket::disconnected,[this]()
            {
                this->close();
                QMessageBox::warning(this,"警告","连接断开");
            }
            );
}


BaseWidget::~BaseWidget()
{
    socket->disconnectFromHost();
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
    socket->write(ar+"\n");

    qDebug()<<"想要发送的内容"<<btime<<ar;
    ui->textBrowser->append(time);
    ui->textBrowser->append(QString(ar));
    ui->plainTextEdit->clear();
}




void BaseWidget::on_ClearDataBtn_clicked()//清理聊天记录
{
    ui->textBrowser->clear();
}

//好友列表按钮的打开
void BaseWidget::on_friendBtn_clicked()
{
    flist *f=new flist(name);
    f->show();
}


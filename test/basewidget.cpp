#include "basewidget.h"
#include "ui_basewidget.h"

//basewidget是私聊 聊天界面

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

    connect(socket,&QTcpSocket::readyRead,this,&BaseWidget::onReadyRead);//传入socket的时候，读取信息并且显示到broswer上
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
    QString from_id=name;
    to_id=ui->ChatObject->text();//确定聊天对象id
    type="private";//类型：私聊
    QString time= QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    //获得时间
    QString content=ui->plainTextEdit->toPlainText();
    QByteArray dataToSend;
    QDataStream out(&dataToSend, QIODevice::WriteOnly);
    //发送规格：群or个人、id、时间、内容
    out << type<<from_id<<to_id<< time << content;
    socket->write(dataToSend);
    socket->waitForBytesWritten();
    //写入
    ui->textBrowser->append(time);
    ui->textBrowser->append(content);
    ui->plainTextEdit->clear();
    //本地呈现记录
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


void BaseWidget::onReadyRead(){
    QByteArray data = socket->readAll();
    QDataStream in(&data,QIODevice::ReadOnly);

    QString type,from_id,time,content;

    if(type=="private"){
        ui->textBrowser->append(time);
        ui->textBrowser->append("From"+from_id+":"+content);
    }
}






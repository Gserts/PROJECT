#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QHostAddress>
#include <mythread.h>
#include <QByteArray>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#define PORT 12345

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT
    QMap<QTcpSocket*, bool> clientLoggedIn; //登陆状态检测

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    void usrMAPsocket(const QString& usr,QTcpSocket * s){
        usrMapSocket[usr]=s;
    }

private slots:
    void newClientHandler();
    void threadRead(QString,QString,QString,QString,QString);
    void clientDisconnected();
    void readyRead();
    void conveyToTarget(const QString& type,
                        const QString& from_id,
                        const QString& to_id,
                        const QString& time,
                        const QString& content);
private:
    Ui::Widget *ui;
    QTcpServer *server;
    QTcpSocket *SendSocket;
    QList <QTcpSocket*> ClientsocketList; // 用来存储socket的list
    QString DB_Server="gz-cynosdbmysql-grp-233inxoz.sql.tencentcdb.com",
        UserInfoTable="Base1",
        DB_PWD="Zsc85621362",
        DB_USR="root";
    QMap<QTcpSocket*, MyThread*> socketThreadMap;
    QMap<QString, QTcpSocket*> usrMapSocket;    ;

 };
#endif // WIDGET_H

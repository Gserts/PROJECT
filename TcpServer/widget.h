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

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void newClientHandler();
    void threadRead(QByteArray);
    void clientDisconnected();

private:
    Ui::Widget *ui;
    QTcpServer *server;
    QTcpSocket *SendSocket;
    QList <QTcpSocket*> ClientsocketList; // 用来存储socket的list

 };
#endif // WIDGET_H

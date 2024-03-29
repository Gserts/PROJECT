#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QObject>
#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QThread>
#include<QDebug>
#include <QByteArray>


class MyThread : public QThread
{
    Q_OBJECT
public:
    explicit MyThread(QTcpSocket *s);
    void run(); //虚函数，需要重写


signals:
    void sendMsg(const QString & type,
                 const QString &from_id,
                 const QString &to_id,
                 const QString & time,
                 const QString& content);
    void sendTarget(const QString & type,
                      const QString &from_id,
                      const QString &to_id,
                      const QString & time,
                      const QString& content);

public slots:
    void ClientInfoSlot();

private:
    QTcpSocket *socket;
    QWidget *ui;
};


#endif // MYTHREAD_H

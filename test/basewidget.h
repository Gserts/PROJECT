#ifndef BASEWIDGET_H
#define BASEWIDGET_H

#include <QWidget>
#include<QTcpSocket>
#include <QMessageBox>
#include <mainwindow.h>
#include <QByteArray>
#include<QDateTime>
#include <flist.h>

namespace Ui {
class BaseWidget;
}

class BaseWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BaseWidget(QTcpSocket *s,const QString &name,QWidget *parent = nullptr);
    ~BaseWidget();

signals:
        void signalClose();

private slots:
    void on_ClearBtn_clicked();
    void on_SendBtn_clicked();
    void on_ClearDataBtn_clicked();
    void onReadyRead();
    void on_friendBtn_clicked();

protected:
    void closeEvent(QCloseEvent *event) override{
        emit signalClose();
        QWidget::closeEvent(event);
    }

private:
    Ui::BaseWidget *ui;
    Ui::MainWindow *lgui;
    QTcpSocket *socket;
    QString name;
    QString type,to_id;//窗口聊天类型，对象id
};

#endif // BASEWIDGET_H

#ifndef BASEWIDGET_H
#define BASEWIDGET_H

#include <QWidget>
#include<QTcpSocket>
#include <QMessageBox>
#include <mainwindow.h>
#include <QByteArray>

namespace Ui {
class BaseWidget;
}

class BaseWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BaseWidget(QTcpSocket *s,QWidget *parent = nullptr);
    ~BaseWidget();

private slots:
    void on_ClearBtn_clicked();

    void on_SendBtn_clicked();

    void on_ClearDataBtn_clicked();

private:
    Ui::BaseWidget *ui;
    Ui::MainWindow *lgui;
    QTcpSocket *socket;
};

#endif // BASEWIDGET_H

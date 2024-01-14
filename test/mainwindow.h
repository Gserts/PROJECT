#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>     //TCPSocket用于网络连接
#include <QHostAddress>   //用于地址

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
protected:
    void closeEvent(QCloseEvent *event) override;

signals:
    void signalConnected();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_ExitButton_clicked();
    void on_Login_clicked();
    void onServerResponse();//登陆回应
    void regResponse();//注册回应
    void Connect(const QString &str);
    void on_reg_clicked();

private:
    Ui::MainWindow *ui;
    QTcpSocket *socket;
    QString IP = "127.0.0.1";  // 服务器IP
    quint16 port = 12345;       // 服务器端口
};
#endif // MAINWINDOW_H

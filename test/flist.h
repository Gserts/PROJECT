#ifndef FLIST_H
#define FLIST_H

#include <QWidget>
#include <QTcpSocket>
#include <QMessageBox>
#include <QByteArray>

namespace Ui {
class flist;
}

class flist : public QWidget
{
    Q_OBJECT

public:
    explicit flist(const QString& usrname,QWidget *parent = nullptr);
    ~flist();

public slots:
    void slotCloseWindow(){
        this->close();
    }
private:
    Ui::flist *ui;
};

#endif // FLIST_H

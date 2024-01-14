#include "flist.h"
#include "ui_flist.h"

flist::flist(const QString &usrname,QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::flist)
{
    ui->setupUi(this);
    ui->usrname->setText(usrname);

}

flist::~flist()
{
    delete ui;
}

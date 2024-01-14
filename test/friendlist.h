#ifndef FRIENDLIST_H
#define FRIENDLIST_H

#include <QObject>
#include <QWidget>

class FriendList : public QWidget
{
    Q_OBJECT
public:
    explicit FriendList(QWidget *parent = nullptr);

signals:
};

#endif // FRIENDLIST_H

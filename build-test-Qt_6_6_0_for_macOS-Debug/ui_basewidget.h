/********************************************************************************
** Form generated from reading UI file 'basewidget.ui'
**
** Created by: Qt User Interface Compiler version 6.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BASEWIDGET_H
#define UI_BASEWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_BaseWidget
{
public:
    QTextBrowser *textBrowser;
    QPlainTextEdit *plainTextEdit;
    QGraphicsView *graphicsView;
    QLabel *hName;
    QWidget *widget;
    QVBoxLayout *verticalLayout_2;
    QPushButton *ClearDataBtn;
    QVBoxLayout *verticalLayout;
    QPushButton *ClearBtn;
    QPushButton *SendBtn;

    void setupUi(QWidget *BaseWidget)
    {
        if (BaseWidget->objectName().isEmpty())
            BaseWidget->setObjectName("BaseWidget");
        BaseWidget->resize(625, 486);
        textBrowser = new QTextBrowser(BaseWidget);
        textBrowser->setObjectName("textBrowser");
        textBrowser->setGeometry(QRect(150, 50, 461, 321));
        plainTextEdit = new QPlainTextEdit(BaseWidget);
        plainTextEdit->setObjectName("plainTextEdit");
        plainTextEdit->setGeometry(QRect(150, 390, 461, 74));
        graphicsView = new QGraphicsView(BaseWidget);
        graphicsView->setObjectName("graphicsView");
        graphicsView->setGeometry(QRect(40, 50, 91, 91));
        hName = new QLabel(BaseWidget);
        hName->setObjectName("hName");
        hName->setGeometry(QRect(60, 150, 58, 16));
        widget = new QWidget(BaseWidget);
        widget->setObjectName("widget");
        widget->setGeometry(QRect(30, 360, 112, 105));
        verticalLayout_2 = new QVBoxLayout(widget);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        ClearDataBtn = new QPushButton(widget);
        ClearDataBtn->setObjectName("ClearDataBtn");

        verticalLayout_2->addWidget(ClearDataBtn);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        ClearBtn = new QPushButton(widget);
        ClearBtn->setObjectName("ClearBtn");

        verticalLayout->addWidget(ClearBtn);

        SendBtn = new QPushButton(widget);
        SendBtn->setObjectName("SendBtn");

        verticalLayout->addWidget(SendBtn);


        verticalLayout_2->addLayout(verticalLayout);


        retranslateUi(BaseWidget);

        QMetaObject::connectSlotsByName(BaseWidget);
    } // setupUi

    void retranslateUi(QWidget *BaseWidget)
    {
        BaseWidget->setWindowTitle(QCoreApplication::translate("BaseWidget", "Chat", nullptr));
        hName->setText(QCoreApplication::translate("BaseWidget", "Name", nullptr));
        ClearDataBtn->setText(QCoreApplication::translate("BaseWidget", "\346\270\205\351\231\244\350\201\212\345\244\251\350\256\260\345\275\225", nullptr));
        ClearBtn->setText(QCoreApplication::translate("BaseWidget", "\346\270\205\351\231\244\350\276\223\345\205\245", nullptr));
        SendBtn->setText(QCoreApplication::translate("BaseWidget", "\345\217\221\351\200\201", nullptr));
    } // retranslateUi

};

namespace Ui {
    class BaseWidget: public Ui_BaseWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BASEWIDGET_H

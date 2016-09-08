/********************************************************************************
** Form generated from reading UI file 'bhxserver.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BHXSERVER_H
#define UI_BHXSERVER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_BHXServerClass
{
public:
    QWidget *centralWidget;
    QPushButton *pb_startserver;
    QTextBrowser *tb_log;
    QLabel *lb_ip;
    QPushButton *pb_clean_log;
    QLabel *lb_wx_mac;
    QTextBrowser *tb_wx_mac;
    QLabel *lb_my_auth_code;
    QLineEdit *le_my_tuth_code;
    QPushButton *pb_authorize;
    QLabel *lb_isAuth;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *BHXServerClass)
    {
        if (BHXServerClass->objectName().isEmpty())
            BHXServerClass->setObjectName(QStringLiteral("BHXServerClass"));
        BHXServerClass->resize(890, 430);
        centralWidget = new QWidget(BHXServerClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        pb_startserver = new QPushButton(centralWidget);
        pb_startserver->setObjectName(QStringLiteral("pb_startserver"));
        pb_startserver->setGeometry(QRect(100, 20, 391, 23));
        tb_log = new QTextBrowser(centralWidget);
        tb_log->setObjectName(QStringLiteral("tb_log"));
        tb_log->setGeometry(QRect(10, 70, 581, 271));
        lb_ip = new QLabel(centralWidget);
        lb_ip->setObjectName(QStringLiteral("lb_ip"));
        lb_ip->setGeometry(QRect(260, 50, 131, 20));
        pb_clean_log = new QPushButton(centralWidget);
        pb_clean_log->setObjectName(QStringLiteral("pb_clean_log"));
        pb_clean_log->setGeometry(QRect(250, 350, 75, 23));
        lb_wx_mac = new QLabel(centralWidget);
        lb_wx_mac->setObjectName(QStringLiteral("lb_wx_mac"));
        lb_wx_mac->setGeometry(QRect(600, 130, 101, 20));
        tb_wx_mac = new QTextBrowser(centralWidget);
        tb_wx_mac->setObjectName(QStringLiteral("tb_wx_mac"));
        tb_wx_mac->setGeometry(QRect(600, 160, 256, 181));
        lb_my_auth_code = new QLabel(centralWidget);
        lb_my_auth_code->setObjectName(QStringLiteral("lb_my_auth_code"));
        lb_my_auth_code->setGeometry(QRect(600, 70, 71, 16));
        le_my_tuth_code = new QLineEdit(centralWidget);
        le_my_tuth_code->setObjectName(QStringLiteral("le_my_tuth_code"));
        le_my_tuth_code->setGeometry(QRect(600, 100, 191, 21));
        pb_authorize = new QPushButton(centralWidget);
        pb_authorize->setObjectName(QStringLiteral("pb_authorize"));
        pb_authorize->setGeometry(QRect(800, 100, 61, 23));
        lb_isAuth = new QLabel(centralWidget);
        lb_isAuth->setObjectName(QStringLiteral("lb_isAuth"));
        lb_isAuth->setGeometry(QRect(280, 0, 54, 12));
        BHXServerClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(BHXServerClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 890, 23));
        BHXServerClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(BHXServerClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        BHXServerClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(BHXServerClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        BHXServerClass->setStatusBar(statusBar);

        retranslateUi(BHXServerClass);

        QMetaObject::connectSlotsByName(BHXServerClass);
    } // setupUi

    void retranslateUi(QMainWindow *BHXServerClass)
    {
        BHXServerClass->setWindowTitle(QApplication::translate("BHXServerClass", "BHXServer", 0));
        pb_startserver->setText(QApplication::translate("BHXServerClass", "\345\220\257\345\212\250\346\234\215\345\212\241\345\231\250", 0));
        lb_ip->setText(QApplication::translate("BHXServerClass", "ip:", 0));
        pb_clean_log->setText(QApplication::translate("BHXServerClass", "\346\270\205\347\220\206\346\266\210\346\201\257", 0));
        lb_wx_mac->setText(QApplication::translate("BHXServerClass", "\346\234\272\345\231\250\347\240\201\350\257\246\346\203\205:", 0));
        lb_my_auth_code->setText(QApplication::translate("BHXServerClass", "\346\234\254\346\234\272\346\216\210\346\235\203\347\240\201:", 0));
        pb_authorize->setText(QApplication::translate("BHXServerClass", "\346\216\210\346\235\203", 0));
        lb_isAuth->setText(QApplication::translate("BHXServerClass", "\346\234\252\346\216\210\346\235\203", 0));
    } // retranslateUi

};

namespace Ui {
    class BHXServerClass: public Ui_BHXServerClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BHXSERVER_H

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
    QLabel *lb_dahao;
    QLineEdit *lineEdit;
    QLabel *lb_xiaohao;
    QLineEdit *lineEdit_2;
    QLabel *lb_dh_mac;
    QLabel *lb_xh_mac;
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
        lb_dahao = new QLabel(centralWidget);
        lb_dahao->setObjectName(QStringLiteral("lb_dahao"));
        lb_dahao->setGeometry(QRect(660, 20, 54, 12));
        lineEdit = new QLineEdit(centralWidget);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setGeometry(QRect(660, 60, 113, 20));
        lb_xiaohao = new QLabel(centralWidget);
        lb_xiaohao->setObjectName(QStringLiteral("lb_xiaohao"));
        lb_xiaohao->setGeometry(QRect(660, 120, 31, 16));
        lineEdit_2 = new QLineEdit(centralWidget);
        lineEdit_2->setObjectName(QStringLiteral("lineEdit_2"));
        lineEdit_2->setGeometry(QRect(660, 160, 113, 20));
        lb_dh_mac = new QLabel(centralWidget);
        lb_dh_mac->setObjectName(QStringLiteral("lb_dh_mac"));
        lb_dh_mac->setGeometry(QRect(660, 40, 41, 16));
        lb_xh_mac = new QLabel(centralWidget);
        lb_xh_mac->setObjectName(QStringLiteral("lb_xh_mac"));
        lb_xh_mac->setGeometry(QRect(660, 140, 41, 16));
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
        lb_dahao->setText(QApplication::translate("BHXServerClass", "\345\244\247\345\217\267", 0));
        lb_xiaohao->setText(QApplication::translate("BHXServerClass", "\345\260\217\345\217\267", 0));
        lb_dh_mac->setText(QApplication::translate("BHXServerClass", "\346\216\210\346\235\203\347\240\201:", 0));
        lb_xh_mac->setText(QApplication::translate("BHXServerClass", "\346\216\210\346\235\203\347\240\201:", 0));
    } // retranslateUi

};

namespace Ui {
    class BHXServerClass: public Ui_BHXServerClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BHXSERVER_H

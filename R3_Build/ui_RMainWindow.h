/********************************************************************************
** Form generated from reading UI file 'RMainWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RMAINWINDOW_H
#define UI_RMAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RMainWindow
{
public:
    QWidget *centralwidget;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *RMainWindow)
    {
        if (RMainWindow->objectName().isEmpty())
            RMainWindow->setObjectName(QString::fromUtf8("RMainWindow"));
        RMainWindow->resize(800, 600);
        centralwidget = new QWidget(RMainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        RMainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(RMainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        RMainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(RMainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        RMainWindow->setStatusBar(statusbar);

        retranslateUi(RMainWindow);

        QMetaObject::connectSlotsByName(RMainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *RMainWindow)
    {
        RMainWindow->setWindowTitle(QCoreApplication::translate("RMainWindow", "RMainWindow", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RMainWindow: public Ui_RMainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RMAINWINDOW_H

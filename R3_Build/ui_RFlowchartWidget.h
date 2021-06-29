/********************************************************************************
** Form generated from reading UI file 'RFlowchartWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RFLOWCHARTWIDGET_H
#define UI_RFLOWCHARTWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RFlowchartWidgetUi
{
public:

    void setupUi(QWidget *RFlowchartWidgetUi)
    {
        if (RFlowchartWidgetUi->objectName().isEmpty())
            RFlowchartWidgetUi->setObjectName(QString::fromUtf8("RFlowchartWidgetUi"));
        RFlowchartWidgetUi->resize(400, 300);

        retranslateUi(RFlowchartWidgetUi);

        QMetaObject::connectSlotsByName(RFlowchartWidgetUi);
    } // setupUi

    void retranslateUi(QWidget *RFlowchartWidgetUi)
    {
        RFlowchartWidgetUi->setWindowTitle(QCoreApplication::translate("RFlowchartWidgetUi", "RFlowchartWidget", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RFlowchartWidgetUi: public Ui_RFlowchartWidgetUi {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RFLOWCHARTWIDGET_H

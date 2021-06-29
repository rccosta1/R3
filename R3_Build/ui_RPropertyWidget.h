/********************************************************************************
** Form generated from reading UI file 'RPropertyWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RPROPERTYWIDGET_H
#define UI_RPROPERTYWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RPropertyWidgetUi
{
public:
    QVBoxLayout *verticalLayout;
    QTableWidget *tableWidget;

    void setupUi(QWidget *RPropertyWidgetUi)
    {
        if (RPropertyWidgetUi->objectName().isEmpty())
            RPropertyWidgetUi->setObjectName(QString::fromUtf8("RPropertyWidgetUi"));
        RPropertyWidgetUi->resize(400, 300);
        verticalLayout = new QVBoxLayout(RPropertyWidgetUi);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        tableWidget = new QTableWidget(RPropertyWidgetUi);
        if (tableWidget->columnCount() < 2)
            tableWidget->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));
        tableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        tableWidget->setAlternatingRowColors(false);
        tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableWidget->setCornerButtonEnabled(false);

        verticalLayout->addWidget(tableWidget);


        retranslateUi(RPropertyWidgetUi);

        QMetaObject::connectSlotsByName(RPropertyWidgetUi);
    } // setupUi

    void retranslateUi(QWidget *RPropertyWidgetUi)
    {
        RPropertyWidgetUi->setWindowTitle(QCoreApplication::translate("RPropertyWidgetUi", "RPropertyWidget", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("RPropertyWidgetUi", "Property", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("RPropertyWidgetUi", "Value", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RPropertyWidgetUi: public Ui_RPropertyWidgetUi {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RPROPERTYWIDGET_H

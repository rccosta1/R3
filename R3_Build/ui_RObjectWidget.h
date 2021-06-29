/********************************************************************************
** Form generated from reading UI file 'RObjectWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ROBJECTWIDGET_H
#define UI_ROBJECTWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RObjectWidgetUi
{
public:
    QVBoxLayout *verticalLayout_3;
    QTreeWidget *treeWidget;

    void setupUi(QWidget *RObjectWidgetUi)
    {
        if (RObjectWidgetUi->objectName().isEmpty())
            RObjectWidgetUi->setObjectName(QString::fromUtf8("RObjectWidgetUi"));
        RObjectWidgetUi->resize(400, 300);
        verticalLayout_3 = new QVBoxLayout(RObjectWidgetUi);
        verticalLayout_3->setSpacing(0);
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        treeWidget = new QTreeWidget(RObjectWidgetUi);
        treeWidget->setObjectName(QString::fromUtf8("treeWidget"));
        treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
        treeWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
        treeWidget->setUniformRowHeights(true);
        treeWidget->setAnimated(true);
        treeWidget->setAllColumnsShowFocus(true);
        treeWidget->setHeaderHidden(true);

        verticalLayout_3->addWidget(treeWidget);


        retranslateUi(RObjectWidgetUi);

        QMetaObject::connectSlotsByName(RObjectWidgetUi);
    } // setupUi

    void retranslateUi(QWidget *RObjectWidgetUi)
    {
        RObjectWidgetUi->setWindowTitle(QCoreApplication::translate("RObjectWidgetUi", "RObjectWidget", nullptr));
        QTreeWidgetItem *___qtreewidgetitem = treeWidget->headerItem();
        ___qtreewidgetitem->setText(0, QCoreApplication::translate("RObjectWidgetUi", "1", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RObjectWidgetUi: public Ui_RObjectWidgetUi {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ROBJECTWIDGET_H

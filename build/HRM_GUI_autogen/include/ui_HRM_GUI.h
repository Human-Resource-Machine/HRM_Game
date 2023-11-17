/********************************************************************************
** Form generated from reading UI file 'HRM_GUI.ui'
**
** Created by: Qt User Interface Compiler version 6.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HRM_GUI_H
#define UI_HRM_GUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_HRM_GUI
{
public:
    QWidget *centralwidget;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *HRM_GUI)
    {
        if (HRM_GUI->objectName().isEmpty())
            HRM_GUI->setObjectName("HRM_GUI");
        HRM_GUI->resize(800, 600);
        centralwidget = new QWidget(HRM_GUI);
        centralwidget->setObjectName("centralwidget");
        HRM_GUI->setCentralWidget(centralwidget);
        menubar = new QMenuBar(HRM_GUI);
        menubar->setObjectName("menubar");
        HRM_GUI->setMenuBar(menubar);
        statusbar = new QStatusBar(HRM_GUI);
        statusbar->setObjectName("statusbar");
        HRM_GUI->setStatusBar(statusbar);

        retranslateUi(HRM_GUI);

        QMetaObject::connectSlotsByName(HRM_GUI);
    } // setupUi

    void retranslateUi(QMainWindow *HRM_GUI)
    {
        HRM_GUI->setWindowTitle(QCoreApplication::translate("HRM_GUI", "HRM_GUI", nullptr));
    } // retranslateUi

};

namespace Ui {
    class HRM_GUI: public Ui_HRM_GUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HRM_GUI_H

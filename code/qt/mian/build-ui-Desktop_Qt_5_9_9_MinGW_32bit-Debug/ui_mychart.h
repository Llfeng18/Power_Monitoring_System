/********************************************************************************
** Form generated from reading UI file 'mychart.ui'
**
** Created by: Qt User Interface Compiler version 5.9.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MYCHART_H
#define UI_MYCHART_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MyChart
{
public:
    QWidget *centralwidget;
    QPushButton *pushButton;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QLineEdit *lineEdit_2;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *lineEdit;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MyChart)
    {
        if (MyChart->objectName().isEmpty())
            MyChart->setObjectName(QStringLiteral("MyChart"));
        MyChart->resize(800, 600);
        centralwidget = new QWidget(MyChart);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        pushButton = new QPushButton(centralwidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(640, 510, 121, 31));
        layoutWidget = new QWidget(centralwidget);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(430, 490, 181, 81));
        horizontalLayout_2 = new QHBoxLayout(layoutWidget);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        label_2 = new QLabel(layoutWidget);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout_2->addWidget(label_2);

        lineEdit_2 = new QLineEdit(layoutWidget);
        lineEdit_2->setObjectName(QStringLiteral("lineEdit_2"));

        horizontalLayout_2->addWidget(lineEdit_2);

        widget = new QWidget(centralwidget);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(30, 490, 361, 81));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(widget);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout->addWidget(label);

        lineEdit = new QLineEdit(widget);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));

        horizontalLayout->addWidget(lineEdit);

        MyChart->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MyChart);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 21));
        MyChart->setMenuBar(menubar);
        statusbar = new QStatusBar(MyChart);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        MyChart->setStatusBar(statusbar);

        retranslateUi(MyChart);

        QMetaObject::connectSlotsByName(MyChart);
    } // setupUi

    void retranslateUi(QMainWindow *MyChart)
    {
        MyChart->setWindowTitle(QApplication::translate("MyChart", "MainWindow", Q_NULLPTR));
        pushButton->setText(QApplication::translate("MyChart", "\347\273\230\345\210\266\346\233\262\347\272\277", Q_NULLPTR));
        label_2->setText(QApplication::translate("MyChart", "\346\227\245\346\234\237:", Q_NULLPTR));
        label->setText(QApplication::translate("MyChart", "\347\224\265\350\241\250\351\200\232\344\277\241\345\234\260\345\235\200:", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MyChart: public Ui_MyChart {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MYCHART_H

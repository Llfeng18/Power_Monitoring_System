/********************************************************************************
** Form generated from reading UI file 'meterdetail.ui'
**
** Created by: Qt User Interface Compiler version 5.9.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_METERDETAIL_H
#define UI_METERDETAIL_H

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
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MeterDetail
{
public:
    QWidget *centralwidget;
    QWidget *widget_6;
    QWidget *widget_10;
    QHBoxLayout *horizontalLayout_4;
    QWidget *widget_11;
    QLabel *label_12;
    QLabel *label_13;
    QLabel *label_14;
    QLabel *label_15;
    QLabel *label_16;
    QLabel *label_17;
    QWidget *widget_12;
    QLineEdit *voltage;
    QLineEdit *current;
    QLineEdit *power;
    QLineEdit *factor;
    QLineEdit *phase;
    QLineEdit *temperature;
    QLabel *label_18;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MeterDetail)
    {
        if (MeterDetail->objectName().isEmpty())
            MeterDetail->setObjectName(QStringLiteral("MeterDetail"));
        MeterDetail->resize(400, 370);
        centralwidget = new QWidget(MeterDetail);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        widget_6 = new QWidget(centralwidget);
        widget_6->setObjectName(QStringLiteral("widget_6"));
        widget_6->setGeometry(QRect(20, 10, 371, 311));
        widget_6->setMinimumSize(QSize(200, 200));
        widget_10 = new QWidget(widget_6);
        widget_10->setObjectName(QStringLiteral("widget_10"));
        widget_10->setGeometry(QRect(0, 40, 351, 321));
        widget_10->setMinimumSize(QSize(190, 160));
        horizontalLayout_4 = new QHBoxLayout(widget_10);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        widget_11 = new QWidget(widget_10);
        widget_11->setObjectName(QStringLiteral("widget_11"));
        widget_11->setMinimumSize(QSize(80, 140));
        label_12 = new QLabel(widget_11);
        label_12->setObjectName(QStringLiteral("label_12"));
        label_12->setGeometry(QRect(9, 9, 51, 21));
        label_13 = new QLabel(widget_11);
        label_13->setObjectName(QStringLiteral("label_13"));
        label_13->setGeometry(QRect(10, 50, 61, 21));
        label_14 = new QLabel(widget_11);
        label_14->setObjectName(QStringLiteral("label_14"));
        label_14->setGeometry(QRect(10, 90, 61, 21));
        label_15 = new QLabel(widget_11);
        label_15->setObjectName(QStringLiteral("label_15"));
        label_15->setGeometry(QRect(10, 140, 81, 21));
        label_16 = new QLabel(widget_11);
        label_16->setObjectName(QStringLiteral("label_16"));
        label_16->setGeometry(QRect(10, 180, 51, 21));
        label_17 = new QLabel(widget_11);
        label_17->setObjectName(QStringLiteral("label_17"));
        label_17->setGeometry(QRect(10, 220, 51, 21));
        widget_12 = new QWidget(widget_11);
        widget_12->setObjectName(QStringLiteral("widget_12"));
        widget_12->setGeometry(QRect(170, -10, 181, 281));
        widget_12->setMinimumSize(QSize(120, 150));
        voltage = new QLineEdit(widget_12);
        voltage->setObjectName(QStringLiteral("voltage"));
        voltage->setGeometry(QRect(9, 22, 108, 22));
        current = new QLineEdit(widget_12);
        current->setObjectName(QStringLiteral("current"));
        current->setGeometry(QRect(9, 63, 108, 22));
        power = new QLineEdit(widget_12);
        power->setObjectName(QStringLiteral("power"));
        power->setGeometry(QRect(9, 104, 108, 22));
        factor = new QLineEdit(widget_12);
        factor->setObjectName(QStringLiteral("factor"));
        factor->setGeometry(QRect(10, 150, 108, 22));
        phase = new QLineEdit(widget_12);
        phase->setObjectName(QStringLiteral("phase"));
        phase->setGeometry(QRect(10, 190, 108, 22));
        temperature = new QLineEdit(widget_12);
        temperature->setObjectName(QStringLiteral("temperature"));
        temperature->setGeometry(QRect(10, 230, 108, 22));

        horizontalLayout_4->addWidget(widget_11);

        label_18 = new QLabel(widget_6);
        label_18->setObjectName(QStringLiteral("label_18"));
        label_18->setGeometry(QRect(130, 10, 80, 35));
        label_18->setMinimumSize(QSize(60, 30));
        MeterDetail->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MeterDetail);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 400, 21));
        MeterDetail->setMenuBar(menubar);
        statusbar = new QStatusBar(MeterDetail);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        MeterDetail->setStatusBar(statusbar);

        retranslateUi(MeterDetail);

        QMetaObject::connectSlotsByName(MeterDetail);
    } // setupUi

    void retranslateUi(QMainWindow *MeterDetail)
    {
        MeterDetail->setWindowTitle(QApplication::translate("MeterDetail", "MainWindow", Q_NULLPTR));
        label_12->setText(QApplication::translate("MeterDetail", "\347\224\265\345\216\213:", Q_NULLPTR));
        label_13->setText(QApplication::translate("MeterDetail", "\347\224\265\346\265\201:", Q_NULLPTR));
        label_14->setText(QApplication::translate("MeterDetail", "\345\212\237\347\216\207:", Q_NULLPTR));
        label_15->setText(QApplication::translate("MeterDetail", "\345\212\237\347\216\207\345\233\240\347\264\240:", Q_NULLPTR));
        label_16->setText(QApplication::translate("MeterDetail", "\347\233\270\344\275\215:", Q_NULLPTR));
        label_17->setText(QApplication::translate("MeterDetail", "\346\270\251\345\272\246:", Q_NULLPTR));
        label_18->setText(QApplication::translate("MeterDetail", "  \347\224\265\350\241\2501", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MeterDetail: public Ui_MeterDetail {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_METERDETAIL_H

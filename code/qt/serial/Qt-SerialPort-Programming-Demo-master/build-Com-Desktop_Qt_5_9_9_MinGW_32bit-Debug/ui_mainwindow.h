/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QGroupBox *groupBox;
    QTextBrowser *comLog;
    QGroupBox *groupBox_2;
    QCheckBox *openCom;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QWidget *widget_2;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QLabel *label_5;
    QLabel *label_2;
    QLabel *label_4;
    QLabel *label_3;
    QWidget *widget1;
    QVBoxLayout *verticalLayout_2;
    QComboBox *portName;
    QComboBox *dataBits;
    QComboBox *baudRate;
    QComboBox *stopBits;
    QComboBox *parity;
    QGroupBox *groupBox_3;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout_6;
    QTextEdit *message;
    QPushButton *btnSend;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(660, 494);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(10, 10, 291, 351));
        comLog = new QTextBrowser(groupBox);
        comLog->setObjectName(QStringLiteral("comLog"));
        comLog->setGeometry(QRect(10, 30, 261, 311));
        groupBox_2 = new QGroupBox(centralWidget);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(320, 10, 301, 351));
        openCom = new QCheckBox(groupBox_2);
        openCom->setObjectName(QStringLiteral("openCom"));
        openCom->setGeometry(QRect(30, 310, 131, 31));
        widget = new QWidget(groupBox_2);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(10, 30, 241, 262));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        widget_2 = new QWidget(widget);
        widget_2->setObjectName(QStringLiteral("widget_2"));
        verticalLayout = new QVBoxLayout(widget_2);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        label = new QLabel(widget_2);
        label->setObjectName(QStringLiteral("label"));

        verticalLayout->addWidget(label);

        label_5 = new QLabel(widget_2);
        label_5->setObjectName(QStringLiteral("label_5"));

        verticalLayout->addWidget(label_5);

        label_2 = new QLabel(widget_2);
        label_2->setObjectName(QStringLiteral("label_2"));

        verticalLayout->addWidget(label_2);

        label_4 = new QLabel(widget_2);
        label_4->setObjectName(QStringLiteral("label_4"));

        verticalLayout->addWidget(label_4);

        label_3 = new QLabel(widget_2);
        label_3->setObjectName(QStringLiteral("label_3"));

        verticalLayout->addWidget(label_3);


        horizontalLayout->addWidget(widget_2);

        widget1 = new QWidget(widget);
        widget1->setObjectName(QStringLiteral("widget1"));
        widget1->setMinimumSize(QSize(140, 260));
        verticalLayout_2 = new QVBoxLayout(widget1);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        portName = new QComboBox(widget1);
        portName->setObjectName(QStringLiteral("portName"));

        verticalLayout_2->addWidget(portName);

        dataBits = new QComboBox(widget1);
        dataBits->setObjectName(QStringLiteral("dataBits"));

        verticalLayout_2->addWidget(dataBits);

        baudRate = new QComboBox(widget1);
        baudRate->setObjectName(QStringLiteral("baudRate"));

        verticalLayout_2->addWidget(baudRate);

        stopBits = new QComboBox(widget1);
        stopBits->setObjectName(QStringLiteral("stopBits"));

        verticalLayout_2->addWidget(stopBits);

        parity = new QComboBox(widget1);
        parity->setObjectName(QStringLiteral("parity"));

        verticalLayout_2->addWidget(parity);


        horizontalLayout->addWidget(widget1);

        groupBox_3 = new QGroupBox(centralWidget);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setGeometry(QRect(10, 370, 641, 111));
        layoutWidget = new QWidget(groupBox_3);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 20, 621, 81));
        horizontalLayout_6 = new QHBoxLayout(layoutWidget);
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        horizontalLayout_6->setContentsMargins(0, 0, 0, 0);
        message = new QTextEdit(layoutWidget);
        message->setObjectName(QStringLiteral("message"));

        horizontalLayout_6->addWidget(message);

        btnSend = new QPushButton(layoutWidget);
        btnSend->setObjectName(QStringLiteral("btnSend"));
        btnSend->setEnabled(false);

        horizontalLayout_6->addWidget(btnSend);

        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Qt\344\270\262\345\217\243\351\200\232\344\277\241", Q_NULLPTR));
        groupBox->setTitle(QApplication::translate("MainWindow", "\351\200\232\344\277\241\346\227\245\345\277\227", Q_NULLPTR));
        groupBox_2->setTitle(QApplication::translate("MainWindow", "\344\270\262\345\217\243\345\217\202\346\225\260", Q_NULLPTR));
        openCom->setText(QApplication::translate("MainWindow", "\346\211\223\345\274\200\344\270\262\345\217\243", Q_NULLPTR));
        label->setText(QApplication::translate("MainWindow", "\344\270\262\345\217\243\357\274\232", Q_NULLPTR));
        label_5->setText(QApplication::translate("MainWindow", "\346\225\260\346\215\256\344\275\215\357\274\232", Q_NULLPTR));
        label_2->setText(QApplication::translate("MainWindow", "\346\263\242\347\211\271\347\216\207\357\274\232", Q_NULLPTR));
        label_4->setText(QApplication::translate("MainWindow", "\345\201\234\346\255\242\344\275\215\357\274\232", Q_NULLPTR));
        label_3->setText(QApplication::translate("MainWindow", "\346\240\241\351\252\214\344\275\215\357\274\232", Q_NULLPTR));
        dataBits->clear();
        dataBits->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "8", Q_NULLPTR)
         << QApplication::translate("MainWindow", "7", Q_NULLPTR)
         << QApplication::translate("MainWindow", "6", Q_NULLPTR)
         << QApplication::translate("MainWindow", "5", Q_NULLPTR)
        );
        baudRate->clear();
        baudRate->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "1200", Q_NULLPTR)
         << QApplication::translate("MainWindow", "2400", Q_NULLPTR)
         << QApplication::translate("MainWindow", "4800", Q_NULLPTR)
         << QApplication::translate("MainWindow", "9600", Q_NULLPTR)
         << QApplication::translate("MainWindow", "4800", Q_NULLPTR)
         << QApplication::translate("MainWindow", "19200", Q_NULLPTR)
         << QApplication::translate("MainWindow", "38400", Q_NULLPTR)
         << QApplication::translate("MainWindow", "57600", Q_NULLPTR)
         << QApplication::translate("MainWindow", "115200", Q_NULLPTR)
        );
        stopBits->clear();
        stopBits->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "1", Q_NULLPTR)
         << QApplication::translate("MainWindow", "2", Q_NULLPTR)
        );
        parity->clear();
        parity->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "NoParity", Q_NULLPTR)
         << QApplication::translate("MainWindow", "EvenParity", Q_NULLPTR)
         << QApplication::translate("MainWindow", "OddParity", Q_NULLPTR)
        );
        groupBox_3->setTitle(QApplication::translate("MainWindow", "   \345\217\221\351\200\201", Q_NULLPTR));
        btnSend->setText(QApplication::translate("MainWindow", "\345\217\221\351\200\201", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H

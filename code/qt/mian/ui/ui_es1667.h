/********************************************************************************
** Form generated from reading UI file 'es1667.ui'
**
** Created by: Qt User Interface Compiler version 5.9.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ES1667_H
#define UI_ES1667_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ES1667
{
public:
    QWidget *centralwidget;
    QGroupBox *groupBox_3;
    QWidget *layoutWidget_2;
    QHBoxLayout *horizontalLayout_2;
    QWidget *widget_6;
    QVBoxLayout *verticalLayout_5;
    QLabel *label;
    QLabel *label_8;
    QLabel *label_9;
    QLabel *label_10;
    QLabel *label_11;
    QWidget *widget_8;
    QVBoxLayout *verticalLayout_6;
    QComboBox *portName;
    QComboBox *dataBits_2;
    QComboBox *baudRate_2;
    QComboBox *stopBits_2;
    QComboBox *parity_2;
    QPushButton *pushButton;
    QWidget *widget;
    QWidget *widget_7;
    QWidget *widget_4;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_6;
    QLabel *label_7;
    QWidget *widget_5;
    QVBoxLayout *verticalLayout_4;
    QLineEdit *lineEdit;
    QLineEdit *lineEdit_2;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_7;
    QHBoxLayout *horizontalLayout;
    QWidget *widget_2;
    QVBoxLayout *verticalLayout;
    QLabel *label_5;
    QLabel *label_2;
    QLabel *label_4;
    QLabel *label_3;
    QWidget *widget_3;
    QVBoxLayout *verticalLayout_2;
    QComboBox *dataBits;
    QComboBox *baudRate;
    QComboBox *stopBits;
    QComboBox *parity;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *ES1667)
    {
        if (ES1667->objectName().isEmpty())
            ES1667->setObjectName(QStringLiteral("ES1667"));
        ES1667->resize(800, 460);
        centralwidget = new QWidget(ES1667);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        groupBox_3 = new QGroupBox(centralwidget);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setGeometry(QRect(390, 40, 351, 371));
        groupBox_3->setMinimumSize(QSize(301, 351));
        layoutWidget_2 = new QWidget(groupBox_3);
        layoutWidget_2->setObjectName(QStringLiteral("layoutWidget_2"));
        layoutWidget_2->setGeometry(QRect(10, 30, 311, 262));
        horizontalLayout_2 = new QHBoxLayout(layoutWidget_2);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        widget_6 = new QWidget(layoutWidget_2);
        widget_6->setObjectName(QStringLiteral("widget_6"));
        verticalLayout_5 = new QVBoxLayout(widget_6);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        label = new QLabel(widget_6);
        label->setObjectName(QStringLiteral("label"));

        verticalLayout_5->addWidget(label);

        label_8 = new QLabel(widget_6);
        label_8->setObjectName(QStringLiteral("label_8"));

        verticalLayout_5->addWidget(label_8);

        label_9 = new QLabel(widget_6);
        label_9->setObjectName(QStringLiteral("label_9"));

        verticalLayout_5->addWidget(label_9);

        label_10 = new QLabel(widget_6);
        label_10->setObjectName(QStringLiteral("label_10"));

        verticalLayout_5->addWidget(label_10);

        label_11 = new QLabel(widget_6);
        label_11->setObjectName(QStringLiteral("label_11"));

        verticalLayout_5->addWidget(label_11);


        horizontalLayout_2->addWidget(widget_6);

        widget_8 = new QWidget(layoutWidget_2);
        widget_8->setObjectName(QStringLiteral("widget_8"));
        widget_8->setMinimumSize(QSize(160, 260));
        verticalLayout_6 = new QVBoxLayout(widget_8);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        portName = new QComboBox(widget_8);
        portName->setObjectName(QStringLiteral("portName"));

        verticalLayout_6->addWidget(portName);

        dataBits_2 = new QComboBox(widget_8);
        dataBits_2->setObjectName(QStringLiteral("dataBits_2"));

        verticalLayout_6->addWidget(dataBits_2);

        baudRate_2 = new QComboBox(widget_8);
        baudRate_2->setObjectName(QStringLiteral("baudRate_2"));

        verticalLayout_6->addWidget(baudRate_2);

        stopBits_2 = new QComboBox(widget_8);
        stopBits_2->setObjectName(QStringLiteral("stopBits_2"));

        verticalLayout_6->addWidget(stopBits_2);

        parity_2 = new QComboBox(widget_8);
        parity_2->setObjectName(QStringLiteral("parity_2"));

        verticalLayout_6->addWidget(parity_2);


        horizontalLayout_2->addWidget(widget_8);

        pushButton = new QPushButton(groupBox_3);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(90, 310, 140, 40));
        pushButton->setMinimumSize(QSize(140, 40));
        widget = new QWidget(centralwidget);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(10, 10, 361, 451));
        widget->setMinimumSize(QSize(301, 391));
        widget_7 = new QWidget(widget);
        widget_7->setObjectName(QStringLiteral("widget_7"));
        widget_7->setGeometry(QRect(10, 20, 341, 91));
        widget_7->setMinimumSize(QSize(300, 80));
        widget_4 = new QWidget(widget_7);
        widget_4->setObjectName(QStringLiteral("widget_4"));
        widget_4->setGeometry(QRect(-1, 0, 171, 80));
        widget_4->setMinimumSize(QSize(155, 80));
        verticalLayout_3 = new QVBoxLayout(widget_4);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        label_6 = new QLabel(widget_4);
        label_6->setObjectName(QStringLiteral("label_6"));

        verticalLayout_3->addWidget(label_6);

        label_7 = new QLabel(widget_4);
        label_7->setObjectName(QStringLiteral("label_7"));

        verticalLayout_3->addWidget(label_7);

        widget_5 = new QWidget(widget_7);
        widget_5->setObjectName(QStringLiteral("widget_5"));
        widget_5->setGeometry(QRect(159, 0, 180, 81));
        widget_5->setMinimumSize(QSize(180, 80));
        verticalLayout_4 = new QVBoxLayout(widget_5);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        lineEdit = new QLineEdit(widget_5);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setEchoMode(QLineEdit::Normal);

        verticalLayout_4->addWidget(lineEdit);

        lineEdit_2 = new QLineEdit(widget_5);
        lineEdit_2->setObjectName(QStringLiteral("lineEdit_2"));
        lineEdit_2->setEchoMode(QLineEdit::Normal);

        verticalLayout_4->addWidget(lineEdit_2);

        groupBox_2 = new QGroupBox(widget);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(20, 120, 330, 271));
        groupBox_2->setMinimumSize(QSize(0, 0));
        verticalLayout_7 = new QVBoxLayout(groupBox_2);
        verticalLayout_7->setObjectName(QStringLiteral("verticalLayout_7"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        widget_2 = new QWidget(groupBox_2);
        widget_2->setObjectName(QStringLiteral("widget_2"));
        widget_2->setMinimumSize(QSize(0, 200));
        verticalLayout = new QVBoxLayout(widget_2);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
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

        widget_3 = new QWidget(groupBox_2);
        widget_3->setObjectName(QStringLiteral("widget_3"));
        widget_3->setMinimumSize(QSize(160, 230));
        verticalLayout_2 = new QVBoxLayout(widget_3);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        dataBits = new QComboBox(widget_3);
        dataBits->setObjectName(QStringLiteral("dataBits"));

        verticalLayout_2->addWidget(dataBits);

        baudRate = new QComboBox(widget_3);
        baudRate->setObjectName(QStringLiteral("baudRate"));

        verticalLayout_2->addWidget(baudRate);

        stopBits = new QComboBox(widget_3);
        stopBits->setObjectName(QStringLiteral("stopBits"));

        verticalLayout_2->addWidget(stopBits);

        parity = new QComboBox(widget_3);
        parity->setObjectName(QStringLiteral("parity"));

        verticalLayout_2->addWidget(parity);


        horizontalLayout->addWidget(widget_3);


        verticalLayout_7->addLayout(horizontalLayout);

        ES1667->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(ES1667);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        ES1667->setStatusBar(statusbar);

        retranslateUi(ES1667);

        QMetaObject::connectSlotsByName(ES1667);
    } // setupUi

    void retranslateUi(QMainWindow *ES1667)
    {
        ES1667->setWindowTitle(QApplication::translate("ES1667", "MainWindow", Q_NULLPTR));
        groupBox_3->setTitle(QString());
        label->setText(QApplication::translate("ES1667", "645\345\217\221\351\200\201\357\274\232", Q_NULLPTR));
        label_8->setText(QApplication::translate("ES1667", "\346\225\260\346\215\256\344\275\215\357\274\232", Q_NULLPTR));
        label_9->setText(QApplication::translate("ES1667", "\346\263\242\347\211\271\347\216\207\357\274\232", Q_NULLPTR));
        label_10->setText(QApplication::translate("ES1667", "\345\201\234\346\255\242\344\275\215\357\274\232", Q_NULLPTR));
        label_11->setText(QApplication::translate("ES1667", "\346\240\241\351\252\214\344\275\215\357\274\232", Q_NULLPTR));
        portName->clear();
        portName->insertItems(0, QStringList()
         << QApplication::translate("ES1667", "\345\215\225\346\222\255", Q_NULLPTR)
         << QApplication::translate("ES1667", "\351\200\217\344\274\240", Q_NULLPTR)
        );
        dataBits_2->clear();
        dataBits_2->insertItems(0, QStringList()
         << QApplication::translate("ES1667", "8", Q_NULLPTR)
         << QApplication::translate("ES1667", "7", Q_NULLPTR)
         << QApplication::translate("ES1667", "6", Q_NULLPTR)
         << QApplication::translate("ES1667", "5", Q_NULLPTR)
        );
        baudRate_2->clear();
        baudRate_2->insertItems(0, QStringList()
         << QApplication::translate("ES1667", "1200", Q_NULLPTR)
         << QApplication::translate("ES1667", "2400", Q_NULLPTR)
         << QApplication::translate("ES1667", "4800", Q_NULLPTR)
         << QApplication::translate("ES1667", "9600", Q_NULLPTR)
         << QApplication::translate("ES1667", "4800", Q_NULLPTR)
         << QApplication::translate("ES1667", "19200", Q_NULLPTR)
         << QApplication::translate("ES1667", "38400", Q_NULLPTR)
         << QApplication::translate("ES1667", "57600", Q_NULLPTR)
         << QApplication::translate("ES1667", "115200", Q_NULLPTR)
        );
        stopBits_2->clear();
        stopBits_2->insertItems(0, QStringList()
         << QApplication::translate("ES1667", "1", Q_NULLPTR)
         << QApplication::translate("ES1667", "2", Q_NULLPTR)
        );
        parity_2->clear();
        parity_2->insertItems(0, QStringList()
         << QApplication::translate("ES1667", "NoParity", Q_NULLPTR)
         << QApplication::translate("ES1667", "EvenParity", Q_NULLPTR)
         << QApplication::translate("ES1667", "OddParity", Q_NULLPTR)
        );
        pushButton->setText(QApplication::translate("ES1667", "\350\256\276\347\275\256\347\275\221\347\273\234\345\217\202\346\225\260", Q_NULLPTR));
        label_6->setText(QApplication::translate("ES1667", "\351\200\232\344\277\241\345\234\260\345\235\200:", Q_NULLPTR));
        label_7->setText(QApplication::translate("ES1667", "\351\200\217\344\274\240\347\255\211\347\272\247\351\231\220\345\210\266:", Q_NULLPTR));
        groupBox_2->setTitle(QApplication::translate("ES1667", "\345\270\247\346\216\247\345\210\266\345\237\237", Q_NULLPTR));
        label_5->setText(QApplication::translate("ES1667", "\346\226\271\345\220\221DIR:", Q_NULLPTR));
        label_2->setText(QApplication::translate("ES1667", "Prm:", Q_NULLPTR));
        label_4->setText(QApplication::translate("ES1667", "Respond:", Q_NULLPTR));
        label_3->setText(QApplication::translate("ES1667", "\344\277\241\351\201\223\347\261\273\345\236\213:", Q_NULLPTR));
        dataBits->clear();
        dataBits->insertItems(0, QStringList()
         << QApplication::translate("ES1667", "\344\273\216\346\234\272\345\217\221\345\207\272\345\270\247", Q_NULLPTR)
         << QApplication::translate("ES1667", "\344\270\273\345\212\250\345\217\221\345\207\272\345\270\247", Q_NULLPTR)
        );
        baudRate->clear();
        baudRate->insertItems(0, QStringList()
         << QApplication::translate("ES1667", "\345\221\275\344\273\244\345\233\236\345\272\224", Q_NULLPTR)
         << QApplication::translate("ES1667", "\344\270\273\345\212\250\345\217\221\350\265\267", Q_NULLPTR)
        );
        stopBits->clear();
        stopBits->insertItems(0, QStringList()
         << QApplication::translate("ES1667", "\345\274\202\345\270\270\345\272\224\347\255\224", Q_NULLPTR)
         << QApplication::translate("ES1667", "\346\255\243\345\270\270\345\272\224\347\255\224", Q_NULLPTR)
        );
        parity->clear();
        parity->insertItems(0, QStringList()
         << QApplication::translate("ES1667", "1642PLC", Q_NULLPTR)
         << QApplication::translate("ES1667", "1663PLC", Q_NULLPTR)
         << QApplication::translate("ES1667", "Bluetooth", Q_NULLPTR)
         << QApplication::translate("ES1667", "WiFi", Q_NULLPTR)
        );
    } // retranslateUi

};

namespace Ui {
    class ES1667: public Ui_ES1667 {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ES1667_H

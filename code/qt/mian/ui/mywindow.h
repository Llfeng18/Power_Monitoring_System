#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include <QDialog>
#include <QMenuBar>
#include <QToolBar>
#include <QDebug>
#include <QPushButton>
#include <QStatusBar>
#include <QLabel>
#include <QDockWidget>
#include <QTextEdit>

#include "es1667.h"
#include "mychart.h"
#include "Serial.h"
#include "meterdetail.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    friend void Serial::recvMsg();
public slots:
    void showvoltage(int index);
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void showmess(void);
    void emitvoltage(int num);
    Ui::MainWindow *ui;
//private:

};
#endif // MAINWINDOW_H

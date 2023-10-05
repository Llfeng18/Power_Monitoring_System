#ifndef SERIAL_H
#define SERIAL_H

#include <QMainWindow>

#include <QSerialPort>
#include <QSerialPortInfo>

#include <QList>
#include <QMessageBox>
#include <QDateTime>
#include <QDebug>
#include <QApplication>
#include <QDialog>
#include <QTimer>

namespace Ui {
class Serial;
}

class Serial : public QMainWindow
{
    Q_OBJECT

signals:
    void Setvoltage(int num);
public:
    explicit Serial(QWidget *parent = nullptr);
    ~Serial();
    static uint8_t mailaddress[5][30];
    static uint8_t Total_STA_num;
    static uint8_t temp;
    void Sleep(int msec);
    QSerialPort *serialPort;
    QTimer *update_time;
protected:
    void findFreePorts();
    bool initSerialPort();
    void sendMsg(const QString &msg);

public slots:
    void recvMsg();
    void getMeter();
private:
    Ui::Serial *ui;

};

#endif // SERIAL_H

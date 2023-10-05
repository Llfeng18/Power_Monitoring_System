#include "Serial.h"
#include "ui_serial.h"
#include "mywindow.h"


uint8_t Serial::mailaddress[5][30]={{0}};
uint8_t Serial::Total_STA_num=0;
uint8_t Serial::temp=0;
extern QByteArray hexData;
extern int newmes;
extern Serial *seri;
Serial::Serial(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Serial)
{
    ui->setupUi(this);
//    Ui::MainWindow *uiwin=MainWindow::MainWindow->ui;
    this->serialPort = new QSerialPort;
    findFreePorts();

    update_time=new QTimer(this);

    connect(update_time,SIGNAL(timeout()),this,SLOT(getMeter()));

    connect(ui->openCom, &QCheckBox::toggled, [=](bool checked){
        if (checked){
            initSerialPort();
            ui->btnSend->setEnabled(true);

            QString msg="0100";
            Serial::sendMsg(msg.toLatin1());

            Sleep(100);
            msg="0200";
            Serial::sendMsg(msg.toLatin1());

            Sleep(100);
            for(int j=0;j<=Serial::Total_STA_num;j++){
                msg="0300";
                char buf[100];
                for(int i=0;i<6;i++){
                    itoa(mailaddress[j][i], buf, 16);
                    if(mailaddress[j][i]<10){
                        msg=msg+"0";
                    }
                    msg=msg+buf;
                }
                qDebug()<<msg;
                Serial::sendMsg(msg.toLatin1());
                Sleep(100);
            }
//            Sleep(2000);
            msg="0400";
            char buf[100];
            for(int i=0;i<6;i++){
                itoa(mailaddress[1][i+7], buf, 16);
                if(mailaddress[1][i+7]<10){
                    msg=msg+"0";
                }
                msg=msg+buf;
            }
            qDebug()<<msg;
            Serial::sendMsg(msg.toLatin1());
            Sleep(3000);
            qDebug()<<"here";
            this->update_time->start(3000); //30秒钟后启动


        }else{
            this->serialPort->close();
            ui->btnSend->setEnabled(false);
            ui->openCom->setChecked(false);
        }
    });
    connect(this->serialPort, SIGNAL(readyRead()), this, SLOT(recvMsg()));
    connect(ui->btnSend, &QPushButton::clicked, [=](){
        sendMsg(ui->message->toPlainText());
    });
    ui->baudRate->setCurrentText("115200");
    setWindowTitle("串口参数设置");
}

Serial::~Serial()
{
    delete ui;
}


//寻找空闲状态串口
void Serial::findFreePorts(){
    QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
    for (int i = 0; i < ports.size(); ++i){
        if (ports.at(i).isBusy()){
            ports.removeAt(i);
            continue;
        }
        ui->portName->addItem(ports.at(i).portName());
    }
    if (!ports.size()){
        QMessageBox::warning(NULL,"Tip",QStringLiteral("找不到空闲串口"));
        return;
    }
}
//初始化串口
bool Serial::initSerialPort(){
    this->serialPort->setPortName(ui->portName->currentText());
    if (!this->serialPort->open(QIODevice::ReadWrite)){
        QMessageBox::warning(NULL,"Tip",QStringLiteral("串口打开失败"));
        return false;
    }
    this->serialPort->setBaudRate(ui->baudRate->currentText().toInt());

    if (ui->dataBits->currentText().toInt() == 8){
        this->serialPort->setDataBits(QSerialPort::Data8);
    }else if (ui->dataBits->currentText().toInt() == 7){
        this->serialPort->setDataBits(QSerialPort::Data7);
    }else if (ui->dataBits->currentText().toInt() == 6){
        this->serialPort->setDataBits(QSerialPort::Data6);
    }else if (ui->dataBits->currentText().toInt() == 5){
        this->serialPort->setDataBits(QSerialPort::Data5);
    }

    if (ui->stopBits->currentText().toInt() == 1){
        this->serialPort->setStopBits(QSerialPort::OneStop);
    }else if (ui->stopBits->currentText().toInt() == 2){
        this->serialPort->setStopBits(QSerialPort::TwoStop);
    }


    if(ui->parity->currentText() == "NoParity"){
        this->serialPort->setParity(QSerialPort::NoParity);
    }else if (ui->parity->currentText() == "EvenParity"){
        this->serialPort->setParity(QSerialPort::EvenParity);
    }else if (ui->parity->currentText() == "OddParity"){
        this->serialPort->setParity(QSerialPort::OddParity);
    }
    return true;
}
//向串口发送信息
void Serial::sendMsg(const QString &msg){
    qDebug() <<"发送数据: " << msg;
    this->serialPort->write(QByteArray::fromHex(msg.toLatin1()));
    ui->comLog->insertPlainText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + " [send] " + msg + "\n");
}
//接受来自串口的信息
void Serial::recvMsg(){
    Sleep(10);
    QByteArray msg = this->serialPort->readAll();
    if(msg==""){
        return;
    }
    hexData = msg.toHex();
    qDebug() <<"接收数据: "<< hexData;
    if(hexData[1]=='1'){
        for(int i=0;i<6;i++){
            if(hexData[2*(i+2)]<='9'&&hexData[2*(i+2)]>='0'){
                Serial::mailaddress[0][i]=(hexData[2*(i+2)]-'0')*16;
            }else{
                Serial::mailaddress[0][i]=(hexData[2*(i+2)]-'a'+10)*16;
            }
    //            qDebug() <<Serial::mailaddress[0][i];
            if(hexData[2*(i+2)+1]<='9'&&hexData[2*(i+2)+1]>='0'){
                Serial::mailaddress[0][i]=hexData[2*(i+2)+1]-'0'+Serial::mailaddress[0][i];
            }else{
                Serial::mailaddress[0][i]=hexData[2*(i+2)+1]-'a'+10+Serial::mailaddress[0][i];
            }
    //            qDebug() <<Serial::mailaddress[0][i];
    //            Serial::mailaddress[0][i]=hexData[2*i]*16+hexData[2*i+1];
    //            qDebug("%0.2x ",Serial::mailaddress[0][i]);
        }
    }else if(hexData[1]=='2'){
        Serial::Total_STA_num=0;
        if(hexData[4]<='9'&&hexData[4]>='0'){
            Serial::Total_STA_num=(hexData[4]-'0')*16;
        }else{
            Serial::Total_STA_num=(hexData[4]-'a'+10)*16;
        }
        if(hexData[5]<='9'&&hexData[5]>='0'){
            Serial::Total_STA_num=hexData[5]-'0'+Serial::Total_STA_num;
        }else{
            Serial::Total_STA_num=hexData[5]-'a'+10+Serial::Total_STA_num;
        }
    //        qDebug() <<Serial::Total_STA_num;
        for(int i=1;i<=Serial::Total_STA_num;i++){
            for(int j=0;j<6;j++){
                if(hexData[2*(6*(i-1)+3+j)]<='9'&&hexData[2*(6*(i-1)+3+j)]>='0'){
                    Serial::mailaddress[i][j]=(hexData[2*(6*(i-1)+3+j)]-'0')*16;
                }else{
                    Serial::mailaddress[i][j]=(hexData[2*(6*(i-1)+3+j)]-'a'+10)*16;
                }
    //            qDebug() <<Serial::mailaddress[0][i];
                if(hexData[2*(6*(i-1)+3+j)+1]<='9'&&hexData[2*(6*(i-1)+3+j)+1]>='0'){
                    Serial::mailaddress[i][j]=hexData[2*(6*(i-1)+3+j)+1]-'0'+Serial::mailaddress[i][j];
                }else{
                    Serial::mailaddress[i][j]=hexData[2*(6*(i-1)+3+j)+1]-'a'+10+Serial::mailaddress[i][j];
                }
    //            qDebug("%0.2x ",Serial::mailaddress[0][i]);
                }
            }
    }else if(hexData[1]=='3'){
        int nummeter=0;
        if(hexData[4]<='9'&&hexData[4]>='0'){
            nummeter=(hexData[4]-'0')*16;
        }else{
            nummeter=(hexData[4]-'a'+10)*16;
        }
        if(hexData[5]<='9'&&hexData[5]>='0'){
            nummeter=hexData[5]-'0'+nummeter;
        }else{
            nummeter=hexData[5]-'a'+10+nummeter;
        }
//        qDebug() <<nummeter;
        Serial::mailaddress[Serial::temp][6]=nummeter;
        for(int i=0;i<nummeter*6;i++){
            if(hexData[6+i*2]<='9'&&hexData[6+i*2]>='0'){
                Serial::mailaddress[Serial::temp][i+7]=(hexData[6+i*2]-'0')*16;
            }else{
                Serial::mailaddress[Serial::temp][i+7]=(hexData[6+i*2]-'a'+10)*16;
            }
    //            qDebug() <<Serial::mailaddress[0][i];
            if(hexData[7+i*2]<='9'&&hexData[7+i*2]>='0'){
                Serial::mailaddress[Serial::temp][i+7]=hexData[7+i*2]-'0'+Serial::mailaddress[Serial::temp][i+7];
            }else{
                Serial::mailaddress[Serial::temp][i+7]=hexData[7+i*2]-'a'+10+Serial::mailaddress[Serial::temp][i+7];
            }
//            qDebug("%0.2x ",Serial::mailaddress[Serial::temp][i+7]);

            }
        Serial::temp++;
    }else if(hexData[1]=='4'){
        int num,i=0;
        num=(hexData[4+i]-'0')*10000000+(hexData[5+i]-'0')*1000000+(hexData[6+i]-'0')*100000+(hexData[7+i]-'0')*10000+(hexData[8+i]-'0')*1000+(hexData[9+i]-'0')*100+(hexData[10+i]-'0')*10+(hexData[11+i]-'0');
        qDebug()<<"组合有功总电能"<<num/100.0;
        i=i+8;
        num=(hexData[4+i]-'0')*10000000+(hexData[5+i]-'0')*1000000+(hexData[6+i]-'0')*100000+(hexData[7+i]-'0')*10000+(hexData[8+i]-'0')*1000+(hexData[9+i]-'0')*100+(hexData[10+i]-'0')*10+(hexData[11+i]-'0');
        qDebug()<<"正向有功总电能"<<num/100.0;
        i=i+8;
        num=(hexData[4+i]-'0')*10000000+(hexData[5+i]-'0')*1000000+(hexData[6+i]-'0')*100000+(hexData[7+i]-'0')*10000+(hexData[8+i]-'0')*1000+(hexData[9+i]-'0')*100+(hexData[10+i]-'0')*10+(hexData[11+i]-'0');
        qDebug()<<"反向有功总电能"<<num/100.0;
        i=i+8;
        num=(hexData[4+i]-'0')*1000+(hexData[5+i]-'0')*100+(hexData[6+i]-'0')*10+(hexData[7+i]-'0');
        qDebug()<<"A相电压"<<num/10.0;
        emit this->Setvoltage(num);
        i=i+4;
        num=(hexData[4+i]-'0')*100000+(hexData[5+i]-'0')*10000+(hexData[6+i]-'0')*1000+(hexData[7+i]-'0')*100+(hexData[8+i]-'0')*10+(hexData[9+i]-'0');
        qDebug()<<"A相电流"<<num/1000.0;
        i=i+6;
        num=(hexData[4+i]-'0')*100000+(hexData[5+i]-'0')*10000+(hexData[6+i]-'0')*1000+(hexData[7+i]-'0')*100+(hexData[8+i]-'0')*10+(hexData[9+i]-'0');
        qDebug()<<"瞬时总有功功率"<<num/10000.0;
        i=i+6;
        num=(hexData[4+i]-'0')*100000+(hexData[5+i]-'0')*10000+(hexData[6+i]-'0')*1000+(hexData[7+i]-'0')*100+(hexData[8+i]-'0')*10+(hexData[9+i]-'0');
        qDebug()<<"瞬时总视在功率"<<num/10000.0;
        i=i+6;
        num=(hexData[4+i]-'0')*1000+(hexData[5+i]-'0')*100+(hexData[6+i]-'0')*10+(hexData[7+i]-'0');
        qDebug()<<"总功率因素"<<num/1000.0;
        i=i+4;
        num=(hexData[4+i]-'0')*1000+(hexData[5+i]-'0')*100+(hexData[6+i]-'0')*10+(hexData[7+i]-'0');
        qDebug()<<"相位"<<num/100.0;
        i=i+4;
        num=(hexData[4+i]-'0')*1000+(hexData[5+i]-'0')*100+(hexData[6+i]-'0')*10+(hexData[7+i]-'0');
        qDebug()<<"温度"<<num/10.0;
        i=i+4;
        newmes=1;
    }
    ui->comLog->insertPlainText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + " [recieve] " + msg.toHex() + "\n");
}
void Serial::Sleep(int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}
void Serial::getMeter(){
    QString msg="0400";
    char buf[100];
    for(int i=0;i<6;i++){
        itoa(mailaddress[1][i+7], buf, 16);
        if(mailaddress[1][i+7]<10){
            msg=msg+"0";
        }
        msg=msg+buf;
    }
    qDebug()<<msg;
    Serial::sendMsg(msg.toLatin1());
    Sleep(3000);

}

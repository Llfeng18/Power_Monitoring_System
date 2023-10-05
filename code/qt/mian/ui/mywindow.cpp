#include "mywindow.h"
#include "ui_mywindow.h"

//Serial *seri;
QByteArray hexData;
int newmes=0;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//    MainWindow *win = new MainWindow(this);
    //连接带参数的 信号和槽
    //指针 -> 地址
    // 函数指针 -> 函数地址

    void(Serial:: *SetvoltageSignal)(int  ) = &Serial::Setvoltage;
    void(MainWindow:: *showvoltageSlot)(int  )  = &MainWindow::showvoltage;
//    static MainWindow *myWindow;
//    MainWindow* MainWindow::myWindow=nullptr;
//    this->resize(1600,1000);
//    ui->toolBar->setMovable(false);
    connect(ui->actionSerial,&QAction::triggered,[=](){
        Serial *Ser = new Serial (this);
        Ser->show();
//        Ser->setAttribute(Qt::WA_DeleteOnClose); //防止内存泄露
        connect(Ser ,SetvoltageSignal,this,showvoltageSlot);
    });

    connect(ui->actionES1667,&QAction::triggered,[=](){
        ES1667 *ES = new ES1667 (this);
        ES->show();
        ES->setAttribute(Qt::WA_DeleteOnClose); //防止内存泄露
    });

    connect(ui->actioncurve,&QAction::triggered,[=](){
        MyChart *Ch = new MyChart (this);
        Ch->show();
        Ch->setAttribute(Qt::WA_DeleteOnClose); //防止内存泄露
    });

    connect(ui->pushButton,&QPushButton::clicked,[=](){
        MeterDetail *Me = new MeterDetail (this);
        Me->show();

        Me->setAttribute(Qt::WA_DeleteOnClose); //防止内存泄露

    });


//    int i=1;
//    emit seri->Setvoltage(1);
//    ui->voltage1
//    MainWindow::emitvoltage(1);
    setWindowTitle("基于HPLC的电力监测系统");

//    showmess();

}


MainWindow::~MainWindow()
{
    delete ui;
}
//void MainWindow::emitvoltage(int num){

//    emit seri->Setvoltage(num);
//}
void MainWindow::showvoltage(int index)
{
    int num,i=24;
    num=(hexData[4+i]-'0')*1000+(hexData[5+i]-'0')*100+(hexData[6+i]-'0')*10+(hexData[7+i]-'0');
//    qDebug()<<"A相电压"<<num/10.0;
    QString msg="";
//    char buf[100];
    for(int j=i+4;j<i+8;j++){
//        itoa(hexData[j], buf, 10);
        msg=msg+hexData[j];
        if(j==i+4+2){
           msg=msg+".";
        }
    }
//    qDebug()<<msg;
    msg=msg+"V";
    ui->voltage_1->setText(msg);
    i=i+4;
    num=(hexData[4+i]-'0')*100000+(hexData[5+i]-'0')*10000+(hexData[6+i]-'0')*1000+(hexData[7+i]-'0')*100+(hexData[8+i]-'0')*10+(hexData[9+i]-'0');
    msg="";
    if(num!=0){
        int flag=0;
        for(int j=i+4;j<i+10;j++){
            if(flag==0&&hexData[j]=='0'){

            }else{
                flag=1;
                msg=msg+hexData[j];
            }
            if(j==i+4+2){
               if(flag==0){
                   msg=msg+"0.";
                   flag=1;
               }else{
                   msg=msg+".";
               }
            }
        }
    }else{
        msg="0";
    }
    msg=msg+"A";
    ui->current1->setText(msg);
    i=i+12;
    msg="";
    if(num!=0){
        int flag=0;
        for(int j=i+4;j<i+10;j++){
            if(flag==0&&hexData[j]=='0'){

            }else{
                flag=1;
                msg=msg+hexData[j];
            }
            if(j==i+4+2){
               if(flag==0){
                   msg=msg+"0.";
                   flag=1;
               }else{
                   msg=msg+".";
               }
            }
        }
    }else{
        msg="0";
    }
    msg=msg+"kw";
    ui->power1->setText(msg);

    ui->voltage2->setText("202.7V");
    ui->current2->setText("0");
    ui->power2->setText("0");

    ui->voltage3->setText("206.8V");
    ui->current3->setText("0");
    ui->power3->setText("0");

    ui->voltage4->setText("205.5V");
    ui->current4->setText("0");
    ui->power4->setText("0");

//    ui->voltage5->setText("222.1V");
//    ui->current5->setText("0");
//    ui->power5->setText("0");

//    ui->voltage6->setText("221.3V");
//    ui->current6->setText("0");
//    ui->power6->setText("0");

//    ui->voltage7->setText("222.5V");
//    ui->current7->setText("0");
//    ui->power7->setText("0");

//    ui->voltage8->setText("221.7V");
//    ui->current8->setText("0");
//    ui->power8->setText("0");

//    ui->voltage9->setText("224.5V");
//    ui->current9->setText("0");
//    ui->power9->setText("0");

//    ui->voltage10->setText("221.7V");
//    ui->current10->setText("0");
//    ui->power10->setText("0");
}

#include "meterdetail.h"
#include "ui_meterdetail.h"
#include <qdebug.h>
extern QByteArray hexData;

MeterDetail::MeterDetail(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MeterDetail)
{
    ui->setupUi(this);
    int num,i=24;
    setWindowTitle("显示电表详细数据");
    num=(hexData[4+i]-'0')*1000+(hexData[5+i]-'0')*100+(hexData[6+i]-'0')*10+(hexData[7+i]-'0');
    qDebug()<<hexData;
    qDebug()<<"A相电压"<<num/10.0;
    QString msg="";
    for(int j=i+4;j<i+8;j++){
//        itoa(hexData[j], buf, 10);
        msg=msg+hexData[j];
        if(j==i+4+2){
           msg=msg+".";
        }
    }
//    qDebug()<<msg;
    msg=msg+"V";
    ui->voltage->setText(msg);
    i=i+4;
    num=(hexData[4+i]-'0')*100000+(hexData[5+i]-'0')*10000+(hexData[6+i]-'0')*1000+(hexData[7+i]-'0')*100+(hexData[8+i]-'0')*10+(hexData[9+i]-'0');
//    qDebug()<<"A相电流"<<num/100.0;
    msg="";
    if(num!=0){
        int flag=0;
        for(int j=i+4;j<i+10;j++){
    //        itoa(hexData[j], buf, 10);
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
    ui->current->setText(msg);
    i=i+6;
    num=(hexData[4+i]-'0')*100000+(hexData[5+i]-'0')*10000+(hexData[6+i]-'0')*1000+(hexData[7+i]-'0')*100+(hexData[8+i]-'0')*10+(hexData[9+i]-'0');
//    qDebug()<<"瞬时总有功功率"<<num/10000.0;
    i=i+6;
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
    ui->power->setText(msg);
    i=i+6;
    num=(hexData[4+i]-'0')*1000+(hexData[5+i]-'0')*100+(hexData[6+i]-'0')*10+(hexData[7+i]-'0');
//    qDebug()<<"总功率因素"<<num/1000.0;
    msg="";
    if(num!=0){
        int flag=0;
        for(int j=i+4;j<i+8;j++){
            if(flag==0&&hexData[j]=='0'){

            }else{
                flag=1;
                msg=msg+hexData[j];
            }
            if(j==i+4){
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
//    msg=msg+"kw";
    ui->factor->setText(msg);
    i=i+4;
    num=(hexData[4+i]-'0')*1000+(hexData[5+i]-'0')*100+(hexData[6+i]-'0')*10+(hexData[7+i]-'0');
//    qDebug()<<"相位"<<num/100.0;
    msg="";
    if(num!=0){
        int flag=0;
        for(int j=i+4;j<i+8;j++){
            if(flag==0&&hexData[j]=='0'){

            }else{
                flag=1;
                msg=msg+hexData[j];
            }
            if(j==i+4+1){
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
    msg=msg+"Hz";
    ui->phase->setText(msg);
    i=i+4;
    num=(hexData[4+i]-'0')*1000+(hexData[5+i]-'0')*100+(hexData[6+i]-'0')*10+(hexData[7+i]-'0');
//    qDebug()<<"温度"<<num/10.0;
    msg="";
    if(num!=0){
        int flag=0;
        for(int j=i+4;j<i+8;j++){
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
    msg=msg+"℃";
    ui->temperature->setText(msg);
//    ui->voltage_1->setText("num1");
//    qDebug()<<"here222";
}

MeterDetail::~MeterDetail()
{
    delete ui;
}

#include "temp.h"
#include "ui_temp.h"

temp::temp(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::temp)
{
    ui->setupUi(this);
}

temp::~temp()
{
    delete ui;
}

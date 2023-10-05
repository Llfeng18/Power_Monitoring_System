#include "es1667.h"
#include "ui_es1667.h"

ES1667::ES1667(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ES1667)
{
    ui->setupUi(this);
    setWindowTitle("ES1667参数设置");
}

ES1667::~ES1667()
{
    delete ui;
}

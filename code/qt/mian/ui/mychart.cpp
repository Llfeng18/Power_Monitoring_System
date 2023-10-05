#include "mychart.h"
#include "ui_mychart.h"

MyChart::MyChart(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MyChart)
{
    ui->setupUi(this);

    QLineSeries *lineSeries = new QLineSeries();        //创建折线系列

    QChartView *chartView = new QChartView(this);


    /*为系列里添加100个数据*/

    for(quint8 i=0; i<100; i++){

        lineSeries->append(i,qrand()%10+215);

    }


    lineSeries->setName("系列一");                  //设置系列名称

    mChart = new QChart();                          //创建图标对象


    mChart->addSeries(lineSeries);                  //向图表中添加系列


    mChart->createDefaultAxes();                       //创建默认的坐标，必须在addSeries之后调用

    mChart->setTheme(QChart::ChartThemeDark);           //设置图标主题


    mChart->setTitle("电压");                       //设置图标名称

    //创建坐标
    QValueAxis *axisX = new QValueAxis;//X轴
    axisX->setRange(0, 24);//设置坐标轴范围
    axisX->setTitleText("time(h)");//标题
//    axisX->setLabelFormat("%.1f"); //标签格式：每个单位保留几位小数
//    axisX->setTickCount(20); //主分隔个数：0到10分成20个单位
//    axisX->setMinorTickCount(4); //每个单位之间绘制了多少虚网线
    //    axisX->setGridLineVisible(false);

    QValueAxis *axisY = new QValueAxis; //Y 轴
    axisY->setRange(215, 225);
    axisY->setTitleText("voltage(V)");
//    axisY->setLabelFormat("%.2f"); //标签格式
//    axisY->setTickCount(10);
//    axisY->setMinorTickCount(4);
    //    axisX->setGridLineVisible(false);
    //为序列设置坐标轴
    mChart->setAxisX(axisX);
    mChart->setAxisY(axisY);



    mChart->legend()->hide();                          //隐藏图例，也就是不显示系列名称了


    chartView->setChart(mChart);                    //向chartView中添加图标


    chartView->resize(QSize(800,500));              //重新设置chartView的大小


    chartView->setRenderHints(QPainter::Antialiasing);//消除边缘，看起来平滑一些


    //ui->actionhorizontalLayout->addWidget(chartView);     //把chartView放到水平布局中（在ui中拖一个水平布局）


    ui->lineEdit->setText("76:30:10:00:12:19");
    ui->lineEdit_2->setText("2022:3:13");
    setWindowTitle("历史曲线查询");
}

MyChart::~MyChart()
{
    delete ui;
}

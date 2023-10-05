#ifndef MYCHART_H
#define MYCHART_H

#include <QMainWindow>

/*添加以下三个头文件*/

#include <QtCharts>

#include <QChartView>

#include <QSplineSeries>

namespace Ui {
class MyChart;
}

class MyChart : public QMainWindow
{
    Q_OBJECT

public:
    explicit MyChart(QWidget *parent = nullptr);
    ~MyChart();

private:
    Ui::MyChart *ui;
    QChart *mChart;    //创建图表对象
};

#endif // MYCHART_H

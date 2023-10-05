#ifndef METERDETAIL_H
#define METERDETAIL_H

#include <QMainWindow>

namespace Ui {
class MeterDetail;
}

class MeterDetail : public QMainWindow
{
    Q_OBJECT

public:
    explicit MeterDetail(QWidget *parent = nullptr);
    ~MeterDetail();
    Ui::MeterDetail *ui;

private:

};

#endif // METERDETAIL_H

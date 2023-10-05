#ifndef ES1667_H
#define ES1667_H

#include <QMainWindow>

namespace Ui {
class ES1667;
}

class ES1667 : public QMainWindow
{
    Q_OBJECT

public:
    explicit ES1667(QWidget *parent = nullptr);
    ~ES1667();

private:
    Ui::ES1667 *ui;
};

#endif // ES1667_H

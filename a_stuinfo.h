#ifndef A_STUINFO_H
#define A_STUINFO_H

#include <QWidget>

namespace Ui {
class A_StuInfo;
}

class A_StuInfo : public QWidget
{
    Q_OBJECT

public:
    explicit A_StuInfo(QWidget *parent = 0);
    ~A_StuInfo();

private:
    Ui::A_StuInfo *ui;
};

#endif // A_STUINFO_H

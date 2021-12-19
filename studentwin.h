#ifndef STUDENTWIN_H
#define STUDENTWIN_H

#include <QWidget>
#include "stugradequery.h"

namespace Ui {
class StudentWin;
}

class StudentWin : public QWidget
{
    Q_OBJECT

public:
    explicit StudentWin(QString id,QWidget *parent = 0);
    ~StudentWin();

private:
    Ui::StudentWin *ui;
    QString id;
    stuGradeQuery *grade;
};

#endif // STUDENTWIN_H

#ifndef TEACHERWIN_H
#define TEACHERWIN_H

#include <QWidget>

namespace Ui {
class TeacherWin;
}

class TeacherWin : public QWidget
{
    Q_OBJECT

public:
    explicit TeacherWin(QString id,QWidget *parent = 0);
    ~TeacherWin();
private:
    Ui::TeacherWin *ui;
    QString id;
};

#endif // TEACHERWIN_H

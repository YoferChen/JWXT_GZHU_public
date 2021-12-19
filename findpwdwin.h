#ifndef FINDPWDWIN_H
#define FINDPWDWIN_H

#include <QWidget>
#include "smtp.h"

namespace Ui {
class FindPwdWin;
}

class FindPwdWin : public QWidget
{
    Q_OBJECT

public:
    explicit FindPwdWin(QWidget *parent = 0);
    ~FindPwdWin();

private:
    Ui::FindPwdWin *ui;
    QString sendVerfCode;
    int identity=0;
};

#endif // FINDPWDWIN_H

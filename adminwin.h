#ifndef ADMINWIN_H
#define ADMINWIN_H

#include <QWidget>
#include "a_stuinfo.h"
#include "modifypwdwin.h"

namespace Ui {
class AdminWin;
}

class AdminWin : public QWidget
{
    Q_OBJECT

public:
    explicit AdminWin(QString id,QWidget *parent = 0);
    ~AdminWin();

private:
    Ui::AdminWin *ui;
    A_StuInfo* stuInfoWin;
    ModifyPwdWin* modifyPwdWin;
    QString id;
    QString pwd;

};

#endif // ADMINWIN_H

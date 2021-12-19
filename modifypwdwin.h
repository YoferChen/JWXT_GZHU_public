#ifndef MODIFYPWDWIN_H
#define MODIFYPWDWIN_H

#include <QWidget>

namespace Ui {
class ModifyPwdWin;
}

class ModifyPwdWin : public QWidget
{
    Q_OBJECT

public:
    explicit ModifyPwdWin(QWidget *parent = 0);
    ~ModifyPwdWin();
    void getIdentity(int identity);
    void getId(QString id);
    void setId();

private:
    Ui::ModifyPwdWin *ui;
    int identity;
    QString id;
};

#endif // MODIFYPWDWIN_H

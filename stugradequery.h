#ifndef STUGRADEQUERY_H
#define STUGRADEQUERY_H

#include <QWidget>

namespace Ui {
class stuGradeQuery;
}

class stuGradeQuery : public QWidget
{
    Q_OBJECT

public:
    explicit stuGradeQuery(int indentity,QString id,QWidget *parent = 0);
    ~stuGradeQuery();

private:
    Ui::stuGradeQuery *ui;
    int identity;
    QString id;
    void getStuGrades(QString CPID);
};

#endif // STUGRADEQUERY_H

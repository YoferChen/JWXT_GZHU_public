#ifndef COURSE_SELECTION_H
#define COURSE_SELECTION_H

#include <QWidget>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QSqlError>
#include <QDebug>
#include "sqltools.h"
#include <QTreeWidgetItem>
#include <QMessageBox>

namespace Ui {
class Course_Selection;
}

class Course_Selection : public QWidget
{
    Q_OBJECT

public:
    explicit Course_Selection(QWidget *parent = 0);
    explicit Course_Selection(QString id,QWidget *parent = 0);
    ~Course_Selection();
    void getID(QString id);
    void welcome();
private:
    Ui::Course_Selection *ui;
    QString userId;
    QSqlTableModel* model;
    SqlTools* sqlTool;
    QTreeWidgetItem* currentItem;
public slots:
    void getCourses(int courseType);
    void selectCourse();
    void cancelSelectCourse();
    void showCourseTeacherInfo();
};

#endif // COURSE_SELECTION_H

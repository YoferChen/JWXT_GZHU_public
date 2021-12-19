#ifndef COURSETABLE_H
#define COURSETABLE_H

#include <QWidget>
#include <QStandardItemModel>
#include <QVBoxLayout>
#include "sqltools.h"
#include "autoschedulealg.h"

namespace Ui {
class CourseTable;
}

class CourseTable : public QWidget
{
    Q_OBJECT

public:
    explicit CourseTable(int identity,QString id,QWidget *parent = 0);
    ~CourseTable();

private:
    Ui::CourseTable *ui;
    int identity;
    QString id;
    SqlTools * sqlTool;
    //查询数据库获取课表信息
public slots:
    void getCourseToTable();
    void getCourse();
    void addCourse();
    void removeCourse();
    void getClassCourse();
    void getCoursepoolList();

    void addCourseForClass();  //为班级添加课程【修改coursePool表中的开课班级字段】
    void removeCourseForClass();   //为班级移出课程【修改coursePool表中的开课班级字段】

    void autoScheduleCourse();  //智能排课

    QString getDay(int column);  //数字转周数，如：1->星期一
    QString getSection(int section);  //第几大节转小节，如：1->1-2节
};

#endif // COURSETABLE_H

#ifndef SQLTOOLS_H
#define SQLTOOLS_H
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QSqlError>
#include <QVariant>
#include <QList>
#include <QSqlQueryModel>
#include <QTableView>
#include <QVBoxLayout>
#include <QSqlQuery>
#include <QHeaderView>


class SqlTools
{
public:
    SqlTools();
    //根据身份和id、需要的字段名获取对应字段
    QVariant getPersonalInfo(int identity,QString id,QString infoNeed);
    //查询course表获取所有课程id   //type：查询课程类别：0：all，1:必修，2：选修
    QVariantList getCourseIds(int type);
    //查询course表获取课程的基本信息
    QSqlRecord getCourseInfo(QString id);
    //查询coursePool表，获取开课信息
    QList<QSqlRecord> getCourseOpenInfo(QString courID);
    //查询sc表，获取某门课的选课学生人数
    int getCourseSelectCount(QString CPID);
    //查询sc表，判断一门课程是否有选择
    bool isSelectCourse(QString stuID,QString CPID);
    //查询sc表，获取学生个人选课情况
    QList<QSqlRecord> getStuSelectCourses(QString stuID);
    //查询coursePool表，获取教师选课情况
    QList<QSqlRecord> getTeaSelectCourses(QString teaID);
    //向sc表添加选课记录
    bool addCourseRecord(QString stuID,QString CPID);
    //从sc表中删除选课记录
    bool removeCourseRecord(QString stuID,QString CPID);
    //查询coursepool表获取信息
    QSqlRecord coursePoolInfo(QString CPID);
    //弹窗显示指定教师信息
    void showTeacherInfo(QString TeaName);
    //根据查询条件获取课程信息
    QList<QSqlRecord> getCourses(QString filter,QString value);
    //获取指定表某个字段的值
    QList<QVariant> getTableColumnList(QString tableName,QString fieldName);
    //根据教师姓名获取教师id
    QString getPersonalIDbyName(int identity,QString name);
    //从coursepool删除课程
    bool removeCourseFromCoursepool(QString CPID);
    //根据条件获取表中满足条件的所有记录
    QList<QSqlRecord> getFilterRecords(QString tableName,QString filter,QString value);







private:
    QSqlTableModel* model;

};

#endif // SQLTOOLS_H

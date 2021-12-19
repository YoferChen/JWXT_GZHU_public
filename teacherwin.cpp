#include "teacherwin.h"
#include "ui_teacherwin.h"
#include <QDebug>
#include "modifypwdwin.h"
#include "coursetable.h"
#include "studentinfomanage.h"
#include "stugradequery.h"

TeacherWin::TeacherWin(QString id, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TeacherWin)
{
    ui->setupUi(this);

    this->id=id;
    ui->welcome->setText("用户："+this->id+" 欢迎您！");

    ui->teacherInfoQuery->hide();
    ui->courseInfoQuery->hide();
    QSqlTableModel model;
    model.setTable("teacher");
    model.setFilter(QObject::tr("TeaID='%1'").arg(this->id));
    model.select();
    if(model.record(0).value("IsDean").toInt()==0)
        ui->studentInfoQuery->hide();

    //修改密码
    connect(ui->modifyPwd,&QToolButton::clicked,this,[=]{
        ModifyPwdWin* modifyPwdWin=new ModifyPwdWin;
        modifyPwdWin->getIdentity(1);
        modifyPwdWin->getId(this->id);
        modifyPwdWin->setId();
        modifyPwdWin->show();
    });

    //查询课表
    connect(ui->scheduleQuery,&QPushButton::clicked,this,[=](){
        CourseTable *courseTable=new CourseTable(1,this->id);
        courseTable->show();
    });

    //学生信息查询
    connect(ui->studentInfoQuery,&QPushButton::clicked,this,[=](){
        studentInfoManage *stuInfo=new studentInfoManage();
        stuInfo->setTitle("学生基本信息查询");
        stuInfo->hideModifyTools();
        stuInfo->show();
    });

    //课程成绩录入
    connect(ui->courseGradeManage,&QToolButton::clicked,this,[=]()
    {
        stuGradeQuery* grade=new stuGradeQuery(1,this->id);
        grade->show();
    });

}

TeacherWin::~TeacherWin()
{
    delete ui;
}

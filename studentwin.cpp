#include "studentwin.h"
#include "ui_studentwin.h"
#include <QDebug>
#include "modifypwdwin.h"
#include "course_selection.h"
#include "coursetable.h"
#include "stugradequery.h"


StudentWin::StudentWin(QString id, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StudentWin)
{
    ui->setupUi(this);

    this->id=id;
    ui->welcome->setText("用户："+this->id+" 欢迎您！");

    //修改密码
    connect(ui->modifyPwd,&QToolButton::clicked,this,[=]{
        ModifyPwdWin* modifyPwdWin=new ModifyPwdWin;
        modifyPwdWin->getIdentity(2);
        modifyPwdWin->getId(this->id);
        modifyPwdWin->setId();
        modifyPwdWin->show();
    });
    //选课系统
    connect(ui->courseSelect,&QToolButton::clicked,this,[=](){
        Course_Selection* courseSelection=new Course_Selection(this->id);
        courseSelection->show();
        courseSelection->getID(this->id);
        courseSelection->welcome();
    });

    //查询课表
    connect(ui->scheduleQuery,&QToolButton::clicked,this,[=]()
    {
        CourseTable *table=new CourseTable(2,this->id);
        table->show();
    });

    //成绩查询
    connect(ui->gradeQuery,&QToolButton::clicked,this,[=](){
        grade=new stuGradeQuery(2,this->id);
        grade->show();
    });

}


StudentWin::~StudentWin()
{
    delete ui;
}

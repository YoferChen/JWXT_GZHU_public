#include "adminwin.h"
#include "ui_adminwin.h"
#include "studentinfomanage.h"
#include "coursetable.h"

AdminWin::AdminWin(QString id, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AdminWin)
{
    ui->setupUi(this);

    this->id=id;
    ui->teacherInfo->hide();
    //修改密码
    connect(ui->modifyPwd,&QToolButton::clicked,this,[=]{
        ModifyPwdWin* modifyPwdWin=new ModifyPwdWin;
        modifyPwdWin->getIdentity(0);
        modifyPwdWin->getId(this->id);
        modifyPwdWin->setId();
        modifyPwdWin->show();
    });

    //学生信息维护
    connect(ui->stuInfo,&QToolButton::clicked,this,[=](){
        studentInfoManage* stuInfoMan=new studentInfoManage();
        stuInfoMan->setAttribute(Qt::WA_DeleteOnClose);
        stuInfoMan->show();

    });

    //课程信息维护
    connect(ui->courseInfo,&QToolButton::clicked,this,[=](){
        CourseTable* courseManage=new CourseTable(0,this->id);
        courseManage->show();
        courseManage->setAttribute(Qt::WA_DeleteOnClose);
    });
}

AdminWin::~AdminWin()
{
    delete ui;
}


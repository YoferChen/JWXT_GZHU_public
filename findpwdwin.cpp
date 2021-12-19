#include "findpwdwin.h"
#include "ui_findpwdwin.h"
#include "random"
#include <QMessageBox>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <ctime>

FindPwdWin::FindPwdWin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FindPwdWin)
{
    ui->setupUi(this);

    //设置随机种子
    srand((int)time(NULL));

    ui->email->setText("1732562137@qq.com");

    //点击发送验证码之前不可确认修改
    ui->sureModify->setEnabled(false);

    //获取用户身份
    connect(ui->admin,&QRadioButton::clicked,this,[=](){
        this->identity=0;
        ui->userId->setText("admin");
    });

    connect(ui->teacher,&QRadioButton::clicked,this,[=](){
        this->identity=1;
        ui->userId->setText("20000");
    });

    connect(ui->student,&QRadioButton::clicked,this,[=](){
        this->identity=2;
        ui->userId->setText("1806100000");
    });

    ui->admin->click();
    ui->userId->setText("admin");



    //发送验证码
    connect(ui->sendVerfCode,&QToolButton::clicked,this,[=](){

        //获取editline的内容
        QString targetMail=ui->email->text();
        qDebug()<<"验证邮箱："<<targetMail;
        ui->sureModify->setEnabled(true);

        //判断邮箱是否与该账户预留的邮箱相匹配
        QString tableName;
        int emailIndex;
        QString FilterID;
        if(identity==0)
        {
            tableName="admin";
            emailIndex=2;
            FilterID="AdminID";
        }
        else if(identity==1)
        {
            tableName="teacher";
            emailIndex=5;
            FilterID="TeaID";
        }
        else
        {
            tableName="student";
            emailIndex=6;
            FilterID="StuID";
        }

        QSqlTableModel model;
        model.setTable(tableName);
        QString id=ui->userId->text();
        model.select();
        model.setFilter(FilterID+"='"+id+"'");

        int rowCount=model.rowCount();
        if(rowCount==0)
        {
            qDebug()<<"用户不存在！";
            QMessageBox::warning(this,"用户不存在","用户不存在，请重新输入用户id");
            return;
        }
        else
        {
            QSqlRecord record=model.record(0);
            if(record.value(emailIndex).toString()!=targetMail)
            {
                qDebug()<<"此邮箱与用户预留的邮箱不符！\n请重新输入，或者联系管理员修改！";
                QMessageBox::warning(this,"邮箱不匹配","此邮箱与用户预留的邮箱不符！\n请重新输入，或者联系管理员修改！");
                return;
            }
        }
        //生成随机验证码，发送邮件
        int randCode=rand()%899999+100000;
        sendVerfCode=QString::number(randCode);
        qDebug()<<"验证码："<<sendVerfCode;
        smtp smtp_test("发送邮件的email","发送邮箱的email的设置中启动stmp得到的token"); //邮箱和密码都要用自己的
        QString content="广州大学教务系统：\n\n验证码:"+sendVerfCode+"\n\n五分钟内有效！\n"+"---------找回密码---------\n";
        qDebug()<<"发送邮件："<<content;
        //发送邮件
        smtp_test.send(targetMail.toLatin1(),"GZHU:找回密码",content);
        QMessageBox::information(this,"验证码发送成功","验证码发送到账户预留的邮箱！");
    });

    //确认修改密码
    connect(ui->sureModify,&QToolButton::clicked,this,[=](){
        //获取输入信息
        QString id=ui->userId->text();
        QString email=ui->email->text();
        QString verfCode=ui->verfcode->text();
        QString pwd_new=ui->newPwd->text();
        QString pwd_new_sure=ui->newPwdSure->text();
        if(id.isEmpty()||email.isEmpty()||verfCode.isEmpty()||pwd_new.isEmpty()||pwd_new_sure.isEmpty())
        {
            qDebug()<<"请补全信息！";
            QMessageBox::warning(this,"信息不完整","请补全信息！");
            return;
        }
        if(pwd_new!=pwd_new_sure)
        {
            qDebug()<<"密码不匹配！";
            QMessageBox::warning(this,"密码不匹配","两次输入的密码不相同，请重新输入密码！");
            return;
        }
        if(sendVerfCode!=verfCode)
        {
            qDebug()<<"验证码错误！";
            QMessageBox::warning(this,"验证码错误","验证码输入有误，请重新输入！");
            return;
        }


        //修改密码
        QString tableName;
        int PwdIndex;
        QString FilterID;
        if(identity==0)
        {
            tableName="admin";
            PwdIndex=1;
            FilterID="AdminID";
        }
        else if(identity==1)
        {
            tableName="teacher";
            PwdIndex=4;
            FilterID="TeaID";
        }
        else
        {
            tableName="student";
            PwdIndex=5;
            FilterID="StuID";
        }

        QSqlTableModel model;
        model.setTable(tableName);
        model.select();
        model.setFilter(FilterID+"='"+id+"'");
//        model.record(0).setValue(PwdIndex,pwd_new);
        qDebug()<<"准备修改密码：\n"<<"查询到的记录数量："<<model.rowCount();
        model.setData(model.index(0,PwdIndex),pwd_new);
        model.submitAll();
        QMessageBox::information(this,"密码修改成功","新密码已生效！");
        this->close();
    });

    //返回登录
    connect(ui->back,&QToolButton::clicked,this,&FindPwdWin::close);
}

FindPwdWin::~FindPwdWin()
{
    delete ui;
}

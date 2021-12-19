#include "modifypwdwin.h"
#include "ui_modifypwdwin.h"
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QMessageBox>
#include <QDebug>

ModifyPwdWin::ModifyPwdWin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ModifyPwdWin)
{
    ui->setupUi(this);

    connect(ui->modify_sure,&QPushButton::clicked,this,[=](){
        //修改密码
        //获取新密码
        qDebug()<<"确认修改密码！";
        QString pwd_new=ui->pwd_new->text();
        QString pwd_new_sure=ui->pwd_new_sure->text();
        if (pwd_new!=pwd_new_sure){
            QMessageBox::warning(this,"密码不匹配","输入的两次密码不相同，请重新输入！");
            ui->pwd_new_sure->clear();
            ui->pwd_new->clear();
            return;
        }
        if (pwd_new.isEmpty()||pwd_new_sure.isEmpty()){
            QMessageBox::warning(this,"密码为空","密码为空，请输入密码！");
            return;
        }
        //修改密码
        QSqlTableModel model;
        if(identity==0){  //管理员
            model.setTable("admin");
            model.select();
            int ret=model.rowCount();
            for(int i=0;i<ret;++i)
            {
                QSqlRecord record=model.record(i);
                if(record.value(0)==this->id){
                    //修改密码
                    model.setData(model.index(i,1),pwd_new);
                    model.submitAll();
                    QMessageBox::information(this,"密码修改成功","修改密码成功！");
                    this->close();
                    break;
                }
            }
        }
        else if(identity==1)  //教师
        {
            model.setTable("teacher");
            model.select();
            int ret=model.rowCount();
            for(int i=0;i<ret;++i)
            {
                QSqlRecord record=model.record(i);
                if(record.value(0)==this->id){
                    //修改密码
                    model.setData(model.index(i,4),pwd_new);
                    model.submitAll();
                    QMessageBox::information(this,"密码修改成功","修改密码成功！");
                    this->close();
                    break;
                }
            }
        }
        else if (identity==2)   //学生
        {
            model.setTable("student");
            model.select();
            int ret=model.rowCount();
            for(int i=0;i<ret;++i)
            {
                QSqlRecord record=model.record(i);
                if(record.value(0)==this->id){
                    //修改密码
                    model.setData(model.index(i,5),pwd_new);
                    model.submitAll();
                    QMessageBox::information(this,"密码修改成功","修改密码成功！");
                    this->close();
                    break;
                }
            }
        }

    });
}

ModifyPwdWin::~ModifyPwdWin()
{
    delete ui;
}

void ModifyPwdWin::getIdentity(int identity)
{
    this->identity=identity;
}

void ModifyPwdWin::getId(QString id)
{
    this->id=id;
}

void ModifyPwdWin::setId()
{
    ui->id->setText(this->id);
    ui->id->setEnabled(false);
}

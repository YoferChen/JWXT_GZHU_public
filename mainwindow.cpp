#define CONNECTION_H
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlError>
#include <QDebug>
#include <QSqlQuery>
#include <QVector>
#include <QSqlResult>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QSqlTableModel>
#include <QMessageBox>

//QSqlDatabase connectDB();

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //连接mysql
//    说明：pro文件增加QT       += sql
//    复制mysql/lib下的libmysql.dll和libmysql.lib到Qt的bin目录下，由于安装的mysql是64位的，只能用64位的Qt编译器
    QSqlDatabase db=QSqlDatabase::addDatabase("QMYSQL");  //ORACL的话是QODBC，需要手动编译
    db.setHostName("localhost");
    db.setUserName("root");
//    db.setPassword("admin");
    db.setPassword("123456");
    db.setDatabaseName("jwxt_gzhu");
    bool bRet=db.open();
    if(bRet==false)
    {
        qDebug()<<"error open database"<<db.lastError().text();
        QMessageBox::warning(this,"数据库连接失败","数据库无法连接，请联系管理员处理！");
        exit(0);
    }
    qDebug()<<"open database success";
//    QSqlQuery query= db.exec("select * from  blade_blog");
//    int size=query.size();
//    bool b=query.first();

    //获取身份选择
    connect(ui->admin,&QRadioButton::clicked,this,[=](){
        indentity=0;
        qDebug()<<"身份选择：管理员 ";  //最后中文与引号要加一个空格，否则会报错
        //测试
        ui->id_input->setText("admin");
        ui->pwd_input->setText("admin");
    });
    connect(ui->teacher,&QRadioButton::clicked,this,[=](){
        indentity=1;
        qDebug()<<"身份选择：教师 ";
        //测试
        ui->id_input->setText("20002");
        ui->pwd_input->setText("123456");
    });
    connect(ui->student,&QRadioButton::clicked,this,[=](){
        indentity=2;
        qDebug()<<"身份选择：学生 ";
        //测试
        ui->id_input->setText("1806100125");
        ui->pwd_input->setText("123456");
    });


    //登录按钮
    connect(ui->login,&QPushButton::clicked,this,[=](){
        if(indentity==0){
            qDebug()<<"管理员登录 ";

            //获取输入的账号密码
            QString id = ui->id_input->text();
            QString pwd=ui->pwd_input->text();
            qDebug()<<"获取到的账号："<<id<<"密码："<<pwd;

            //查询adminb表
            QSqlTableModel model;
            model.setTable("admin");
            model.select();
            int rowCount=model.rowCount();
            bool loginSuccess=false;
            for(int i=0;i<rowCount;++i)
            {
                QSqlRecord record=model.record(i);
                if(record.value(0)==id&&record.value(1)==pwd)
                {
                    qDebug()<<"账号密码与数据库匹配成功！";
                    adminWin=new AdminWin(id);
        //            this->setVisible(false);
                    this->hide();
                    adminWin->show();
                    loginSuccess=true;
                    break;
                }
            }
            if (loginSuccess==false)
                QMessageBox::warning(this,"登录失败","账号或密码错误！请重新输入！");

        }
        else if(indentity==1){
            qDebug()<<"教师登录 ";
            
            //获取输入的账号密码
            QString id = ui->id_input->text();
            QString pwd=ui->pwd_input->text();
            qDebug()<<"获取到的账号："<<id<<"密码："<<pwd;

            //查询teacher表
            QSqlTableModel model;
            model.setTable("teacher");

            model.select();
            int rowCount=model.rowCount();
            bool loginSuccess=false;
            for(int i=0;i<rowCount;++i)
            {
                QSqlRecord record=model.record(i);
                
                if(record.value(0)==id&&record.value(4)==pwd)
                {
                    qDebug()<<"账号密码与数据库匹配成功！";
                    teacherWin=new TeacherWin(id);
                    this->hide();
                    teacherWin->show();
                    loginSuccess=true;
                    break;
                }
            }
            if (loginSuccess==false)
                QMessageBox::warning(this,"登录失败","账号或密码错误！请重新输入！");
        }
        else{
            qDebug()<<"学生登录 ";

            //获取输入的账号密码
            QString id = ui->id_input->text();
            QString pwd=ui->pwd_input->text();
            qDebug()<<"获取到的账号："<<id<<"密码："<<pwd;

            //查询teacher表
            QSqlTableModel model;
            model.setTable("student");

            model.select();
            int rowCount=model.rowCount();
            bool loginSuccess=false;
            for(int i=0;i<rowCount;++i)
            {
                QSqlRecord record=model.record(i);

                if(record.value(0)==id&&record.value(5)==pwd)
                {
                    qDebug()<<"账号密码与数据库匹配成功！";
                    studentWin=new StudentWin(id);
                    this->hide();
                    studentWin->show();
                    loginSuccess=true;
                    break;
                }
            }
            if (loginSuccess==false)
                QMessageBox::warning(this,"登录失败","账号或密码错误！请重新输入！");

        }
    });
//    找回密码
    connect(ui->findPwd,&QPushButton::clicked,this,[=](){
        qDebug()<<"找回密码";
        findPwdWin=new FindPwdWin();
        findPwdWin->show();
    });


    //设置默认选项
    ui->admin->click();
//    ui->student->click();
}

MainWindow::~MainWindow()
{
    delete ui;


}


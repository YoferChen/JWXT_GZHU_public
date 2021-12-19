#include "stugradequery.h"
#include "ui_stugradequery.h"
#include <QSqlTableModel>
#include <ctime>
#include <random>
#include <QSqlRecord>
#include <QDebug>
#include <QList>
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QSqlError>


stuGradeQuery::stuGradeQuery(int indentity, QString id, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::stuGradeQuery)
{
        ui->setupUi(this);
        this->id=id;
        this->identity=indentity;


        if(identity==2)
        {

            ui->courTypeWidget->hide();
            ui->gradeModify->hide();

            //查询选课数量
            QSqlTableModel model;
            model.setTable("sc");
            model.setFilter(QObject::tr("StuID='%1'").arg(id));
            model.select();

            ui->gradeTable->setColumnCount(5);
            ui->gradeTable->setRowCount(model.rowCount());
            for(int i=0;i<5;++i)
            {
                ui->gradeTable->setColumnWidth(i,168);
            }


            ui->gradeTable->setHorizontalHeaderLabels(QStringList() <<"课程号"<< "课程名" << "课程类型" << "授课教师" << "成绩");

            //根据CPID查询CPID信息和Course信息
            QStringList CPID_List;
            QList<int> gradeList;
            QStringList teaList;
            QStringList courNameList;
            QStringList courTypeList;
            QList<float> creditList;
            int row=model.rowCount();
            for(int i=0;i<row;++i)
            {
                CPID_List.append(model.record(i).value("CPID").toString());
                gradeList.append(model.record(i).value("StuGrade").toInt());
            }
            //查询课程详细信息
            for(int i=0;i<CPID_List.count();++i)
            {
                model.setTable("coursepool");
                model.setFilter(QObject::tr("CPID='%1'").arg(CPID_List[i]));
                model.select();
                QString CourID=model.record(0).value("CourID").toString();
                QString TeaID=model.record(0).value("TeaID").toString();
                model.setTable("teacher");
                model.setFilter(QObject::tr("TeaID='%1'").arg(TeaID));
                model.select();
                QString TeaName=model.record(0).value("TeaName").toString();
                teaList.append(TeaName);
                model.setTable("course");
                model.setFilter(QObject::tr("CourID='%1'").arg(CourID));
                model.select();
                courNameList.append(model.record(0).value("CourName").toString());
                courTypeList.append(model.record(0).value("CourType").toString());
                creditList.append(model.record(0).value("Credit").toFloat());
            }
            for(int i=0;i<row;++i)
            {
                for(int k=0;k<5;++k)
                    ui->gradeTable->setItem(i,k,new QTableWidgetItem());
                //"课程号"<< "课程名" << "课程类型" << "授课教师" << "成绩"
                ui->gradeTable->item(i,0)->setText(CPID_List[i]);
                ui->gradeTable->item(i,1)->setText(courNameList[i]);
                ui->gradeTable->item(i,2)->setText(courTypeList[i]);
                ui->gradeTable->item(i,3)->setText(teaList[i]);
                ui->gradeTable->item(i,4)->setText(QString::number(gradeList[i]));
            }
            //计算绩点
            float GPA=0;
            float creditSum=0;
            for(int i=0;i<gradeList.count();++i)
            {
                creditSum+=creditList[i];
                if(gradeList[i]>=60)
                {
                    float grade=(gradeList[i]-50)/10;
                    if(grade>4.0)
                    {
                        grade=4.0;
                    }
                    qDebug()<<"grade="<<grade;
                    GPA+=grade*creditList[i];
                }
                else
                {

                }
            }
            qDebug()<<"GPA="<<GPA;

            GPA=GPA/creditSum;
            qDebug()<<"GPA="<<GPA;
            qDebug()<<"总学分："<<creditSum;
            qDebug()<<"成绩列表："<<gradeList;
            qDebug()<<"学分列表："<<creditList;
            qDebug()<<"绩点:"<<GPA;
            connect(ui->gpaQuery,&QPushButton::clicked,this,[=](){
                QMessageBox::information(this,"绩点",QObject::tr("您的绩点：%1 (4.0分制)！").arg(GPA));
            });

            /*
        证    明
          学生XXX（学号：XXX），系广州大学计算机科学与网络工程学院2018级计算机科学与技术专业计科18X班的学生，该生在校学年,平均学分绩点为：XX/(0-4.0)，排名为：XX，该年级专业共有XXX人。
        计算机科学与网络工程学院
        CERTIFICATE
          This is to certify that 如有误请在学生个人信息姓名拼音中填写(student ID: XXX) has studied in class 183 of Computer Science and Technology major in School of Computer Science and Network Engineering of Guangzhou University.So far ,this student's gpa is XXX/(0-4.0) ,ranked XXX among XXX students in the major.
        School of Computer Science and Network Engineering

            */

            //设置成绩，查询绩点
        //    srand(unsigned int(time(NULL)));
        //    connect(ui->gpaQuery,&QPushButton::clicked,this,[=](){
        //        //随机为学生所有课程添加成绩
        //        QSqlTableModel model;
        //        model.setTable("sc");
        //        model.select();
        //        for(int i=0;i<model.rowCount();++i)
        //        {
        //            model.setData(model.index(i,model.record().indexOf("StuGrade")),QString::number((int)rand()%30+71));
        //            model.submit();
        //        }
        //        model.submitAll();
        //        model.database().commit();
        //        qDebug()<<"添加成绩成功！";

        //    });

        }
        else if(identity==1)
        {
            //获取教师授课列表
            this->resize(QSize(1200,800));
            ui->gpaQuery->hide();
            QStringList CPIDList;
            QSqlTableModel model;
            model.setTable("coursepool");
            model.setFilter(QObject::tr("TeaID='%1'").arg(this->id));
            model.select();
            //获取课程名
            //添加课程信息到下拉框
            for(int i=0;i<model.rowCount();++i)
            {
                QString CPID= model.record(i).value("CPID").toString();
//                qDebug()<<"CPID="<<CPID;
                CPIDList.append(CPID);
                QString OpenClass=model.record(i).value("OpenCLass").toString();
//                qDebug()<<"OpenClass="<<OpenClass;
                //获取课程名称
                QString CourID=model.record(i).value("CourID").toString();
//                qDebug()<<"CourID="<<CourID;
               QSqlTableModel model_1;
               model_1.setTable("course");
               model_1.setFilter(QObject::tr("CourID='%1'").arg(CourID));
               model_1.select();
               QString CourName=model_1.record(0).value("CourName").toString();
//               qDebug()<<"CourName="<<CourName;
               QString courseItem=CPID+"/"+CourName+"/"+OpenClass;
//               qDebug()<<"courseItem"<<courseItem;
               ui->courList->addItem(courseItem);
            }
            //添加信息到表格
            ui->gradeTable->setColumnCount(4);
            ui->gradeTable->setRowCount(model.rowCount());
            for(int i=0;i<4;++i)
            {
                ui->gradeTable->setColumnWidth(i,168);
            }
            ui->gradeTable->setHorizontalHeaderLabels(QStringList() <<"学号"<< "姓名" << "班级" << "成绩" );
            QString currentCPID=ui->courList->currentText().split(QRegExp("/"))[0];
//            qDebug()<<"currentCPID="<<currentCPID;  //
            this->getStuGrades(currentCPID);
        }

        //课程选择改变
        void (QComboBox:: *signal)(const QString&) = &QComboBox::currentIndexChanged;
        connect(ui->courList,signal,this,[=](){
            this->getStuGrades(ui->courList->currentText().split(QRegExp("/"))[0]);
        });

        //课程成绩修改提交
        connect(ui->gradeModify,&QToolButton::clicked,this,[=](){
            //获取每个学生的成绩
            QString CPID=ui->courList->currentText().split(QRegExp("/"))[0];
            qDebug()<<ui->gradeTable->rowCount()<<"行数据";


            QSqlTableModel model;
            model.setTable("sc");
//            for(int i=0;i<ui->gradeTable->rowCount();++i)
//            {
//                qDebug()<<"获取到的成绩："<<ui->gradeTable->item(i,3)->text();
//            }
//            return;

            for(int i=0;i<ui->gradeTable->rowCount();++i)
            {
                QString grade=ui->gradeTable->item(i,3)->text();
                qDebug()<<"grade="<<grade;
                QString StuID=ui->gradeTable->item(i,0)->text();
                model.setFilter(QObject::tr("StuID='%1' and CPID='%2'").arg(StuID).arg(CPID));
                model.select();
                qDebug()<<"查询到"<<model.rowCount()<<"条选课记录:"<<CPID<<StuID<<model.record(0).value("StuGrade")<<"->"<<grade;
                //提交事务
//                qDebug()<<"成绩修改为："<<grade;
//                qDebug()<<model.lastError();
//                model.setData(model.index(0,2),grade.toInt());
                model.database().transaction();
                QSqlRecord record=model.record(0);
                record.setValue("StuGrade",grade.toInt());
                model.setRecord(0,record);
                if(model.submit())
                {
                    qDebug()<<"修改成功！";
                    model.database().commit();
                }
                else
                {
                    qDebug()<<"修改失败！" ;
                    return;
                }
            }
            QMessageBox::information(this,"录入成绩","录入成功");
//            if(model.submitAll())
//            {
//                qDebug()<<"事务提交成功！";
//                model.database().commit();
//            }
//            else
//            {
//                qDebug()<<"成绩修改失败";
//                model.database().rollback();
//            }
            //刷新选课信息
            this->getStuGrades(CPID);
        });

}



stuGradeQuery::~stuGradeQuery()
{
    delete ui;
}

void stuGradeQuery::getStuGrades(QString CPID)
{
    ui->gradeTable->clearContents();
    QSqlTableModel model;
    model.setTable("sc");;
    qDebug()<<"CPID="<<CPID;
    model.setFilter(QObject::tr("CPID='%1'").arg(CPID));
    model.select();
    int row=model.rowCount();
    qDebug()<<"row="<<row;
    //获取学生信息
    ui->gradeTable->setRowCount(row);
    for(int i=0;i<row;++i)
    {
        for(int k=0;k<4;++k)
            ui->gradeTable->setItem(i,k,new QTableWidgetItem());

        QString StuID=model.record(i).value("StuID").toString();
        qDebug()<<"StuID="<<StuID;
        QSqlTableModel model_1;
        model_1.setTable("student");
        model_1.setFilter(QObject::tr("StuID='%1'").arg(StuID));
        model_1.select();
        qDebug()<<model_1.record(0).value("StuName").toString();
        qDebug()<<model_1.record(0).value("ClassName").toString();

        //获取成绩
        QSqlTableModel model_2;
        model_2.setTable("sc");
        model_2.setFilter(QObject::tr("StuID='%1' and CPID='%2'").arg(StuID).arg(CPID));
        model_2.select();
        qDebug()<<"成绩查询到"<<model_2.rowCount()<<"条记录";
        QString grade=QString::number(model_2.record(0).value("StuGrade").toInt());
        qDebug()<<model_2.record(0).value(0).toString()<<model_2.record(0).value(1).toString()<<model_2.record(0).value(2).toString()<< grade;


        for(int k=0;k<4;++k)
        {
            ui->gradeTable->item(i,0)->setText(StuID);
            ui->gradeTable->item(i,1)->setText(model_1.record(0).value("StuName").toString());
            ui->gradeTable->item(i,2)->setText(model_1.record(0).value("ClassName").toString());
            ui->gradeTable->item(i,3)->setText(grade);
        }
    }

}

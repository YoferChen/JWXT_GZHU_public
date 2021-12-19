#include "course_selection.h"
#include "ui_course_selection.h"
#include <QTreeWidgetItem>
#include <QTreeWidget>


Course_Selection::Course_Selection(QString id, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Course_Selection)
{
    ui->setupUi(this);

    this->userId=id;
    //初始化数据库模型
    model=new QSqlTableModel();
    sqlTool=new SqlTools();
    //选择所有课程
    connect(ui->allCourse,&QRadioButton::clicked,this,[=](){
        currentItem=NULL;
        this->getCourses(0);
    });

    //选择专业必修
    connect(ui->requiredCourse,&QRadioButton::clicked,this,[=](){
        currentItem=NULL;
        this->getCourses(1);
    });

    //选择专业选修
    connect(ui->electiveCourse,&QRadioButton::clicked,this,[=](){
        currentItem=NULL;
        this->getCourses(2);
    });

    //默认选择全部课程
    ui->allCourse->setChecked(true);
    ui->allCourse->clicked(true);

//    展开和收起item
    connect(ui->courseTree,&QTreeWidget::itemDoubleClicked,this,[=](){
        if(currentItem!=NULL)
        {
            qDebug()<<"收起上一个item";
            currentItem->setExpanded(false);
        }
        else
        {
            qDebug()<<"指针为空！";
        }
        currentItem=ui->courseTree->currentItem();
    });

    //选课
    connect(ui->select,&QToolButton::clicked,this,&Course_Selection::selectCourse);

    //退课
    connect(ui->cancel,&QToolButton::clicked,this,&Course_Selection::cancelSelectCourse);

    //课程详情
    connect(ui->teacher,&QToolButton::clicked,this,&Course_Selection::showCourseTeacherInfo);

    //退出
    connect(ui->exit,&QToolButton::clicked,this,&Course_Selection::close);

}

Course_Selection::~Course_Selection()
{
    delete ui;
}

void Course_Selection::getID(QString id)
{
    this->userId=id;
}

void Course_Selection::welcome()
{
    //查询用户名
    QVariant result=sqlTool->getPersonalInfo(2,this->userId,"name");
    qDebug()<<"学生姓名："<<result;
    ui->userID->setText("用户："+this->userId+"  "+result.toString());
}

//-----------------------获取选课课程信息-------------------------
//QTreeWidget的使用：https://blog.csdn.net/chunyexiyu/article/details/78057323
void Course_Selection::getCourses(int courseType)
{
    ui->courseTree->clear();
//    ui->courseTree->setHeaderHidden(true);  //隐藏表头

    ui->courseTree->setColumnCount(6);
    QStringList headNameList;
//    headNameList<<"教学班"<<"上课教师"<<"上课时间"<<"教学地点"<<"课程性质"<<"已选/容量";
    headNameList<<"课程信息"<<"教学班"<<"选课状态"<<""<<""<<"";
    ui->courseTree->setColumnWidth(0,300);
//    headNameList<<"课程信息";
    ui->courseTree->setHeaderLabels(headNameList);

    //查询数据库course表，获取所有课程id    
    QVariantList courseIds=sqlTool->getCourseIds(courseType);

    //获取用户选课情况
//    QList<QSqlRecord> recordList=sqlTool->getStuSelectCourses(this->userId);  //由于一开始是在构造函数中执行的，此时userId还未传入，导致获取不到userId
    //以每个课程为单位，给tree添加子节点
    for(int i=0;i<courseIds.count();++i)
    {
        //查询course表，获取课程的基本信息
        QString courseId=courseIds.at(i).toString();
        QSqlRecord courseInfo=sqlTool->getCourseInfo(courseId);
        int columnCount=courseInfo.count();
        qDebug()<<"-----------------------------";

        //查询coursePool获取教学班信息
        QList<QSqlRecord> courseOpenList=sqlTool->getCourseOpenInfo(courseId);

        int classCount=courseOpenList.count();

        //查询sc表获取课程选择状态
        QString courseSelectState="未选";
        bool isSelectedCourse=false;
        int chooseIndex=-1;
        for(int t=0;t<classCount;++t)
        {
            QString CPID=courseOpenList.at(t).value(0).toString();
            qDebug()<<"CPID="<<CPID;
            if(sqlTool->isSelectCourse(this->userId,CPID))
            {
                isSelectedCourse=true;
                chooseIndex=t;
                break;
            }
        }
        if(isSelectedCourse) courseSelectState="已选";

        //添加item到tree
        QString courseName="("+courseInfo.value(0).toString()+")"+courseInfo.value(1).toString()+" - "
                +courseInfo.value("Credit").toString()+"学分";
        QString classesCount="教学班个数："+QString::number(classCount);
        QString courseState="状态："+courseSelectState;
//        QString courseStament;
//        qDebug()<<courseName.length();
//        if(courseName.length()>26)
//            courseStament=courseName+"\t教学班个数："+QString::number(classCount)+"\t状态："+courseSelectState;
//        else if(courseName.length()>16)
//             courseStament=courseName+"\t\t教学班个数："+QString::number(classCount)+"\t状态："+courseSelectState;
//        else
//             courseStament=courseName+"\t\t\t教学班个数："+QString::number(classCount)+"\t状态："+courseSelectState;
//        qDebug()<<courseStament;

        QTreeWidgetItem *item=new QTreeWidgetItem(ui->courseTree);
        item->setText(0,courseName);
        item->setText(1,classesCount);
        item->setText(2,courseState);
//        item->setText(0,"("+courseInfo.value(0).toString()+")");
//        item->setText(1,courseInfo.value("Credit").toString());
        QTreeWidgetItem* subItem=new QTreeWidgetItem();
        subItem->setText(0,"教学班");
        subItem->setText(1,"上课教师");
        subItem->setText(2,"上课时间");
        subItem->setText(3,"教学地点");
        subItem->setText(4,"课程性质");
        subItem->setText(5,"已选/容量");
        subItem->setDisabled(true);
        item->addChild(subItem);
        for(int t=0;t<courseOpenList.count();++t)
        {
            QSqlRecord record=courseOpenList.at(t);
            if(record.value("Time").toString().isEmpty()) continue;  //上课时间不完整，不加入选课表
            QTreeWidgetItem* subItem=new QTreeWidgetItem();
//            headNameList<<"教学班"<<"上课教师"<<"上课时间"<<"教学地点"<<"课程性质"<<"已选/容量";
            subItem->setText(0,record.value(0).toString());
            subItem->setText(1,sqlTool->getPersonalInfo(1,record.value(2).toString(),"name").toString());
            subItem->setText(2,record.value(6).toString());
            subItem->setText(3,record.value(5).toString());
            subItem->setText(4,courseInfo.value(2).toString());
            //获取选课人数
            int selectCount=sqlTool->getCourseSelectCount(record.value(0).toString());
            subItem->setText(5,QObject::tr("%1/%2").arg(QString::number(selectCount)).arg(record.value(8).toString()));
            item->addChild(subItem);
            if(courseSelectState=="已选"&&t==chooseIndex)
            {
                for(int j=0;j<6;++j)
                {
                    subItem->setTextColor(j,QColor(255,0,0));
                    subItem->setBackgroundColor(j,QColor(251,216,0));
                }
            }
        }

//        break;  //测试

        if(i==0&&courseIds.count()!=0)
        {
            ui->courseTree->setCurrentItem(item);
        }
    }

}

//-----------------------选课-----------------------
void Course_Selection::selectCourse()
{
    QTreeWidgetItem* item=ui->courseTree->currentItem();
    //判断选择的item是否为父结点
    if(item->childCount()!=0)
    {
        qDebug()<<"未选择合法的item";
        QMessageBox::warning(this,"请选择课程","请点击选中要选择的课程");
        return;
    }
    QString CPID=item->text(0);
    qDebug()<<"选择课程号："<<CPID;
    //修改父结点状态
    QTreeWidgetItem* parent=item->parent();
    qDebug()<<"父结点："<<parent->text(0);
    if(parent->text(2)=="状态：已选")
    {
        QMessageBox::warning(this,"课程已选上","课程已选上，请勿重复选择！");
        return;
    }
    //判断时间是否冲突
    QSqlTableModel _model;
    _model.setTable("coursepool");
    _model.setFilter(QObject::tr("CPID='%1'").arg(CPID));
    _model.select();
    QString selectCourseTime=_model.record(0).value("Time").toString();
    QStringList selectCourseTime_split=selectCourseTime.split(QRegExp("/"));
    qDebug()<<"selectCourseTime="<<selectCourseTime;
    _model.setTable("sc");
    _model.setFilter(QObject::tr("StuID='%1'").arg(this->userId));
    _model.select();
    int row=_model.rowCount();
    for(int i=0;i<_model.rowCount();++i)
    {
        QString CPID=_model.record(i).value("CPID").toString();
        QSqlTableModel _model_1;
        _model_1.setTable("coursepool");
        _model_1.setFilter(QObject::tr("CPID='%1'").arg(CPID));
        _model_1.select();
        QString time=_model_1.record(0).value(6).toString();
        qDebug()<<"time="<<time;
        //比较判断
        QString weeks=time.split(QRegExp("/"))[0];  //周数
        QString day=time.split(QRegExp("/"))[1];  //星期几
        QString section=time.split(QRegExp("/"))[2];  //小节

        qDebug()<<"time="<<time;
        qDebug()<<"weeks="<<weeks;
        qDebug()<<"day="<<day;
        qDebug()<<"section="<<section;
        if(selectCourseTime_split[1]==day && selectCourseTime_split[2]==section)
        {
            QStringList weekSplit=weeks.split(QRegExp("-"));
            for(int k=0;k<2;++k)
            {
                for(int j=0;j<2;++j)
                {
                    if(weekSplit[k]==selectCourseTime_split[0].split(QRegExp("-"))[j])
                    {
                        qDebug()<<"课程时间冲突，选课失败！";
                        QMessageBox::warning(this,"选课失败","课程时间冲突，选课失败！");
                        return;
                    }
                }
            }
        }

    }


    //判断选课人数是否达到上限
    QSqlRecord coursePoolRecord=sqlTool->coursePoolInfo(CPID);
    int maxCount=coursePoolRecord.value(8).toInt();
    qDebug()<<QObject::tr("课程：%1  容量：%2").arg(CPID).arg(maxCount);

    //选课人数判断
    if(sqlTool->getCourseSelectCount(CPID)>=maxCount)
    {
        qDebug()<<"选课人数达到上限，选课失败！";
        QMessageBox::warning(this,"选课失败","选课人数达到课程容量上限，选课失败！");
        return;
    }


    parent->setText(2,"状态：已选");

    //向数据库SC表添加记录
    bool success=sqlTool->addCourseRecord(this->userId,CPID);
    if(!success)
    {
        qDebug()<<"事务提交失败！";
        QMessageBox::warning(this,"选课失败","数据库异常，选课失败！");
        return;
    }


    //更新选课人数
    int selectCount=sqlTool->getCourseSelectCount(CPID);

    QSqlTableModel model;
    model.setTable("coursepool");
    model.setFilter(QObject::tr("CPID='%1'").arg(CPID));
    model.select();
    QSqlRecord record=model.record(0);
    item->setText(5,QObject::tr("%1/%2").arg(QString::number(selectCount)).arg(record.value(8).toString()));
    for(int j=0;j<6;++j)
    {
        item->setTextColor(j,QColor(255,0,0));
        item->setBackgroundColor(j,QColor(251,216,0));
    }

}

//------------------------退课------------------------
void Course_Selection::cancelSelectCourse()
{
    QTreeWidgetItem* item=ui->courseTree->currentItem();
    //判断选择的item是否为父结点
    if(item->childCount()!=0)
    {
        qDebug()<<"未选择合法的item";
        QMessageBox::warning(this,"请选择课程","请点击选中要选择的课程");
        return;
    }
    QString CPID=item->text(0);
    qDebug()<<"选择课程号："<<CPID;
    //查询课时是否已选上
    QSqlTableModel model;
    model.setTable("sc");
    model.setFilter(QObject::tr("StuID='%1' and CPID='%2'").arg(this->userId).arg(CPID));
    model.select();
    if(model.rowCount()!=1)
    {
        QMessageBox::warning(this,"退课失败","课程未选择，退课失败！");
        return;
    }

    //修改父结点状态
    QTreeWidgetItem* parent=item->parent();
    qDebug()<<"父结点："<<parent->text(0);
    if(parent->text(2)=="状态：未选")
    {
        QMessageBox::warning(this,"退课失败","课程未选择，退课失败！");
        return;
    }
    parent->setText(2,"状态：未选");

    //向数据库SC表删除记录
    bool success=sqlTool->removeCourseRecord(this->userId,CPID);
    if(!success)
    {
        qDebug()<<"事务提交失败！";
        QMessageBox::warning(this,"选课失败","数据库异常，选课失败！");
        return;
    }


    //更新选课人数
    int selectCount=sqlTool->getCourseSelectCount(CPID);

    model.setTable("coursepool");
    model.setFilter(QObject::tr("CPID='%1'").arg(CPID));
    model.select();
    QSqlRecord record=model.record(0);

    item->setText(5,QObject::tr("%1/%2").arg(QString::number(selectCount)).arg(record.value(8).toString()));
    for(int j=0;j<6;++j)
    {
        item->setTextColor(j,QColor(0,0,0));
        item->setBackgroundColor(j,QColor(255,255,255));
    }

}

//---------------------------显示课程详情------------------------------
void Course_Selection::showCourseTeacherInfo()
{
    //获取点击的磕碜CPID
    QTreeWidgetItem* item=ui->courseTree->currentItem();
    //判断选择的item是否为父结点
    if(item->childCount()!=0)
    {
        qDebug()<<"未选择合法的item";
        QMessageBox::warning(this,"请选择课程","请点击选中要选择的课程");
        return;
    }
    QString TeaID=item->text(1);
    qDebug()<<"教师工号："<<TeaID;

    sqlTool->showTeacherInfo(TeaID);
}

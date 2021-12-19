#include "coursetable.h"
#include "ui_coursetable.h"
#include <QDebug>
#include <QMessageBox>
#include <qsqlrelationaltablemodel.h>
#include <QWidget>
#include <QCheckBox>
#include <QGridLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QList>

CourseTable::CourseTable(int identity, QString id, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CourseTable)
{
    ui->setupUi(this);

    this->identity=identity;
    this->id=id;

    sqlTool=new SqlTools();

    if(identity==2)
    {
        ui->tools->hide();
        ui->courseFilter->hide();
        ui->classList->hide();
        ui->autoScheduleCourse->hide();
        //查询信息
        QString name=sqlTool->getPersonalInfo(2,this->id,"name").toString();
        ui->title->setText(name+" 的课表");
    }
    else if(identity==1)
    {
        this->resize(QSize(this->size().width()+180,this->size().height()+100));
        ui->classWidget->hide();
        ui->coursePoolWidget->hide();
        ui->classCourseTable->hide();
        ui->autoScheduleCourse->hide();

        QString name=sqlTool->getPersonalInfo(1,this->id,"name").toString();
        ui->teacherWidget->hide();
        ui->title->setText(name+" 的课表");
    }
    else if(identity==0)
    {
        //往教师控件中添加信息
        this->resize(QSize(this->size().width()+180,this->size().height()+100));
        QList<QVariant> teacherList= sqlTool->getTableColumnList("teacher","TeaName");
        for(int i=0;i<teacherList.count();++i)
        {
            ui->teacherName->addItem(teacherList.at(i).toString());
        }

        QString name=ui->teacherName->currentText();
        ui->coursePoolWidget->hide();
        ui->title->setText(name+" 的课表");
        identity=1;
        ui->classWidget->hide();
        ui->autoScheduleCourse->hide();
    }
    else
    {
        qDebug()<<"身份无法识别";
    }

//    int idx=0;
    ui->table->setColumnCount(7);
    ui->table->setRowCount(11);
    for(int i=0;i<7;++i)
    {
        ui->table->setColumnWidth(i,168);
    }

//    ui->table->setHorizontalHeaderLabels(QStringList()<<"name"<<"age");
    ui->table->setHorizontalHeaderLabels(QStringList() << "星期一" << "星期二" << "星期三" << "星期四"<<"星期五"<<"星期六"<< "星期天");
//    ui->table->setSelectionBehavior(QAbstractItemView::SelectRows);  //整行选中的方式r
//    ui->table->setEditTriggers(QAbstractItemView::NoEditTriggers);   //禁止修改
//    ui->table->setSelectionMode(QAbstractItemView::SingleSelection);  //设置为可以选中单个
    //需要初始化item，否则单元格的内容为空，出出错
//    for(int i=0;i<11;++i)
//        for(int k=0;k<7;++k)
//             ui->table->setItem(i,k,new QTableWidgetItem(QString::number(idx++)));
//    ui->table->verticalHeader()->setVisible(false);   //隐藏列表头

    ui->table->selectRow(0);


    //设置内容不可编辑
//    ui->table->setEditTriggers(QAbstractItemView::NoEditTriggers);

//    ui->table->setSelectionMode(QAbstractItemView::SingleSelection);

//    ui->table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

//    ui->table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);    //填充表格
    ui->table->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch); //自适应行高

//    for(int i=0;i<7;++i)
//        ui->table->horizontalHeader()->setSectionResizeMode(i, QHeaderView::ResizeToContents);   //然后设置要根据内容使用宽度的列（其他没设置的列自动缩放）


    for(int i=0;i<11;++i)
//        ui->table->horizontalHeader()->setSectionResizeMode(i, QHeaderView::ResizeToContents);   //然后设置要根据内容使用宽度的列（其他没设置的列自动缩放）
        ui->table->verticalHeader()->setSectionResizeMode(i, QHeaderView::ResizeToContents);   //然后设置要根据内容使用宽度的列（其他没设置的列自动缩放）



    //获取课程信息填入表格
    this->getCourseToTable();

    //选课
    connect(ui->addCourse,&QToolButton::clicked,this,[=](){
        if(ui->classCourseTable->text()=="班级课表")
        {
            this->addCourse();
        }
        else
        {
            this->addCourseForClass();
        }
    });

    //退课
    connect(ui->removeCourse,&QToolButton::clicked,this,[=](){
        if(ui->classCourseTable->text()=="班级课表")
        {
            this->removeCourse();
        }
        else
        {
            this->removeCourseForClass();
        }
    });

    //添加课程类型
    ui->courseType->addItem("全部课程");
    ui->courseType->addItem("专业必修");
    ui->courseType->addItem("专业选修");

    //上课周次选择
    ui->courseWeeks->addItem("1-16周");
    ui->courseWeeks->addItem("1-8周");
    ui->courseWeeks->addItem("9-16周");

    //小节数
    ui->courseSections->addItem("2");
    ui->courseSections->addItem("3");

    //根据课程类型获取课程信息  course表
    void (QComboBox:: *signal)(const QString&) = &QComboBox::currentIndexChanged;  //函数指针指定重载函数
    connect(ui->courseType,signal,this,&CourseTable::getCourse);

    ui->courseType->setCurrentIndex(1);
    ui->courseType->setCurrentIndex(0);

    //教师选择改变
    connect(ui->teacherName,signal,this,[=](){
        QString name=ui->teacherName->currentText();
        ui->title->setText(name+" 的课表");
        qDebug()<<"------------查询"<<name<<"的课表---------------";
        this->getCourseToTable();
    });

    //切换到班级课表
    connect(ui->classCourseTable,&QToolButton::clicked,this,[=](){
       if(ui->classCourseTable->text()=="班级课表")
       {
           ui->table->clearContents();

           ui->title->setText(ui->classList->currentText());
           QStringList classNamelist;
           //查询班级名称，添加到控件
           QList<QVariant> list=sqlTool->getTableColumnList("class","ClassName");
           for(int i=0;i<list.count();++i)
           {
               classNamelist.append(list.at(i).toString());
           }
           ui->classList->addItems(classNamelist);

           ui->classList->setCurrentText("计科183");

           ui->teacherWidget->hide();
           ui->weeksWidget->hide();
           ui->sectionWidget->hide();
           ui->placeWidget->hide();
           ui->addClassWidget->hide();
           ui->teachClassWidget->hide();
           //先隐藏控件再显示控件，避免窗口被拉伸
           ui->classWidget->show();
           ui->coursePoolWidget->show();
           ui->autoScheduleCourse->show();

           QString className=ui->classList->currentText();
           ui->title->setText(className+" 的课表");
           ui->classCourseTable->setText("教师课表");
           this->getClassCourse();
       }
       else
       {
           ui->coursePoolWidget->hide();
           ui->classWidget->hide();
           ui->autoScheduleCourse->hide();
           ui->weeksWidget->show();
           ui->sectionWidget->show();
           ui->placeWidget->show();
           ui->teacherWidget->show();
           ui->addClassWidget->show();
           ui->classCourseTable->setText("班级课表");
           QString name=ui->teacherName->currentText();
           ui->title->setText(name+" 的课表");
           this->getCourseToTable();
       }
    });

    //班级切换
    connect(ui->classList,signal,this,[=](){
        QString className=ui->classList->currentText();
        ui->title->setText(className+" 的课表");
        this->getClassCourse();
    });

    //获取教学班
    void (QComboBox:: *signal_activate)(const QString&) = &QComboBox::activated;
    connect(ui->courseList,signal_activate,this,[=](){
        this->getCoursepoolList();
    });

    //添加授课班级
    connect(ui->addClasses,&QToolButton::clicked,this,[=](){
        qDebug()<<ui->teachClasses->currentIndex();
        QWidget *widget=new QWidget();
        widget->resize(200,200);
        QGridLayout* lay=new QGridLayout();
        widget->setLayout(lay);
        QList<QVariant> classNameList=sqlTool->getTableColumnList("class","ClassName");
        int classNum=classNameList.count();
        QCheckBox** box=new QCheckBox*[classNum];
        for(int i=0;i<classNum;++i)
        {
            box[i]=new QCheckBox();
        }
        for(int i=0;i<classNameList.count();++i)
        {
            QString className=classNameList.at(i).toString();
            box[i]=new QCheckBox();
            box[i]->setText(className);
            lay->addWidget(box[i]);
        }
        QPushButton* btn=new QPushButton("确定");
        lay->addWidget(btn);
        widget->setAttribute(Qt::WA_DeleteOnClose);
        widget->show();

        connect(btn,&QPushButton::clicked,this,[=](){
            ui->teachClasses->clear();
            for(int i=0;i<classNum;++i)
            {
                qDebug()<<box[i]->text();
                if(box[i]->checkState()==Qt::Checked)
                {
                    ui->teachClasses->addItem(box[i]->text());
                }
            }
            widget->close();

            for(int i=0;i<classNum;++i)
            {
                delete box[i];
            }
        });
    });

    //智能排课
    connect(ui->autoScheduleCourse,&QToolButton::clicked,this,&CourseTable::autoScheduleCourse);
}

CourseTable::~CourseTable()
{
    delete ui;
}

//查询课表信息
void CourseTable::getCourseToTable()
{
    qDebug()<<"身份："<<this->identity;
    ui->table->clearContents();
    for(int i=0;i<11;++i)
        for(int k=0;k<7;++k)
             ui->table->setItem(i,k,new QTableWidgetItem());
//    ui->table->setHorizontalHeaderLabels(QStringList() << "星期一" << "星期二" << "星期三" << "星期四"<<"星期五"<<"星期六"<< "星期天");
    ui->table->setCurrentCell(0,0);
    //查询sc表获取选课信息
    if(identity==1)  //教师
    {
//        this->resize(QSize(this->size().width()+180,this->size().height()+100));
        //根据教师工号，查询coursePool表，获取老师上课的信息
        QList<QSqlRecord> recordList=sqlTool->getTeaSelectCourses(id);
        QList<QString> CPIDlist;
        qDebug()<<"课程数量："<<recordList.count();
        for(int i=0;i<recordList.count();++i)
        {
            qDebug()<<"---------------------------------";
            QString CPID=recordList.at(i).value(0).toString();
            qDebug()<<"CPID："<<CPID;
            CPIDlist.append(CPID);
           //根据CPID查询课程信息
            QSqlRecord coursePoolInfo=sqlTool->coursePoolInfo(CPID);
            //获取课程id
            QString courseID=coursePoolInfo.value(1).toString();
            QSqlRecord courseInfo=sqlTool->getCourseInfo(courseID);

            //显示课程信息
            for(int j=0;j<coursePoolInfo.count();++j)
            {
                qDebug()<<coursePoolInfo.fieldName(j)<<":"<<coursePoolInfo.value(j);
            }
            for(int j=0;j<courseInfo.count();++j)
            {
                qDebug()<<courseInfo.fieldName(j)<<":"<<courseInfo.value(j);
            }

            //上课时间解析
            QString time=coursePoolInfo.value(6).toString();  //1-8周/星期一/1-2节
//            time="1-8周/星期二/1-2节";
            qDebug()<<"time_teacher:"<<time;
            if(time.isEmpty())
            {
                continue;
            }
            QStringList timeSplit=time.split(QRegExp("[/]"));
            qDebug()<<"时间解析_teacher："<<timeSplit;
            //获取节数（行号+1）
            QStringList rowList=timeSplit[2].split(QRegExp("[节]"))[0].split(QRegExp("[-]"));
            int startRow=rowList[0].toInt();
            int endRow=rowList[1].toInt();
            rowList.clear();
            for(int t=startRow;t<=endRow;++t)
            {
                rowList.append(QString::number(t));
            }

            qDebug()<<"rowList="<<rowList[0].toInt();

            //星期转换
            QStringList week;
            week<< "星期一" << "星期二" << "星期三" << "星期四"<<"星期五"<<"星期六"<< "星期日";
            //转换根据week转换成列数
            for(int j=0;j<week.count();++j)
            {
                //判断插入到哪一列
                if(week.at(j)==timeSplit.at(1))
                {
                    qDebug()<<QObject::tr("数据插入第%1列").arg(j);
                    //插入数据到表格中
                    //获取表格指定单元格的数据，追加数据
                    for(int k=0;k<rowList.count();++k)
                    {
                        qDebug()<<rowList.count();
                        int row=rowList[k].toInt()-1;
                        qDebug()<<"插入行号："<<row;
                        QTableWidgetItem *item= ui->table->item(row,j);
                        qDebug()<<ui->table->currentRow()<<ui->table->currentColumn();
                        if(item==NULL)
                        {
                         qDebug()<<item;
                         qDebug()<<"item指针为空！";
                         return;
                        }
//                        QString content=ui->table->item(rowList[0].toInt()-1,j)->text();
//                        content=content+';\n';
                        //coursePool信息和course表信息组合
                        QString courseName=courseInfo.value(1).toString();
//                        QString classTime=timeSplit[0];
                        QString classTime=coursePoolInfo.value(6).toString();
                        QString local=coursePoolInfo.value(5).toString();
                        QString classType=courseInfo.value(2).toString();
                        QString teaClasses=coursePoolInfo.value(3).toString();
                        QString examType=courseInfo.value(3).toString();
                        QString info=CPID+"/"+courseName+"/"+classTime+"/"+local+"/"+"/"+teaClasses+"/"+classType+"/"+examType;

                        QTableWidgetItem* get_item=ui->table->item(row,j);
                        item->setText(get_item->text()+info);

                    }
                    break;
                }
                if(j==week.count()-1)
                {
                    qDebug()<<"解析出错！";
                }
            }
        }

    }
    else if(identity==2)  //学生
    {
        //查询sc表
        QList<QSqlRecord> recordList=sqlTool->getStuSelectCourses(id);
        QList<QString> CPIDlist;
        qDebug()<<"课程数量："<<recordList.count();
        for(int i=0;i<recordList.count();++i)
        {
            qDebug()<<"---------------------------------";
            QString CPID=recordList.at(i).value(1).toString();
            qDebug()<<"CPID："<<CPID;
            CPIDlist.append(CPID);
           //根据CPID查询课程信息
            QSqlRecord coursePoolInfo=sqlTool->coursePoolInfo(CPID);
            //获取课程id
            QString courseID=coursePoolInfo.value(1).toString();
            QSqlRecord courseInfo=sqlTool->getCourseInfo(courseID);

            //显示课程信息
            for(int j=0;j<coursePoolInfo.count();++j)
            {
                qDebug()<<coursePoolInfo.fieldName(j)<<":"<<coursePoolInfo.value(j);
            }
            for(int j=0;j<courseInfo.count();++j)
            {
                qDebug()<<courseInfo.fieldName(j)<<":"<<courseInfo.value(j);
            }

            //上课时间解析
            QString time=coursePoolInfo.value(6).toString();  //1-8周/星期一/1-2节
//            time="1-8周/星期二/1-2节";
            qDebug()<<"time:"<<time;
            QStringList timeSplit=time.split(QRegExp("[/]"));
            qDebug()<<"时间解析："<<timeSplit;
            //获取节数（行号+1）
            QStringList rowList=timeSplit[2].split(QRegExp("[节]"))[0].split(QRegExp("[-]"));
            qDebug()<<"rowList="<<rowList[0].toInt();
            //星期转换
            QStringList week;
            week<< "星期一" << "星期二" << "星期三" << "星期四"<<"星期五"<<"星期六"<< "星期日";
            //转换根据week转换成列数
            for(int j=0;j<week.count();++j)
            {
                //判断插入到哪一列
                if(week.at(j)==timeSplit.at(1))
                {
                    qDebug()<<QObject::tr("数据插入第%1列").arg(j);
                    //插入数据到表格中
                    //获取表格指定单元格的数据，追加数据
                    for(int k=0;k<rowList.count();++k)
                    {
                        qDebug()<<rowList.count();
                        int row=rowList[k].toInt()-1;
                        qDebug()<<"插入行号："<<row;
                        QTableWidgetItem *item= ui->table->item(row,j);
                        qDebug()<<ui->table->currentRow()<<ui->table->currentColumn();
                        if(item==NULL)
                        {
                         qDebug()<<item;
                         qDebug()<<"item指针为空！";
                         return;
                        }
//                        QString content=ui->table->item(rowList[0].toInt()-1,j)->text();
//                        content=content+';\n';
                        //coursePool信息和course表信息组合
                        QString courseName=courseInfo.value(1).toString();
//                        QString classTime=timeSplit[0];
                        QString classTime=coursePoolInfo.value(6).toString();
                        QString local=coursePoolInfo.value(5).toString();
                        QString classType=courseInfo.value(2).toString();
                        QString teacherName=sqlTool->getPersonalInfo(1,coursePoolInfo.value(2).toString(),"name").toString();
                        QString examType=courseInfo.value(3).toString();
                        QString info=CPID+"/"+courseName+"/"+teacherName+"/"+classTime+"/"+local+"/"+classType+"/"+examType;
                        QTableWidgetItem* get_item=ui->table->item(row,j);
                        item->setText(get_item->text()+info);
                    }
                    break;
                }
                if(j==week.count()-1)
                {
                    qDebug()<<"解析出错！";
                }
            }
        }
    }
    else if(identity==0) //管理员
    {
        QString teaName=ui->teacherName->currentText();
        qDebug()<<"当前教师姓名："<<teaName;
        QString teaID=sqlTool->getPersonalIDbyName(1,teaName);
        qDebug()<<"教师工号："<<teaID;
        QList<QSqlRecord> recordList=sqlTool->getTeaSelectCourses(teaID);
        QList<QString> CPIDlist;
        qDebug()<<"课程数量："<<recordList.count();
        for(int i=0;i<recordList.count();++i)
        {
            qDebug()<<"---------------------------------";
            QString CPID=recordList.at(i).value(0).toString();
            qDebug()<<"CPID："<<CPID;
            CPIDlist.append(CPID);
           //根据CPID查询课程信息
            QSqlRecord coursePoolInfo=sqlTool->coursePoolInfo(CPID);
            //获取课程id
            QString courseID=coursePoolInfo.value(1).toString();
            QSqlRecord courseInfo=sqlTool->getCourseInfo(courseID);

            //显示课程信息
            for(int j=0;j<coursePoolInfo.count();++j)
            {
                qDebug()<<coursePoolInfo.fieldName(j)<<":"<<coursePoolInfo.value(j);
            }
            for(int j=0;j<courseInfo.count();++j)
            {
                qDebug()<<courseInfo.fieldName(j)<<":"<<courseInfo.value(j);
            }

            //上课时间解析
            QString time=coursePoolInfo.value(6).toString();  //1-8周/星期一/1-2节
//            time="1-8周/星期二/1-2节";
            qDebug()<<"time:"<<time;
            if(time.isEmpty())
            {
                continue;
            }
            QStringList timeSplit=time.split(QRegExp("[/]"));
            qDebug()<<"时间解析："<<timeSplit;
            //获取节数（行号+1）
            QStringList rowList=timeSplit[2].split(QRegExp("[节]"))[0].split(QRegExp("[-]"));
            int startRow=rowList[0].toInt();
            int endRow=rowList[1].toInt();
            rowList.clear();
            for(int t=startRow;t<=endRow;++t)
            {
                rowList.append(QString::number(t));
            }
            //星期转换
            QStringList week;
            week<< "星期一" << "星期二" << "星期三" << "星期四"<<"星期五"<<"星期六"<< "星期日";
            //转换根据week转换成列数
            for(int j=0;j<week.count();++j)
            {
                //判断插入到哪一列
                if(week.at(j)==timeSplit.at(1))
                {
                    qDebug()<<QObject::tr("数据插入第%1列").arg(j);
                    //插入数据到表格中
                    //获取表格指定单元格的数据，追加数据
                    for(int k=0;k<rowList.count();++k)
                    {
                        qDebug()<<rowList.count();
                        int row=rowList[k].toInt()-1;
                        qDebug()<<"插入行号："<<row;
                        QTableWidgetItem *item= ui->table->item(row,j);
                        qDebug()<<ui->table->currentRow()<<ui->table->currentColumn();
                        if(item==NULL)
                        {
                         qDebug()<<item;
                         qDebug()<<"item指针为空！";
                         return;
                        }
//                        QString content=ui->table->item(rowList[0].toInt()-1,j)->text();
//                        content=content+';\n';
                        //coursePool信息和course表信息组合
                        QString courseName=courseInfo.value(1).toString();
//                        QString classTime=timeSplit[0];
                        QString classTime=coursePoolInfo.value(6).toString();
                        QString local=coursePoolInfo.value(5).toString();
                        QString classType=courseInfo.value(2).toString();
                        QString teaClasses=coursePoolInfo.value(3).toString();
                        QString examType=courseInfo.value(3).toString();
                        QString info=CPID+"/"+courseName+"/"+classTime+"/"+local+"/"+"/"+teaClasses+"/"+classType+"/"+examType;

                        QTableWidgetItem* get_item=ui->table->item(row,j);
                        item->setText(get_item->text()+info);

                    }
                    break;
                }
                if(j==week.count()-1)
                {
                    qDebug()<<"解析出错！";
                }
            }
        }

    }
    else
    {
        qDebug()<<"用户身份无法识别！";
    }


}

//==========================获取课程=========================
void CourseTable::getCourse()
{
    ui->courseList->clear();
    qDebug()<<"查询课程";
    QString courseType=ui->courseType->currentText();
    //获取指定类型的课程
    QString examType;
    qDebug()<<"选择的课程类型："<<ui->courseType->currentText();
    if(ui->courseType->currentText()=="全部课程") examType="NULL";
    else if(ui->courseType->currentText()=="专业必修") examType="专业必修课程";
    else examType="专业选修课程";
    qDebug()<<"CourType="<<examType;
    QList<QSqlRecord> courses=sqlTool->getCourses("CourType",examType);
    //添加到控价
    QStringList courseList;
    for(int i=0;i<courses.count();++i)
    {
        courseList.append(courses.at(i).value(1).toString());
    }
    ui->courseList->addItems(courseList);


    //获取教学班
    this->getCoursepoolList();
}

//====================添加课程====================
void CourseTable::addCourse()
{
    QTableWidgetItem* clickItem=ui->table->currentItem();
    int row=ui->table->currentRow();
    int column=ui->table->currentColumn();
    //获取选择的课程
    QString courseName=ui->courseList->currentText();
    //获取课程id
    QString CourID=sqlTool->getCourses("CourName",courseName).at(0).value(0).toString();
    //添加到coursePool表
    //计算主键
    int CPID_new=0;
    QList<QVariant> CPID_list= sqlTool->getTableColumnList("coursepool","CPID");
    for(int i=0;i<CPID_list.count();++i)
    {
        int CPID_current=CPID_list.at(i).toString().toInt();
        if(CPID_current>CPID_new) CPID_new=CPID_current;
    }
    CPID_new+=1;
    //新增记录到coursepool表
    QSqlTableModel _model;
    _model.setTable("coursepool");
    QSqlRecord record=_model.record();
    record.setValue("CPID",CPID_new);
    qDebug()<<"CPID="<<CPID_new;
    if(identity==1)
        record.setValue("TeaID",this->id);
    else if(identity==0)
    {
        QString teaName=ui->teacherName->currentText();
        QString teaID=sqlTool->getPersonalIDbyName(1,teaName);
        record.setValue("TeaID",teaID);
    }
    record.setValue("CourID",CourID);
    QString time;
    //获取点击位置和周数选择，确定上课时间【冲突检测】
    QString weeks=ui->courseWeeks->currentText();
    int columnSelected=ui->table->currentColumn();
    int rowSelected=ui->table->currentRow();

    QString day;
    QStringList days;
    days<< "星期一" << "星期二" << "星期三" << "星期四"<<"星期五"<<"星期六"<< "星期日";
    for(int i=0;i<days.count();++i)
    {
        if(columnSelected==i)
        {
            day=days[i];
            break;
        }
    }
    QString courseSection=ui->courseSections->currentText();
    time=weeks+"/"+day+"/"+QString::number(rowSelected+1)+"-"+QString::number(rowSelected+ui->courseSections->currentText().toInt())+"节";
    qDebug()<<"课程时间："<<time;
    record.setValue("Time",time);
    QString place=ui->coursePlace->text();
    qDebug()<<"上课地点："<<place;
    if(place.isEmpty())
    {
        QMessageBox::warning(this,"地点为空","请输入授课地点");
        return;
    }
    record.setValue("Place",place);
    if(ui->teachClasses->currentIndex()==-1)  //教授班级为空，
    {
        QMessageBox::warning(this,"授课班级为空","请点击+号按钮，添加授课班级！");
        return;
    }
    //获取教授班级字段信息
    QString OpenClass;
    qDebug()<<"ui->teachClasses->count()="<<ui->teachClasses->count();
    for(int i=0;i<ui->teachClasses->count();++i)
    {
        if(i!=ui->teachClasses->count()-1)
            OpenClass+=ui->teachClasses->itemText(i)+";";
        else
            OpenClass+=ui->teachClasses->itemText(i);
    }
    qDebug()<<"OpenClass="<<OpenClass;
    record.setValue("OpenClass",OpenClass);
    int OpenNumMax=ui->teachClasses->count()*40;
    int OpenNumMin=30;
    record.setValue("OpenNumMax",OpenNumMax);
    record.setValue("OpenNumMin",OpenNumMin);

    qDebug()<<"添加课程！";
    _model.insertRecord(-1,record);
    //提交事务
    _model.database().transaction();
    if(_model.submitAll())
    {
        _model.database().commit();
        QMessageBox::warning(this,"操作成功","添加课程成功！");
        this->getCourseToTable();
    }
    else
    {
        _model.database().rollback();
        QMessageBox::warning(this,"操作失败","数据库出错，添加课程失败！请联系管理员处理！");
    }

}

//================================删除课程=============================
void CourseTable::removeCourse()
{
    qDebug()<<"删除课程";
    if(identity==0 ||identity==1)
    {
        //获取CPID
        QString courseInfo = ui->table->currentItem()->text();
        if(courseInfo.isEmpty())
        {
            QMessageBox::warning(this,"选择的课程为空","请点击选择合法的课程");
            return;
        }

        QString CPID=ui->table->currentItem()->text().split(QRegExp("/"))[0];
        qDebug()<<"CPID="<<CPID;
        bool removeCourse=sqlTool->removeCourseFromCoursepool(CPID);
        if(removeCourse)
        {
            QMessageBox::information(this,"操作成功","课程删除成功！");
            //刷新课程表
            this->getCourseToTable();
        }
        else
        {
            QMessageBox::warning(this,"操作失败","数据库异常，课程删除失败！");
            return;
        }
    }
}

//班级课表
void CourseTable::getClassCourse()
{
    //初始化单元格
    ui->table->clearContents();
    for(int i=0;i<11;++i)
        for(int k=0;k<7;++k)
             ui->table->setItem(i,k,new QTableWidgetItem());
//    ui->table->setHorizontalHeaderLabels(QStringList() << "星期一" << "星期二" << "星期三" << "星期四"<<"星期五"<<"星期六"<< "星期天");
    ui->table->setCurrentCell(0,0);


    //获取班级CPID
    qDebug()<<"查询班级课表";
    //获取班级名称
    QString className=ui->classList->currentText();
    //查询coursepool表获取班级课程信息
    QSqlTableModel model;
    model.setTable("coursepool");
    model.select();
    QStringList CPID_list;
    for(int i=0;i<model.rowCount();++i)
    {
        QSqlRecord record=model.record(i);
        if(record.value("Time").toString().isEmpty()) continue;  //为确定时间的课程不能选
        QString openClasses=record.value(record.indexOf("OpenClass")).toString();
//        qDebug()<<openClasses;
        QStringList split=openClasses.split(QRegExp(";"));
//        qDebug()<<split;
        for(int k=0;k<split.count();++k)
        {
            if(split[k]==className)
            {
                CPID_list.append(record.value("CPID").toString());
                break;
            }
        }
    }
//    qDebug()<<"CPID_list="<<CPID_list;

//    过滤掉非必修课程
    QStringList CPID_list_filter;
    for(int i=0;i<CPID_list.count();++i)
    {
        QString CourID=sqlTool->coursePoolInfo(CPID_list[i]).value("CourID").toString();
        QString CourType=sqlTool->getCourseInfo(CourID).value("CourType").toString();
        if(CourType=="专业必修课程")
        {
            CPID_list_filter.append(CPID_list[i]);
        }
    }
    qDebug()<<"CPID_list_filter="<<CPID_list_filter;

    //------------添加课表信息【代码复用】---------------
    for(int i=0;i<CPID_list_filter.count();++i)
    {
//        qDebug()<<"---------------------------------";
        QString CPID=CPID_list_filter[i];
//        qDebug()<<"CPID："<<CPID;
       //根据CPID查询课程信息
        QSqlRecord coursePoolInfo=sqlTool->coursePoolInfo(CPID);
        //获取课程id
        QString courseID=coursePoolInfo.value("CourID").toString();
        QSqlRecord courseInfo=sqlTool->getCourseInfo(courseID);

        //显示课程信息
//        for(int j=0;j<coursePoolInfo.count();++j)
//        {
//            qDebug()<<coursePoolInfo.fieldName(j)<<":"<<coursePoolInfo.value(j);
//        }
//        for(int j=0;j<courseInfo.count();++j)
//        {
//            qDebug()<<courseInfo.fieldName(j)<<":"<<courseInfo.value(j);
//        }

        //上课时间解析
        QString time=coursePoolInfo.value(6).toString();  //1-8周/星期一/1-2节
//            time="1-8周/星期二/1-2节";
//        qDebug()<<"time:"<<time;
        QStringList timeSplit=time.split(QRegExp("[/]"));
//        qDebug()<<"时间解析："<<timeSplit;
        //获取节数（行号+1）
        QStringList rowList=timeSplit[2].split(QRegExp("[节]"))[0].split(QRegExp("[-]"));
        int start=rowList[0].toInt();
        int end=rowList[1].toInt();
        rowList.clear();
        for(int i=start;i<=end;++i)
        {
            rowList.append(QString::number(i));
        }
//        qDebug()<<"rowList="<<rowList[0].toInt();
        //星期转换
        QStringList week;
        week<< "星期一" << "星期二" << "星期三" << "星期四"<<"星期五"<<"星期六"<< "星期日";
        //转换根据week转换成列数
        for(int j=0;j<week.count();++j)
        {
            //判断插入到哪一列
            if(week.at(j)==timeSplit.at(1))
            {
//                qDebug()<<QObject::tr("数据插入第%1列").arg(j);
                //插入数据到表格中
                //获取表格指定单元格的数据，追加数据
                for(int k=0;k<rowList.count();++k)
                {
//                    qDebug()<<rowList.count();
                    int row=rowList[k].toInt()-1;
//                    qDebug()<<"插入行号："<<row;
                    QTableWidgetItem *item= ui->table->item(row,j);
//                    qDebug()<<ui->table->currentRow()<<ui->table->currentColumn();
                    if(item==NULL)
                    {
                     qDebug()<<item;
                     qDebug()<<"item指针为空！";
                     return;
                    }
//                        QString content=ui->table->item(rowList[0].toInt()-1,j)->text();
//                        content=content+';\n';
                    //coursePool信息和course表信息组合
                    QString courseName=courseInfo.value(1).toString();
//                        QString classTime=timeSplit[0];
                    QString classTime=coursePoolInfo.value(6).toString();
                    QString local=coursePoolInfo.value(5).toString();
                    QString classType=courseInfo.value(2).toString();
                    QString teacherName=sqlTool->getPersonalInfo(1,coursePoolInfo.value(2).toString(),"name").toString();
                    QString examType=courseInfo.value(3).toString();
                    QString info=CPID+"/"+courseName+"/"+teacherName+"/"+classTime+"/"+local+"/"+classType+"/"+examType;
                    QTableWidgetItem* get_item=ui->table->item(row,j);
                    item->setText(get_item->text()+info);
                }
                break;
            }
            if(j==week.count()-1)
            {
                qDebug()<<"解析出错！";
            }
        }
    }
}

//===================获取教学班信息===================
void CourseTable::getCoursepoolList()
{
    ui->coursepoolList->clear();
    QString CourName=ui->courseList->currentText();
    qDebug()<<"CourName="<<CourName;
    QString CourID=sqlTool->getCourses("CourName",CourName).at(0).value("CourID").toString();
    QList<QSqlRecord> records=sqlTool->getFilterRecords("coursepool","CourID",CourID);
    //获取教学班信息
    for(int i=0;i<records.count();++i)
    {
        if(records[i].value("Time").toString().isEmpty()) continue;
        QString teaName=sqlTool->getPersonalInfo(1,records[i].value("TeaID").toString(),"name").toString();
        QString info=records[i].value("CPID").toString()+"/"+teaName+"/"+records[i].value("Time").toString()+"/"+records[i].value("Place").toString();
        qDebug()<<"info:"<<info;
        ui->coursepoolList->addItem(info);
    }
}

 //为班级添加课程【修改coursePool表中的开课班级字段】
void CourseTable::addCourseForClass()
{
    //获取班级名
    QString className=ui->classList->currentText();
    qDebug()<<"班级："<<className;
    //获取添加的课程的CPID
    QString CPID=ui->coursepoolList->currentText().split(QRegExp("/"))[0];
    qDebug()<<"CPID="<<CPID;

    //查询记录
    QSqlTableModel model;
    model.setTable("coursepool");
    model.setFilter(QObject::tr("CPID='%1'").arg(CPID));
    model.select();
    qDebug()<<"查询到"<<model.rowCount()<<"条记录";
    QSqlRecord record=model.record(0);
    //获取班级字段信息
    QString classNameFieldValue=model.record(0).value("OpenClass").toString();
    if(!classNameFieldValue.isEmpty())
    {
        QStringList classesList=classNameFieldValue.split(QRegExp(";"));
        //判断是否已有该课程
        for(int i=0;i<classesList.count();++i)
        {
            if(className==classesList[i])
            {
                QMessageBox::warning(this,"课程已存在","课程已存在，请勿重复选择！");
                return;
            }
        }
    }
    qDebug()<<"classNameFieldValue="<<classNameFieldValue;
    if(classNameFieldValue.isEmpty())
        classNameFieldValue+=className;
    else
        classNameFieldValue=classNameFieldValue+";"+className;
    model.setData(model.index(0,record.indexOf("OpenClass")),classNameFieldValue);

    //提交
    model.database().transaction();
    if(model.submitAll())
    {
        model.database().commit();
        QMessageBox::warning(this,"操作成功","添加课程成功！");
        this->getClassCourse();  //刷新课表
    }
    else
    {
        model.database().rollback();
        QMessageBox::warning(this,"操作失败","数据库出错，添加课程失败！请联系管理员处理！");
    }
    this->getClassCourse();
}

//为班级移出课程【修改coursePool表中的开课班级字段】
void CourseTable::removeCourseForClass()
{
    //获取班级名
    QString className=ui->classList->currentText();
    qDebug()<<"班级："<<className;
    //获取要退的课程
    QTableWidgetItem* item=ui->table->currentItem();
    if(item->text().isEmpty())
    {
        QMessageBox::warning(this,"选择课程为空","请点击选择合法的课程！");
        return;
    }
    QString CPID=item->text().split(QRegExp("/"))[0];
    qDebug()<<"CPID="<<CPID;

    this->getClassCourse();  //刷新课表
    QSqlTableModel model;
    model.setTable("coursepool");
    model.setFilter(QObject::tr("CPID='%1'").arg(CPID));
    model.select();
    qDebug()<<"查询到"<<model.rowCount()<<"条记录";
    QSqlRecord record=model.record(0);
    //获取班级字段信息
    QString classNameFieldValue=model.record(0).value("OpenClass").toString();

    qDebug()<<"classNameFieldValue="<<classNameFieldValue;
    QStringList classesList=classNameFieldValue.split(QRegExp(";"));
    if(classesList.count()==1)
        classNameFieldValue="";
    else
    {
        for(int i=0;i<classesList.count();++i)
        {
            if(classesList[i]==className){
                classesList.removeAt(i);
                break;
            }
        }
    }
    classNameFieldValue=classesList.join(";");

    model.setData(model.index(0,record.indexOf("OpenClass")),classNameFieldValue);

    //提交
    model.database().transaction();
    if(model.submitAll())
    {
        model.database().commit();
        QMessageBox::warning(this,"操作成功","移除课程成功！");
        this->getClassCourse();  //刷新课表
    }
    else
    {
        model.database().rollback();
        QMessageBox::warning(this,"操作失败","数据库出错，移除课程失败！请联系管理员处理！");
    }
    this->getClassCourse();
}

//智能排课【为了方便测试，读取计科的课程安排，为软件排课】
void CourseTable::autoScheduleCourse()
{
    qDebug()<<"----------------智能排课---------------";
    AutoScheduleAlg* alg=new AutoScheduleAlg();
    QList<QList<int>> cpInfoList;
    QSqlTableModel _model;
    _model.setTable("course");
    _model.setFilter(QObject::tr("CourType='%1'").arg("专业必修课程"));
    _model.select();
    qDebug()<<"查询到"<<_model.rowCount()<<"门专业必修课";
    QStringList courIDList;  //课程id
    QList<int> hourList;  //学时
    //获取专业必修课程ID【假设计科与软件的课程相同】
    for(int i=0;i<_model.rowCount();++i)
    {
        courIDList.append(_model.record(i).value("CourID").toString());
        hourList.append(_model.record(i).value("CourHour").toInt());
        qDebug()<<_model.record(i).value("CourName").toString();
    }
    //查询课程的所有教学班信息
    _model.setTable("coursepool");
    for(int i=0;i<courIDList.count();++i)
    {
        _model.setFilter(QObject::tr("CourID='%1'").arg(courIDList[i]));
        _model.select();
        qDebug()<<courIDList[i]<<"教学班："<<_model.rowCount();
        //获取相关字段信息
        for(int j=0;j<_model.rowCount();++j)
        {
            QList<int> cpInfo;
            cpInfo.append(hourList[i]);
            cpInfo.append(_model.record(j).value("CPID").toInt());
            cpInfo.append(_model.record(j).value("teaID").toInt());
            QString teaClasses=_model.record(j).value("OpenClass").toString();
            if(teaClasses.isEmpty())
            {
                QMessageBox::warning(this,"开课班级为空",_model.record(j).value("CPID").toString()+"的开课班级为空！");
                return;
            }
            QStringList teaClassList=teaClasses.split(QRegExp(";"));
//            qDebug()<<"teaClassList="<<teaClassList;
            cpInfo.append(teaClassList.count());
            //生成班级代号序列
            QString classCodes;
            //判断是否是软件的课程【判断该课程给软件专业开设的班级数量】
            int jikeNum=0;
            for(int k=0;k<teaClassList.count();++k)
            {
                if(teaClassList[k].split(QRegExp("18"))[0]=="软件")
                {
                    jikeNum+=1;
                    classCodes+=teaClassList[k].split(QRegExp("18"))[1]; //计科183==>计科18|3
                }
            }
            if(jikeNum==0)  //过滤掉计科的课程
            {
                continue;
            }
            cpInfo.append(classCodes.toInt());
            cpInfo.append(courIDList[i].toInt());
            //打印测试
            cpInfoList.append(cpInfo);
            qDebug()<<cpInfo[0]<<cpInfo[1]<<cpInfo[2]<<cpInfo[3]<<cpInfo[4]<<cpInfo[5];
        }
    }
//    qDebug()<<cpInfoList;

    //开课班级【用软件专业的班级来测试】
    _model.setTable("class");
    _model.select();
    int classNum=0;
    for(int i=0;i<_model.rowCount();++i)
    {
        if(_model.record(i).value("ClassName").toString().split(QRegExp("18"))[0]=="软件")
        {
            classNum+=1;
        }
    }
    qDebug()<<"软件的班级数量："<<classNum;
//    return;

    alg->test(cpInfoList,6);

    //将课表添加记录到coursepool表
    //增加教学班
    //手动增加软件181到软件186班级
    qDebug()<<"---------------------------------";
    _model.setTable("coursepool");
    for(int i=1;i<=classNum;++i)  //课程信息算法返回的数量下标从1开始
    {
        qDebug()<<"----------软件18"<<i<<"-------------";
        //1-8周课表
        for (int y = 0; y < 5; y++)
        {
            //cout << "第" << x << "大节";
            for (int x = 1; x <= 7; x++)
            {
                //1-8周
//                qDebug()<<QObject::tr("alg->cla[%1].first[%2][%3]=").arg(i).arg(x).arg(y)<<alg->cla[i].first[x][y];
                if (alg->cla[i].first[x][y])
                {  //有课程，添加记录【根据所在的时间，为记录的时间字段添加数值】
                    //存储CPID，int类型需要转换成QString类型
                    QString weekCount="1-8周";
                    if(alg->cla[i].last[x][y]) weekCount="1-16周";
//                    cout<< alg->cla[i].first[x][y] << '\t';
                    //获取记录
                    QString CPID=QString::number(alg->cla[i].first[x][y]);
                    _model.setFilter(QObject::tr("CPID='%1'").arg(CPID));
                    _model.select();
                    //判断是否已有时间
                    if(!_model.record(0).value("Time").toString().isEmpty()){qDebug()<<"课程时间已添加！";}
                    else{
                        QString time;
                        QString day=getDay(x);
                        QString section=getSection(y+1);
                        time=weekCount+"/"+day+"/"+section;
                        qDebug()<<"time1="<<time;
                        _model.setData(_model.index(0,_model.record().indexOf("Time")),time);
                        _model.submit();
                    }

                }
                //9-16周
//                qDebug()<<QObject::tr("alg->cla[%1].last[%2][%3]=").arg(i).arg(x).arg(y)<<alg->cla[i].last[x][y];
//                qDebug()<<bool(alg->cla[i].last[x][y]);
                if (alg->cla[i].last[x][y])
                {
                    if(alg->cla[i].first[x][y]){qDebug()<<"1:课程时间已添加！";}  //前面已经添加了，无需重复添加
                    else
                    {
//                        cout << alg->cla[i].last[x][y]<< '\t';
                        //获取记录
                        QString CPID=QString::number(alg->cla[i].last[x][y]);
                        _model.setFilter(QObject::tr("CPID='%1'").arg(CPID));
                        _model.select();
                        //判断是否已有时间
                        if(!_model.record(0).value("Time").toString().isEmpty()){qDebug()<<"2:课程时间已添加！";}
                        else{
                            QString weekCount="9-16周";
                            QString time;
                            QString day=getDay(x);
                            QString section=getSection(y+1);
                            time=weekCount+"/"+day+"/"+section;
                            qDebug()<<"time2="<<time;
                        }
                    }
//                    return;  //测试
                }
            }
        }
    }

    //提交事务
    if(_model.submitAll())
    {
        _model.database().commit();
        QMessageBox::information(this,"操作成功","智能排课成功！\n专业必修课程已添加到软件工程专业课表中！");
    }
    else{
        _model.database().rollback();
        QMessageBox::warning(this,"操作失败","智能排课失败！\n数据库异常，请联系管理员处理！");
    }

}

QString CourseTable::getDay(int column)
{
    switch (column) {
    case 1:
        return "星期一";
        break;
    case 2:
        return "星期二";
        break;
    case 3:
        return "星期三";
        break;
    case 4:
        return "星期四";
        break;
    case 5:
        return "星期五";
        break;
    case 6:
        return "星期六";
        break;
    case 7:
        return "星期日";
        break;
    default:
        break;
    }
}

QString CourseTable::getSection(int section)
{
    switch (section) {
    case 1:
        return "1-2节";
        break;
    case 2:
        return "3-4节";
        break;
    case 3:
        return "5-6节";
        break;
    case 4:
        return "7-8节";
        break;
    case 5:
        return "9-11节";
        break;
    default:
        break;
    }
}


#include "studentinfomanage.h"
#include "ui_studentinfomanage.h"
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QSqlError>
#include <QDebug>
#include <QVBoxLayout>
#include <QStandardItemModel>
#include <QSqlField>


studentInfoManage::studentInfoManage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::studentInfoManage)
{
    ui->setupUi(this);

    //函数指针（信号函数重载）
    void(QComboBox::*signal)(int )=&QComboBox::currentIndexChanged;
    //选择专业下拉按钮
    connect(ui->major,signal,this,&studentInfoManage::addClassName);


//    ui->major->setCurrentIndex(2);
    ui->major->currentIndexChanged(0);

    //选择班级，并刷新班级学生信息
    connect(ui->class_idx,signal,this,&studentInfoManage::selectClassAndShowTable);

    //初始化widget和view
    lay=new QVBoxLayout(ui->tableData);
    view=new QTableView();


    //获取初始化班级的学生信息
    this->selectClassAndShowTable();

    //增加记录
    addRecordTable = new QStandardItemModel(ui->addDataTable);
    addRecordTable->setColumnCount(5);
    addRecordTable->setHeaderData(0,Qt::Horizontal,"学号");
    addRecordTable->setHeaderData(1,Qt::Horizontal,"班级");
    addRecordTable->setHeaderData(2,Qt::Horizontal,"姓名");
    addRecordTable->setHeaderData(3,Qt::Horizontal,"性别");
    addRecordTable->setHeaderData(4,Qt::Horizontal,"邮箱");
    ui->addDataTable->setModel(addRecordTable);
//    addRecordTable->
    ui->addDataTable->setColumnWidth(4,300);
    QList<QStandardItem*> list;
    list << new QStandardItem("1806100500") << new QStandardItem("计科181") <<new QStandardItem("李华")<< new QStandardItem("男")<<new QStandardItem("1806100500@gzhu.edu.cn");
    addRecordTable->insertRow(0, list);


    //新增行
    connect(ui->addLine,&QPushButton::clicked,this,&studentInfoManage::addRecord);

    //提交新增数据
    connect(ui->addCommit,&QPushButton::clicked,this,&studentInfoManage::addCommit);

    //提交修改
    connect(ui->modifySure,&QPushButton::clicked,this,&studentInfoManage::modifyComfirm);

    //撤销修改
    connect(ui->modifyRestore,&QPushButton::clicked,this,&studentInfoManage::modifyRestore);

    //删除选中记录
    connect(ui->removeRecords,&QPushButton::clicked,this,&studentInfoManage::removeRecords);

    //返回
    connect(ui->exit,&QPushButton::clicked,this,&studentInfoManage::close);
}

studentInfoManage::~studentInfoManage()
{
    delete ui;
}

//添加班级信息到下拉按钮
void studentInfoManage::addClassName()
{
    QString majorName=ui->major->currentText() ;

    //根据专业查询班级列表，添加到班级下拉按钮中
    //根据班级表获取班级名称
    QSqlTableModel model;
    model.setTable("class");
    model.select();
    int ret=model.rowCount();
    ui->class_idx->clear();
    for(int i=0;i<ret;++i)
    {
        QSqlRecord record= model.record(i);

        if(majorName=="计算机科学与技术")
        {
             QString className=record.value(0).toString();
             QString classType=className.mid(0,2);
             if(classType=="计科"){
                 ui->class_idx->addItem(className);
             }
        }
        else if (majorName=="软件工程")
        {
            QString className=record.value(0).toString();
            QString classType=className.mid(0,2);
            if(classType=="软件"){
                ui->class_idx->addItem(className);
            }
        }
        else if(majorName=="网络工程")
        {
            QString className=record.value(0).toString();
            QString classType=className.mid(0,2);
            if(classType=="网络"){
                ui->class_idx->addItem(className);
            }
        }
    }
}

//选择班级并显示班级学生信息
void studentInfoManage::selectClassAndShowTable()
{
    QString classSelect=ui->class_idx->currentText();
    model=new QSqlTableModel();
    model->setTable("student");
    model->setFilter("ClassName='"+classSelect+"'");
    model->select();
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);  //设置手动提交
    model->setHeaderData(0,Qt::Horizontal,"学号");
    model->setHeaderData(1,Qt::Horizontal,"班级");
    model->setHeaderData(2,Qt::Horizontal,"姓名");
    model->setHeaderData(3,Qt::Horizontal,"性别");
    model->setHeaderData(4,Qt::Horizontal,"个人课表");
    model->setHeaderData(6,Qt::Horizontal,"邮箱");

    view->setModel(model);
    view->hideColumn(5);

    lay->addWidget(view);



}

//添加记录
void studentInfoManage::addRecord()
{
//    qDebug()<<"表格新增空白行！";
    addRecordTable->insertRow(addRecordTable->rowCount());
}

//提交新增数据
void studentInfoManage::addCommit()
{
    //读取数据
    int rowCount=addRecordTable->rowCount();
    //获取所有学号
    QSqlTableModel* newModel=new QSqlTableModel();
    newModel->setTable("student");
    newModel->select();


    for(int i=0;i<rowCount;++i)
    {
        //检查数据是否合规，是否与现有数据冲突
        for(int t=0;t<newModel->rowCount();++t)
        {
            QSqlRecord record=newModel->record(t);
//            qDebug()<<record.value(0).toString()<<"=?"<<addRecordTable->item(i,0)->text();
            if(record.value(0).toString()==addRecordTable->item(i,0)->text())
            {
                qDebug()<<"学号已存在！";
                QMessageBox::information(this,"添加失败","学号已存在，请检查输入的信息！");
                return;
            }
        }
    }
    //没有问题：提交数据库
    for(int i=0;i<rowCount;++i)
    {
        //表格数据转换为记录
//        model->database().transaction();
        QSqlRecord newRecord;
        newRecord=model->record();  //根据model->record获取各字段的数据类型格式

        newRecord.setValue(0,addRecordTable->item(i,0)->text());  //学号
        newRecord.setValue(1,addRecordTable->item(i,1)->text());  //班级
        newRecord.setValue(2,addRecordTable->item(i,2)->text());  //姓名
        newRecord.setValue(3,addRecordTable->item(i,3)->text());  //性别
        newRecord.setNull(4);  //个人课表
        newRecord.setNull(5);  //密码
        newRecord.setValue(6,addRecordTable->item(i,4)->text());  //邮箱
        //添加到数据库
        qDebug()<<newRecord.value(0)<<newRecord.value(1)<<newRecord.value(2);

        model->insertRecord(-1,newRecord);
    }
    //提交事务到数据库
    model->database().transaction();
    if(!model->submitAll())
    {
        qDebug()<<model->lastError().text();
        QMessageBox::warning(this,"插入数据失败","导入数据到数据库失败！\n请检查数据是否合法!");
    }
    else
    {
        qDebug()<<"新增数据成功！";
        model->database().commit();  //提交
        QMessageBox::information(this,"添加数据成功","成功添加学生数据到数据库！");
        //情况输入框
        int rowCount=addRecordTable->rowCount();
        for(int i=0;i<rowCount;++i)
        {
            addRecordTable->removeRow(i);
        }
    }
    //刷新数据视图
    this->selectClassAndShowTable();

}

//确认修改
void studentInfoManage::modifyComfirm()
{
    // 开始事务操作
    model->database().transaction();
    //尝试提交事务
    if (model->submitAll())
    {
        model->database().commit(); //提交
        qDebug()<<"提交数据库成功";
        QMessageBox::information(this,"修改成功！","提交数据库修改成功！");
    }
    else
    {
        model->database().rollback(); //回滚
        qDebug()<<model->lastError().text();
        QMessageBox::warning(this, tr("数据修改失败"),tr("数据库错误: %1").arg(model->lastError().text()));
    }
}

//撤销model的修改【还未提交数据库】
void studentInfoManage::modifyRestore()
{
    model->revertAll();
    QMessageBox::information(this,"撤销修改","撤销所有修改成功！");
}

//删除选中的记录
void studentInfoManage::removeRecords()
{
    int answer=QMessageBox::question(this,"删除确认","确定要删除选中的记录？");
    qDebug()<<answer;
    if (answer==QMessageBox::No) return;
    //通过view获取被选中的部分数据
    QItemSelectionModel* selectModel=view->selectionModel();
    //通过选中的数据结构，获取选择格子的ModelIndex
    QModelIndexList selectList=selectModel->selectedIndexes();
    //存储要删除的行号
    QList<int> delRow;
    //行号去重
    for(int i=0;i<selectList.size();++i)
    {
        QModelIndex index=selectList.at(i);
        delRow<<index.row();
    }
    if(delRow.size()==0)
    {
        QMessageBox::warning(this,"删除异常","请选择要删除的记录！");
        return;
    }
    while(delRow.size()>0)
    {
        int row=delRow.at(0);
        delRow.removeAll(row);  //去重
        model->removeRow(row);  //删除记录
    }
    //提交事务
    model->database().transaction();
    if (model->submitAll())
    {
        model->database().commit(); //提交
        qDebug()<<"删除选择记录成功";
        QMessageBox::information(this,"删除成功！","从数据库删除选中学生记录成功！");

    }
    else
    {
        model->database().rollback(); //回滚
        qDebug()<<model->lastError().text();
        QMessageBox::warning(this, tr("数据修改失败"),tr("数据库错误: %1").arg(model->lastError().text()));

    }

}

void studentInfoManage::hideModifyTools()
{
    ui->toolWidget->hide();
    ui->addDataWidget->hide();
}

void studentInfoManage::setTitle(QString title)
{
    ui->title->setText(title);
}

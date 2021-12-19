#include "sqltools.h"
#include <QDebug>

SqlTools::SqlTools()
{
    model=new QSqlTableModel();
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
}

//根据身份和id、需要的字段名获取对应字段
QVariant SqlTools::getPersonalInfo(int identity, QString id, QString infoNeed)
{

    QString tableName;  //设置模型的表名
    int fieldIndex;  //字段所在下标
    QString condition;  //筛选条件

    if(identity==0)  //管理员
    {
        condition="AdminID='"+id+"'";
        tableName="admin";
        if(infoNeed=="pwd") fieldIndex=1;
        else if (infoNeed=="email") fieldIndex=2;
        else{
            qDebug()<<"请求信息非法！";
            return -1;
        }
    }
    else if(identity==1)  //教师
    {
        condition="TeaID='"+id+"'";
        tableName="teacher";
        if(infoNeed=="name") fieldIndex=1;
        else if (infoNeed=="table") fieldIndex=2;
        else if (infoNeed=="isDean") fieldIndex=3;
        else if (infoNeed=="pwd") fieldIndex=4;
        else if (infoNeed=="email") fieldIndex=5;
        else{
            qDebug()<<"请求信息非法！";
            return -1;
        }
    }
    else if(identity==2)  //学生
    {
        condition="StuID='"+id+"'";
        tableName="student";
        if(infoNeed=="className") fieldIndex=1;
        else if (infoNeed=="name") fieldIndex=2;
        else if (infoNeed=="sex") fieldIndex=3;
        else if (infoNeed=="table") fieldIndex=4;
        else if (infoNeed=="pwd") fieldIndex=5;
        else if (infoNeed=="email") fieldIndex=6;
        else{
            qDebug()<<"请求信息非法！";
            return -1;
        }
    }
    else
    {
        qDebug()<<"身份信息无法识别！";
        return -1;
    }
//    qDebug()<<"tableName="<<tableName;
//    qDebug()<<"fieldIndex="<<fieldIndex;

    model->setTable(tableName);
//    qDebug()<<"查找条件："<<condition;
    model->setFilter(condition);
    model->select();
//    qDebug()<<"model->rowCount()="<<model->rowCount();
    if(model->rowCount()==1)
    {
        QSqlRecord record=model->record(0);
        qDebug()<<record.value(fieldIndex);
        qDebug()<<record.value(fieldIndex).type();
        return record.value(fieldIndex);
    }
    else if(model->rowCount()==0)
    {
        qDebug()<<"用户不存在！";
        return -1;
    }
    else
    {
        qDebug()<<"数据库异常！";
        return -1;
    }

}

//查询course表获取所有课程id   //type：查询课程类别：0：all，1:必修，2：选修
QVariantList SqlTools::getCourseIds(int type)
{
    model->setTable("course");
    if(type==0);
    else if(type==1) model->setFilter(QObject::tr("CourType='%1'").arg("专业必修课程"));
    else if(type==2) model->setFilter(QObject::tr("CourType='%1'").arg("专业选修课程"));
    model->select();
    int rowCount=model->rowCount();
    QVariantList list;
    for(int i=0;i<rowCount;++i)
    {
        QSqlRecord record=model->record(i);
        list.append(record.value(0));
    }
    qDebug()<<"查询到的课程id："<<list;
    return list;
}

//查询course表获取课程的基本信息
QSqlRecord SqlTools::getCourseInfo(QString id)
{
    model->setTable("course");
    model->setFilter(QObject::tr("CourID='%1'").arg(id));
    model->select();
//    qDebug()<<QObject::tr("查询到%1条记录").arg(model->rowCount());
    return model->record(0);
}

//查询coursePool表，获取开课信息
QList<QSqlRecord> SqlTools::getCourseOpenInfo(QString courID)
{
    model->setTable("coursepool");
    model->setFilter(QObject::tr("CourID='%1'").arg(courID));
    model->select();
    QList<QSqlRecord> list;
    for(int i=0;i<model->rowCount();++i)
    {
        list.append(model->record(i));
    }
    return list;
}

//查询sc表，获取某门课的选课学生人数
int SqlTools::getCourseSelectCount(QString CPID)
{
    model->setTable("sc");
    model->setFilter(QObject::tr("CPID='%1'").arg(CPID));
    model->select();
    return model->rowCount();
}

//查询sc表，判断一门课程是否有选择
bool SqlTools::isSelectCourse(QString stuID, QString CPID)
{
    model->setTable("sc");
    model->setFilter(QObject::tr("CPID='%1' and StuID='%2'").arg(CPID).arg(stuID));
    model->select();
    qDebug()<<"查询选课情况："<<model->rowCount()<<bool(model->rowCount());
    return bool(model->rowCount());
}

//查询sc表，获取学生个人选课情况
QList<QSqlRecord> SqlTools::getStuSelectCourses(QString stuID)
{
    model->setTable("sc");
    model->setFilter(QObject::tr("StuID='%1'").arg(stuID));
    model->select();
    QList<QSqlRecord> list;
    int rowCount=model->rowCount();
    qDebug()<<"用户："<<stuID<<"查询到"<<rowCount<<"条选课记录！";
    for(int i=0;i<rowCount;++i)
    {
        list.append(model->record(i));
    }
    return list;
}

//查询coursepool表，获取教师个人选课情况
QList<QSqlRecord> SqlTools::getTeaSelectCourses(QString teaID)
{
    model->setTable("coursepool");
    model->setFilter(QObject::tr("TeaID='%1'").arg(teaID));
    model->select();
    QList<QSqlRecord> list;
    int rowCount=model->rowCount();
    qDebug()<<"用户："<<teaID<<"查询到"<<rowCount<<"条选课记录！";
    for(int i=0;i<rowCount;++i)
    {
        list.append(model->record(i));
    }
    return list;
}

//向sc表添加选课记录
bool SqlTools::addCourseRecord(QString stuID,QString CPID)
{

    model->setTable("sc");
    QSqlRecord newRecord=model->record();
    newRecord.setValue(0,stuID);
    newRecord.setValue(1,CPID);
    model->insertRecord(-1,newRecord);
    //提交事务
    model->database().transaction();
    if(model->submitAll())
    {
        model->database().commit();
    }
    else
    {
        model->database().rollback();
        return false;
    }
    return true;
}

//从sc表中删除选课记录
bool SqlTools::removeCourseRecord(QString stuID, QString CPID)
{
    model->setTable("sc");
    model->setFilter(QObject::tr("StuID='%1' and CPID='%2'").arg(stuID).arg(CPID));
    model->select();
    model->removeRow(0);
//    qDebug()<<"删除的行："<<model->record(0).
//    model->
    //提交事务
    model->database().transaction();
    if(model->submitAll())
    {
        model->database().commit();
    }
    else
    {
        model->database().rollback();
        return false;
    }
    return true;
}

//查询coursepool表获取信息
QSqlRecord SqlTools::coursePoolInfo(QString CPID)
{
    QSqlTableModel model;
    model.setTable("coursepool");
    model.setFilter(QObject::tr("CPID='%1'").arg(CPID));
    model.select();
    return model.record(0);
}

//弹窗显示指定教师信息
void SqlTools::showTeacherInfo(QString TeaName)
{
    qDebug()<<"教师："<<TeaName;
    QSqlQueryModel* _model=new QSqlQueryModel();
    QWidget* widget=new QWidget();
    widget->setWindowTitle("教师信息");
    QTableView *_view=new QTableView(widget);
    _view->setModel(_model);
    widget->resize(QSize(600,100));

    _model->setQuery(QObject::tr("select TeaID,TeaName,isDean,Email from teacher where TeaName='%1'").arg(TeaName));
    _model->setHeaderData(0,Qt::Horizontal, "教师工号");
    _model->setHeaderData(1,Qt::Horizontal, "教师姓名");
    _model->setHeaderData(2,Qt::Horizontal, "系主任");
    _model->setHeaderData(3,Qt::Horizontal, "邮箱");
    qDebug()<<"教师信息：";

    QVBoxLayout *lay=new QVBoxLayout(widget);
    lay->addWidget(_view);
    widget->show();
}

//根据查询条件获取相关课程记录
QList<QSqlRecord> SqlTools::getCourses(QString filter, QString value)
{
    qDebug()<<"filter="<<filter<<"value="<<value;
    model->setTable("course");
    QString comdition;
    if(value!="NULL")
        comdition=filter+"='"+value+"'";
        qDebug()<<"过滤条件："<<comdition;
    model->setFilter(comdition);
    model->select();
    QList<QSqlRecord> records;
    for(int i=0;i<model->rowCount();++i)
    {
        records.append(model->record(i));
    }
    qDebug()<<"查询到："<<model->rowCount()<<"条记录";
    return records;
}

QList<QVariant> SqlTools::getTableColumnList(QString tableName, QString fieldName)
{
    model->setTable(tableName);
    model->select();
    QSqlRecord record=model->record();
    int index=record.indexOf(fieldName);
    qDebug()<<fieldName<<"字段的索引号为："<<index;
    QList<QVariant> recordList;
    for(int i=0;i<model->rowCount();++i)
    {
        recordList.append(model->record(i).value(index));
    }
    return recordList;
}

//根据教师姓名获取教师id
QString SqlTools::getPersonalIDbyName(int identity, QString name)
{
    QString tableName;
    QString fieldName;
    if(identity==0){
        tableName="admin";
        fieldName="AdminName";
    }
    else if(identity==1){
        tableName="teacher";
        fieldName="TeaName";
    }
    else{
        tableName="student";
        fieldName="StuName";
    }
    model->setTable(tableName);
    QString filter=QObject::tr("%1='%2'").arg(fieldName).arg(name);
    qDebug()<<"查询条件："<<tableName<<filter;
    model->setFilter(filter);
    model->select();
    qDebug()<<"查询到"<<model->rowCount()<<"条记录";
    return model->record(0).value(0).toString();
}

//从coursepool中删除课程
bool SqlTools::removeCourseFromCoursepool(QString CPID)
{
    qDebug()<<"获取到的CPID:"<<CPID;
    model->setTable("coursepool");
    model->setFilter(QObject::tr("CPID='%1'").arg(CPID));
    model->select();
    qDebug()<<"model.rowCount()="<<model->rowCount();

    model->removeRow(0);
    //提交事务
    model->database().transaction();
    if(model->submitAll())
    {
        model->database().commit();
    }
    else
    {
        model->database().rollback();
        return false;
    }
    return true;
}

//根据条件获取表中满足条件的所有记录
QList<QSqlRecord> SqlTools::getFilterRecords(QString tableName, QString filter, QString value)
{
    model->setTable(tableName);
    model->setFilter(QObject::tr("%1='%2'").arg(filter).arg(value));
    model->select();
    QList<QSqlRecord> records;
    for(int i=0;i<model->rowCount();++i)
    {
        records.append(model->record(i));
    }
    qDebug()<<"查询到"<<records.count()<<"条记录";
    return records;
}

/*
 *     if(identity==0)  //管理员
    {
        tableName="admin";
        if(infoNeed=="Pwd") fieldIndex=1;
        else if (infoNeed=="Email") fieldIndex=2;
        else{
            qDebug()<<"请求信息非法！";
            return;
        }
    }
    else if(identity==1)  //教师
    {
        tableName="teacher";
        if(infoNeed=="TeaName") fieldIndex=1;
        else if (infoNeed=="TeaTable") fieldIndex=2;
        else if (infoNeed=="isDean") fieldIndex==3;
        else if (infoNeed=="Pwd") fieldIndex==4;
        else if (infoNeed=="Email") fieldIndex==5;
        else{
            qDebug()<<"请求信息非法！";
            return;
        }
    }
    else if(identity==2)  //学生
    {
        tableName="student";
        if(infoNeed=="ClassName") fieldIndex=1;
        else if (infoNeed=="StuName") fieldIndex=2;
        else if (infoNeed=="StuSex") fieldIndex==3;
        else if (infoNeed=="PersonTable") fieldIndex==4;
        else if (infoNeed=="Pwd") fieldIndex==5;
        else if (infoNeed=="Email") fieldIndex==6;
        else{
            qDebug()<<"请求信息非法！";
            return;
        }
    }
    else
    {
        qDebug()<<"身份信息无法识别！";
        return;
    }
 */

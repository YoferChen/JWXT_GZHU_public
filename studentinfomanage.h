#ifndef STUDENTINFOMANAGE_H
#define STUDENTINFOMANAGE_H

#include <QWidget>
#include <QSqlTableModel>
#include <QTableView>
#include "QSqlRecord"
#include <QDebug>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QStandardItemModel>

namespace Ui {
class studentInfoManage;
}

class studentInfoManage : public QWidget
{
    Q_OBJECT

public:
    explicit studentInfoManage(QWidget *parent = 0);
    ~studentInfoManage();

private:
    Ui::studentInfoManage *ui;
    QSqlTableModel* model;
    QTableView* view;
    QVBoxLayout* lay;
    QStandardItemModel* addRecordTable;

//    QSqlTableModel* modelAdd;
//    QTableView* viewAdd;
//    QVBoxLayout* layAdd;

public slots:
    void addClassName();  //添加班级名称（根据专业从数据库读取）
    void selectClassAndShowTable();  //选择班级并刷新班级学生信息
    void addRecord();  //向表中添加新的记录
    void addCommit();  //提交新增数据
    void modifyComfirm();  //确认修改数据（提交数据库）
    void modifyRestore();  //撤销修改
    void removeRecords();  //删除选择的记录

    void hideModifyTools();
    void setTitle(QString title);

};

#endif // STUDENTINFOMANAGE_H

#include "a_stuinfo.h"
#include "ui_a_stuinfo.h"

A_StuInfo::A_StuInfo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::A_StuInfo)
{
    ui->setupUi(this);
}

A_StuInfo::~A_StuInfo()
{
    delete ui;
}

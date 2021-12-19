#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlError>
#include<QDebug>
#include "adminwin.h"
#include "teacherwin.h"
#include "studentwin.h"
#include "findpwdwin.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    AdminWin* adminWin;
    TeacherWin* teacherWin;
    StudentWin* studentWin;
    FindPwdWin* findPwdWin;

private:
    Ui::MainWindow *ui;
    QSqlDatabase connectDB();
    int indentity=0;
};

#endif // MAINWINDOW_H

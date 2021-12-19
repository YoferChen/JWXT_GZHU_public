#-------------------------------------------------
#
# Project created by QtCreator 2021-01-04T11:20:08
#
#-------------------------------------------------

QT       += core gui
QT       += sql
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = JWXT_GZHU
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    adminwin.cpp \
    teacherwin.cpp \
    studentwin.cpp \
    findpwdwin.cpp \
    a_stuinfo.cpp \
    modifypwdwin.cpp \
    smtp.cpp \
    studentinfomanage.cpp \
    course_selection.cpp \
    sqltools.cpp \
    coursetable.cpp \
    autoschedulealg.cpp \
    stugradequery.cpp

HEADERS += \
        mainwindow.h \
    adminwin.h \
    teacherwin.h \
    studentwin.h \
    findpwdwin.h \
    a_stuinfo.h \
    modifypwdwin.h \
    smtp.h \
    studentinfomanage.h \
    course_selection.h \
    sqltools.h \
    coursetable.h \
    autoschedulealg.h \
    stugradequery.h

FORMS += \
        mainwindow.ui \
    adminwin.ui \
    teacherwin.ui \
    studentwin.ui \
    findpwdwin.ui \
    a_stuinfo.ui \
    modifypwdwin.ui \
    studentinfomanage.ui \
    course_selection.ui \
    coursetable.ui \
    stugradequery.ui

QTPLUGIN += qsqlodbc

QMAKE_CXXFLAGS += /utf-8  # 解决vc++编译中文乱码

RESOURCES += \
    recourse.qrc

#CONFIG += console


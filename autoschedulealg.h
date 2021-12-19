#ifndef AUTOSCHEDULEALG_H
#define AUTOSCHEDULEALG_H

#include <string>
#include <iostream>
#include <fstream>
#include<algorithm>
#include <QList>
using namespace std;


/*
如果是只有8周的课往后放
不考虑连续上三小节的情况
16课时->1-8周/9-16周两小节课
32课时->1-16周两小节课
48课时->尽量安排晚上，1-16周晚上三小节
*/
/*struct course_info
{
    int hour;		//课时
    int cour_id;	//课程ID
    int CPID;		//课程池名称
    int tea_id;		//教师ID
    int cla_num;	//有几个班
    bool cla[999];	//哪几个班上这门课
    bool first[8][5];//1-8周星期一到星期五 一天五大节
    bool last[8][5];//9-16周
}cp[999];	*/	//cp_num+1个课程池

//struct table
//{
//    int first[8][5];//1-8周星期一到星期日 一天五大节
//    int last[8][5];	//9-16周
//}cla[999],tea[999];	//6个班,19个教师

////给课程排序，同时上的班级多的优先考虑
//bool cmp(course a, course b)
//{
//    return a.cla_num > b.cla_num;
//}

class AutoScheduleAlg
{
public:
    AutoScheduleAlg();
    int cp_num, all_class;

    struct course
    {
        int hour;		//课时
        int cour_id;	//课程ID
        int CPID;		//课程池名称
        int tea_id;		//教师ID
        int cla_num;	//有几个班
        bool cla[999];	//哪几个班上这门课
        bool first[8][5];//1-8周星期一到星期五 一天五大节
        bool last[8][5];//9-16周
    }cp[999];		//cp_num+1个课程池

    struct table
    {
        int first[8][5];//1-8周星期一到星期日 一天五大节
        int last[8][5];	//9-16周
    }cla[999],tea[999];	//6个班,19个教师

    //给课程排序，同时上的班级多的优先考虑
    static bool cmp(course a, course b)
    {
        return a.cla_num > b.cla_num;
    }
    void course_arranging_base();
    void print();
    void sort_course();

    void test(QList<QList<int>> cpInfo,int classNum);

};

#endif // AUTOSCHEDULEALG_H

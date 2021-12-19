#include "autoschedulealg.h"
#include<QDebug>
#include <QList>
AutoScheduleAlg::AutoScheduleAlg()
{

}


void AutoScheduleAlg::course_arranging_base()
{
    //扫描每一个课程进行排课
    qDebug()<<"cp_num="<<cp_num<<"cp[i].cla_num="<<cp[0].cla_num ;
    qDebug()<<"cla[j]="<<cla[0].first<<cla[0].last;
    qDebug()<<"-------------------------------------";
//    for(int i=1;i<=cp_num;i++)
//    {
//        qDebug()<<cp[i].cla_num;
//        for(int j=0;j<all_class;++j)
//        {
//            qDebug()<<"cp[i].cla[j]="<<cp[i].cla[j];
//        }
//    }
    for(int i = 0; i < cp_num; i++)
    {
        qDebug()<<"课程："<<i;
        qDebug()<<cp[i].hour<<cp[i].CPID<<cp[i].tea_id<<cp[i].cla_num;
    }
    qDebug()<<"-------------------------------------";
    for (int i = 1; i <= cp_num; i++)
    {
        int flag = 0;//0-没安排上课，1-安排上课
        int is_ok = 1;
        //如果只有一个班上这门课,扫描这个班的时间空隙
        qDebug()<<"----cp[i].cla_num----"<<cp[i].cla_num;
        if (cp[i].cla_num == 1)
        {
            qDebug()<<"1Class";
            //扫描是哪个班上课
            int j;
            for (j = 1; j <= all_class; j++)
            {
                if (cp[i].cla[j] == 1)
                    break;
            }
            qDebug()<<j<<"class";
            //如果是16课时
            if (cp[i].hour == 16)
            {
                qDebug()<<cp[i].hour<<"hours";
                for (int x = 1; x <= 5; x++)
                {
                    for (int y = 0; y < 4; y++)
                    {
                        //如果这个班级星期X的第Y大节课是空的并且老师的课表是空的，则安排上
                        if (!cla[j].last[x][y]&&!tea[cp[i].tea_id- 20000].last[x][y])
                        {
                            cla[j].last[x][y] = cp[i].CPID;
                            tea[cp[i].tea_id - 20000].last[x][y] = cp[i].CPID;
                            flag = 1;
                            qDebug()<<"sucess";
                        }
                        if (flag)
                            break;
                    }
                    if (flag)
                        break;
                }
                //如果后八周没找到位置安排课程，则遍历前八周
                if (!flag)
                {
                    for (int x = 1; x <= 5; x++)
                    {
                        for (int y = 0; y < 4; y++)
                        {
                            //如果这个班级星期X的第Y大节课是空的，则安排上
                            if (!cla[j].first[x][y]&&!tea[cp[i].tea_id - 20000].first[x][y])
                            {
                                cla[j].first[x][y] = cp[i].CPID;
                                tea[cp[i].tea_id - 20000].first[x][y] = cp[i].CPID;
                                flag = 1;
                                qDebug()<<"sucess";
                            }
                            if (flag)
                                break;
                        }
                        if (flag)
                            break;
                    }
                }

            }
            else if (cp[i].hour == 32)
            {
                for (int x = 1; x <= 5; x++)
                {
                    for (int y = 0; y < 4; y++)
                    {
                        //如果这个班级星期X的第Y大节课是空的，则安排上
                        if (!cla[j].first[x][y]&&!cla[j].last[x][y] && !tea[cp[i].tea_id - 20000].first[x][y]&& !tea[cp[i].tea_id - 20000].last[x][y])
                        {
                            cla[j].first[x][y] = cp[i].CPID;
                            cla[j].last[x][y] = cp[i].CPID;
                            tea[cp[i].tea_id - 20000].first[x][y] = cp[i].CPID;
                            tea[cp[i].tea_id - 20000].last[x][y] = cp[i].CPID;
                            flag = 1;
                            qDebug()<<"sucess";
                        }
                        if (flag)
                            break;
                    }
                    if (flag)
                        break;
                }
            }
            else if (cp[i].hour == 48)
            {
                for (int x = 1; x <= 5; x++)
                {
                    //如果这个班级星期X的第Y大节课是空的，则安排上
                    if (!cla[j].first[x][4] && !cla[j].last[x][4] && !tea[cp[i].tea_id - 20000].first[x][4] && !tea[cp[i].tea_id - 20000].last[x][4])
                    {
                        cla[j].first[x][4] = cp[i].CPID;
                        cla[j].last[x][4] = cp[i].CPID;
                        tea[cp[i].tea_id - 20000].first[x][4] = cp[i].CPID;
                        tea[cp[i].tea_id - 20000].last[x][4] = cp[i].CPID;
                        flag = 1;
                        qDebug()<<"sucess";
                    }
                    if (flag)
                        break;
                }
            }

        }
        else
        {
            qDebug()<<"many Class";
            if (cp[i].hour == 16)
            {
                qDebug()<<cp[i].hour<<"hours";
                for (int x = 1; x <= 5; x++)
                {
                    for (int y = 0; y < 4; y++)
                    {
                        is_ok = 1;
                        //如果教师这节课有空
                        if (!tea[cp[i].tea_id - 20000].last[x][y])
                        {
                            //判断那几个班是不是有空
                            for (int j = 1; j <= all_class; j++)
                            {
                                if (cp[i].cla[j] && cla[j].last[x][y])
                                {
                                    is_ok = 0;
                                    break;
                                }
                            }
                            //这几个班级这个时间段有空
                            if (is_ok)
                            {
                                for (int j = 1; j <= all_class; j++)
                                {
                                    if(cp[i].cla[j])
                                        cla[j].last[x][y] = cp[i].CPID;
                                    qDebug()<<j<<"calsucess";
                                }
                                flag = 1;
                                if (flag)
                                    break;
                            }
                        }
                    }
                    if (flag)
                        break;
                }
                if (!flag)
                {
                    for (int x = 1; x <= 5; x++)
                    {
                        for (int y = 0; y < 4; y++)
                        {
                            is_ok = 1;
                            //如果教师这节课有空
                            if (!tea[cp[i].tea_id - 20000].first[x][y])
                            {
                                //判断那几个班是不是有空
                                for (int j = 1; j <= all_class; j++)
                                {
                                    if (cp[i].cla[j] && cla[j].first[x][y])
                                    {
                                        is_ok = 0;
                                        break;
                                    }
                                }
                                //这几个班级这个时间段有空
                                if (is_ok)
                                {
                                    tea[cp[i].tea_id - 20000].last[x][y] = cp[i].CPID;
                                    tea[cp[i].tea_id - 20000].first[x][y] = cp[i].CPID;
                                    for (int j = 1; j <= all_class; j++)
                                    {
                                        if (cp[i].cla[j])
                                            cla[j].first[x][y] = cp[i].CPID;
                                    }
                                    flag = 1;
                                    if (flag)
                                        break;
                                }
                            }
                        }
                        if (flag)
                            break;
                    }
                }
            }
            else if (cp[i].hour == 32)
            {
                qDebug()<<cp[i].hour<<"hours";
                for (int x = 1; x <= 5; x++)
                {
                    for (int y = 0; y < 4; y++)
                    {
                        is_ok = 1;
                        //如果教师这节课有空
                        if (!tea[cp[i].tea_id - 20000].first[x][y]&&!tea[cp[i].tea_id - 20000].last[x][y])
                        {
                            //判断那几个班是不是有空
                            for (int j = 1; j <= all_class; j++)
                            {
                                if (cp[i].cla[j] && (cla[j].first[x][y] || cla[j].last[x][y]))
                                {
                                    is_ok = 0;
                                    break;
                                }
                            }
                            //这几个班级这个时间段有空
                            if (is_ok)
                            {
                                tea[cp[i].tea_id - 20000].last[x][y] = cp[i].CPID;
                                tea[cp[i].tea_id - 20000].first[x][y] = cp[i].CPID;
                                for (int j = 1; j <= all_class; j++)
                                {
                                    if (cp[i].cla[j])
                                    {
                                        cla[j].last[x][y] = cp[i].CPID;
                                        cla[j].first[x][y] = cp[i].CPID;
                                    }

                                }
                                flag = 1;
                                if (flag)
                                    break;
                            }
                        }
                    }
                    if (flag)
                        break;
                }
            }
            else if (cp[i].hour == 48)
            {
                qDebug()<<cp[i].hour<<"hours";
                for (int x = 1; x <= 5; x++)
                {
                    is_ok = 1;
                    //如果教师这节课有空
                    if (!tea[cp[i].tea_id - 20000].first[x][4] && !tea[cp[i].tea_id - 20000].last[x][4])
                    {
                        //判断那几个班是不是有空
                        for (int j = 1; j <= all_class; j++)
                        {
                            if (cp[i].cla[j] && (cla[j].first[x][4] || cla[j].last[x][4]))
                            {
                                is_ok = 0;
                                break;
                            }
                        }
                        //这几个班级这个时间段有空
                        if (is_ok)
                        {
                            tea[cp[i].tea_id - 20000].last[x][4] = cp[i].CPID;
                            tea[cp[i].tea_id - 20000].first[x][4] = cp[i].CPID;
                            for (int j = 1; j <= all_class; j++)
                            {
                                if (cp[i].cla[j])
                                {
                                    cla[j].last[x][4] = cp[i].CPID;
                                    cla[j].first[x][4] = cp[i].CPID;
                                }
                            }
                            flag = 1;
                            if (flag)
                                break;
                        }
                    }
                    if (flag)
                        break;
                }
            }
        }
        qDebug()<<"try Class";
        if (!flag)
        {
            cout << "ERROR：有一门课排不上课。" << endl;
            cout << cp[i].CPID << endl;
        }
    }
}

void AutoScheduleAlg::print()
{
    for (int i = 1; i < all_class; i++)
    {
        qDebug() << QObject::tr("计科18%1的课表").arg(i);
        qDebug() << "1-8周" << endl;
        qDebug() << "一" << "\t" << "二" << "\t" << "三" << "\t" << "四" << "\t" << "五" << "\t" << "六" << "\t" << "日" << endl;
        for (int y = 0; y < 5; y++)
        {
            //cout << "第" << x << "大节";
            for (int x = 1; x <= 7; x++)
            {
                if (cla[i].first[x][y])
                    cout<< cla[i].first[x][y] << '\t';
                else
                    cout << "\t";
            }
            cout << endl;
        }
        cout << endl;
        qDebug() << "9-16周" << endl;
        qDebug() << "一" << "\t" << "二" << "\t" << "三" << "\t" << "四" << "\t" << "五" << "\t" << "六" << "\t" << "日" << endl;
        for (int y = 0; y < 5; y++)
        {
            //cout << "第" << x << "大节";
            for (int x = 1; x <= 7; x++)
            {
                if (cla[i].last[x][y])
                {
                    cout << cla[i].last[x][y]<< '\t';
//                    return;  //测试
                }
                else
                    cout << "\t";
            }
            cout << endl;
        }
        cout << endl<<endl;
    }
}


void AutoScheduleAlg::sort_course()
{
    course temp;

    return;
    for (int i = 1; i <= cp_num; i++) {
        //对待排序序列进行冒泡排序
        for (int j = i + 1; j + 1 < cp_num - i; j++)
        {
            //相邻元素进行比较，当顺序不正确时，交换位置
            if (cp[i].cla_num < cp[j].cla_num)
            {
                for (int x = 1; x <= all_class; x++)
                    temp.cla[x] = cp[i].cla[x];
                temp.cla_num = cp[i].cla_num;
                temp.cour_id = cp[i].cour_id;
                temp.CPID = cp[i].CPID;
                temp.hour = cp[i].hour;

                for (int x = 1; x <= all_class; x++)
                    cp[i].cla[x] = cp[j].cla[x];
                cp[i].cla_num = cp[j].cla_num;
                cp[i].cour_id = cp[j].cour_id;
                cp[i].CPID = cp[j].CPID;
                cp[i].hour = cp[j].hour;

                for (int x = 1; x <= all_class; x++)
                    cp[j].cla[x] = temp.cla[x];
                cp[j].cla_num = temp.cla_num;
                cp[j].cour_id = temp.cour_id;
                cp[j].CPID = temp.CPID;
                cp[j].hour = temp.hour;
            }
        }
    }
}


void AutoScheduleAlg::test(QList<QList<int>> cpInfo,int classNum)
{
//    ifstream myfile("C:\\Users\\17325\\Desktop\\PK2.txt");
//    cp_num = 36;
    cp_num=cpInfo.count();
    all_class = classNum+1;
    int s;
    memset(cp,0,sizeof(cp));
    memset(cla,0,sizeof(cla));
    memset(tea,0,sizeof(tea));
    qDebug()<<"获取到的数据："<<cpInfo;
    for (int i = 0; i < cp_num; i++)
    {
//        myfile >> cp[i].hour;  //学时
//        myfile >> cp[i].CPID;  //CPID
//        myfile >> cp[i].tea_id;  //教师工号
//        myfile >> cp[i].cla_num;  //课程授课班级数
//        myfile >> s;  //选课班级代号（如123，代表计科181，计科182，计科183）
//        myfile >> cp[i].cour_id;  //课程id
        qDebug()<<"课程："<<i;
        qDebug()<<cp[i].hour<<cp[i].CPID<<cp[i].tea_id<<cp[i].cla_num;
        qDebug()<<cpInfo[i];
        cp[i+1].hour=cpInfo[i][0];
        cp[i+1].CPID=cpInfo[i][1];
        cp[i+1].tea_id=cpInfo[i][2];
        cp[i+1].cla_num=cpInfo[i][3];
        s=cpInfo[i][4];
        cp[i+1].cour_id=cpInfo[i][5];
//        qDebug()<<"测试："<<cp[i].hour<<cp[i].CPID<<cp[i].tea_id<<cp[i].cla_num<<s<<cp[i].cour_id;
//        return;
        int temp= s % 10;
        while (temp)
        {
            cp[i+1].cla[temp] = 1;
            s = s / 10;
            temp = s % 10;
        }
    }

//    for(int i = 0; i < cp_num; i++)
//    {
//        qDebug()<<"课程："<<i;
//        qDebug()<<cp[i].hour<<cp[i].CPID<<cp[i].tea_id<<s<<cp[i].cla_num;
//    }
//    sort((cp + 1), (cp + cp_num + 1), cmp);
    sort_course();
    //
    course_arranging_base();
    qDebug()<<"---------------排课完成，打印课表----------------";
    print();
}

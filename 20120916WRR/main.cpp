#include<iostream>
#include<stdio.h>
#include"wrrscheduler.h"
#include"stepscheduler.h"
#include"randomscheduler.h"
#include"levelscheduler.h"
#include"arrays.h"
#include"timer.h"

using namespace std;


const int ip_cnt = 4;
const int call_times = 100000;
int main()
{
    int *arr = new int[ip_cnt];
    for (int i=0; i<ip_cnt; ++i)
    {
        arr[i] = (i+1)*1000000;
    }

    int size = ip_cnt;
    StepScheduler step;
    step.saveServer(arr, size);
    RandomScheduler rand;
    rand.saveServer(arr, size);

    Timer t;
    for(int i=0; i<call_times; i++)
    {
        step.allocServer();
    }
    cout << t.timeDiff() << "us" << endl;

    for(int i=0; i<call_times; i++)
    {
        rand.allocServer();
    }
    cout << t.timeDiff() << "us" << endl;

    
}


/**
 * scheduler info
 */
/*
int main()
{
//    int arr[] = {4, 3, 2, 1};
    int arr[] = {40000, 30000, 20000, 10000};
//    int arr[] = {400, 300, 200, 100};
//    int arr[] = {40, 30, 20, 10};
    int size = sizeof(arr)/sizeof(int);
    WRRScheduler wrr;
    wrr.saveServer(arr, size);
    StepScheduler step;
    step.saveServer(arr, size);
    RandomScheduler rand;
    rand.saveServer(arr, size);
    LevelScheduler level;
    level.saveServer(arr, size);

    int wrr_sum = 0;
    int step_sum = 0;
    int rand_sum = 0;
    int level_sum = 0;
    int *wrrall_cnt = new int[size]();
    int *wrr_cnt = new int[size]();
    int *stepall_cnt = new int[size]();
    int *step_cnt = new int[size]();
    int *randall_cnt = new int[size]();
    int *rand_cnt = new int[size]();
    int *levelall_cnt = new int[size]();
    int *level_cnt = new int[size]();

    //cout << "num" << "\t" << "wrr" << "\t" << "step" << "\t" << "random" << "\t" << "level" << endl;
    cout << "num" << "\t" << "wrr" << "\t" << "step" << "\t" << "random" << "\t" << "level" ;
    cout << "\t" << "allwrr" << "\t" << "allstep" << "\t" << "allrandom" << "\t" << "alllevel" << endl;
    int TIMES = array_sum(arr, size);
    for (int i = 0; i < TIMES; ++i)
    {
        int wrr_i = wrr.allocServer();
        int step_i = step.allocServer();
        int rand_i = rand.allocServer();
        int level_i = level.allocServer();
        ++wrrall_cnt[wrr_i];
        ++wrr_cnt[wrr_i];
        ++stepall_cnt[step_i];
        ++step_cnt[step_i];
        ++randall_cnt[rand_i];
        ++rand_cnt[rand_i];
        ++levelall_cnt[level_i];
        ++level_cnt[level_i];
        
        if ((i+1)%100 == 0)
        {
            for (int j = 0; j < size; ++j)
            {
                cout << "|" << wrr_cnt[j] ;
                wrr_cnt[j] = 0;
            }
            cout << "|\t";
            for (int j = 0; j < size; ++j)
            {
                cout << "|" << step_cnt[j] ;
                step_cnt[j] = 0;
            }
            cout << "|\t";
            for (int j = 0; j < size; ++j)
            {
                cout << "|" << rand_cnt[j] ;
                rand_cnt[j] = 0;
            }
            cout << "|\t";
            for (int j = 0; j < size; ++j)
            {
                cout << "|" << level_cnt[j] ;
                level_cnt[j] = 0;
            }
            cout << "|" << "\t";

            for (int j = 0; j < size; ++j)
            {
                cout << "|" << wrrall_cnt[j] ;
            }
            cout << "|\t";
            for (int j = 0; j < size; ++j)
            {
                cout << "|" << stepall_cnt[j] ;
            }
            cout << "|\t";
            for (int j = 0; j < size; ++j)
            {
                cout << "|" << randall_cnt[j] ;
            }
            cout << "|\t";
            for (int j = 0; j < size; ++j)
            {
                cout << "|" << levelall_cnt[j] ;
            }
            cout << "|" << endl;

        }
        

        wrr_sum += wrr_i;
        step_sum += step_i;
        rand_sum += rand_i;
        level_sum += level_i;
        //cout << i+1 << "\t" << size - wrr_i << "\t" << size - step_i << "\t" << size - rand_i << "\t" << size - level_i << endl;
    }
    cout << "all_sum:" << "\t" << wrr_sum << "\t" << step_sum << "\t" << rand_sum << "\t" << level_sum << endl;

}

*/

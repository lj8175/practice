#include <stdio.h>    // for printf()  
#include <stdlib.h>   // for exit()
#include <unistd.h>   // for pause()  
#include <signal.h>   // for signal()  
#include <string.h>   // for memset()  
#include <sys/time.h> // struct itimeral. setitimer()  
#include <iostream>
#include <vector>
#include <math.h>
#include <float.h>
#include "timer.h"

using namespace std;

#define handle_error(msg) \
  do { perror(msg); exit(EXIT_FAILURE); } while (0)

const int ip_cnt = 4;
const int call_cnt = 10;
const int rp =    10;

// double 72%
// int    70%
// -O2    16%


void periodic(int sig){
    //int *vec = (int*)malloc(ip_cnt*sizeof(int));
    //int *vec_inc = (int*)malloc(ip_cnt*sizeof(int));
    //int *vec_note = (int*)malloc(ip_cnt*rp*sizeof(int));
    vector<int> vec;
    vector<int> vec_inc;
    vector<int> vec_note;
    int vec_max = ip_cnt - 1;
    
    vector<int> vec_times;
    vector<double> vec_square;
    vector<int> vec_snote;
    vector<double> square_note;
    double all = 0;
    int vec_min = 0;
    double square_min = DBL_MAX;
    

    for(int i = 0; i < ip_cnt; i++)
    {
        //vec[i] = vec_inc[i] = (int)ip_cnt*rp*((i+1)*1.0)/((1+ip_cnt)*ip_cnt/2);
        //vec.push_back((int)ip_cnt*rp*((i+1)*1.0)/((1+ip_cnt)*ip_cnt/2));
        //vec_inc.push_back((int)ip_cnt*rp*((i+1)*1.0)/((1+ip_cnt)*ip_cnt/2));
        vec.push_back(0); //bug : should be zero, not i+1;
        vec_inc.push_back((i+1)*10000);
        vec_times.push_back(0);
        vec_square.push_back(0);
    }

    Timer t;
    // step method
    //for(int i = 0; i < ip_cnt*rp; i++)
    for(int i = 0; i < call_cnt; i++)
    {
        for(int j = 0; j < ip_cnt; j++)
        {
            vec[j] += vec_inc[j];
            if( vec[j] >= vec[vec_max])
            {
                vec_max = j;
            }
        } 
        //vec[vec_max] -= rp*ip_cnt;
        vec[vec_max] -= 100000;
        //vec_note[i] = vec_max;
        vec_note.push_back(vec_max + 1);
    }

    cout << t.timeDiff() << "us" << endl;

    // square method
    for(int i = 0; i < call_cnt; i++)
    {
        for(int j = 0; j < ip_cnt; j++)
        {
            double all_inner = all + 1.0/vec_inc[j];
            vec_times[j]++;
            vec_square[j] = 0;
            for(int k=0; k < ip_cnt; k++)
            {
                vec_square[j] +=pow((1.0*vec_times[k]/vec_inc[k] - all_inner/4), 2);
            }
            vec_times[j]--;
            if(vec_square[j] <= square_min)
            {
                vec_min = j;
                square_min = vec_square[j];
            }
        }
        all += 1.0/vec_inc[vec_min];
        vec_times[vec_min]++;
        vec_snote.push_back(vec_min+1);
        square_note.push_back(square_min);
        square_min = DBL_MAX;
    }

    cout << t.timeDiff() << "us" << endl;
    
    for(int i=0; i < vec_note.size(); i++)
    {
        cout << vec_note[i] << "\t" << vec_snote[i] << "\t" << square_note[i] << "\t" << endl;
    }
}

int main(){
  int res=0;
  struct itimerval tick;
  memset(&tick,0,sizeof(tick));
  tick.it_value.tv_sec=1;
  tick.it_value.tv_usec=0;
  tick.it_interval.tv_sec=1;
  tick.it_interval.tv_usec=0;

  signal(SIGALRM,periodic);
  res=setitimer(ITIMER_REAL,&tick,NULL);
  if(res) {printf("set timer failed!\n");exit(127);}


  while(1)
  {
    pause();
    //cout << "catch a signal" << endl;
  }
 
  return 0;
}

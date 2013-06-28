#ifndef __SAFEMAP__H
#define __SAFEMAP__H
#include<map>
#include<string>
#include<signal.h>
using namespace std;
class SafeMap
{
public:
    SafeMap();
    int tryInsert(string str, int time);
    int write2file(string file);

private:
    map<string, int> _sm;
    pthread_mutex_t _mutex;
    int _call, _succ;
};

#endif

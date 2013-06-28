#include <iostream>
#include <fstream>
#include <stdlib.h>
#include "safemap.h"

SafeMap::SafeMap()
{
    _call=_succ=0;
    if(0!= ::pthread_mutex_init (&_mutex, 0))
    {
        fprintf(stderr, "SafeMap::mutex init failed\n");
        exit(-1);
    }
}

int SafeMap::tryInsert(string str, int time)
{
    _call++;
    if(0!= ::pthread_mutex_trylock(&_mutex))
    {
        return -1;
    }
    map<string, int>::iterator it;
    it = _sm.find(str);
    if(it == _sm.end())
    {
        _sm.insert(make_pair(str, time));
    }
    else
    {
        it->second = time;
    }
    if(0!= ::pthread_mutex_unlock(&_mutex))
    {
        fprintf(stderr, "SafeMap:insert unlock failed\n");
        return -1;
    }
    _succ++;
    return 0;
}

int SafeMap::write2file(string file)
{
    if(file.empty())
    {
        return -1;
    }
    ofstream fout(file.c_str());
    if(!fout.is_open())
    {
        fprintf(stderr, "SafeMap:%s file open failed\n", file.c_str());
        return -1;
    }
    map<string, int>::iterator it;
    if(0!= ::pthread_mutex_lock(&_mutex))
    {
        fprintf(stderr, "SafeMap:write2file lock failed\n");
        return -1;
    }
    for(it=_sm.begin(); it!=_sm.end(); it++)
    {
        fout<<it->first<<"\t"<<it->second<<endl;
    }
    fout<<_call<<"\t"<<_succ<<endl;
    fout.close();
    if(0!= ::pthread_mutex_unlock(&_mutex))
    {
        fprintf(stderr, "SafeMap:write2file unlock failed\n");
        return -1;
    }
    return 0;
}


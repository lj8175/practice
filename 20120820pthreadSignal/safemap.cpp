
class SafeMap
{
public:
    SafeMap()
    {
        if(0!= ::pthread_mutex_init (&_mutex, 0))
        {
            fprintf(stderr, "SafeMap::mutex init failed\n");
        }
    }

    int tryInsert(string str, int time)
    {
        if(0!= ::pthread_mutex_trylock(&_mutex))
        {
            return -1;
        }
        _sm.inset(make_pair(str, time);
        if(0!= ::pthread_mutex_unlock(&_mutex))
        {
            fprintf(stderr, "SafeMap:insert unlock failed\n");
            return -1;
        }
        return 0;
    }

    int write2file(string file)
    {
        if(file.empty())
        {
            return -1;
        }
        ofstream fout(file);
        if(!fout)
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
            fout<<_sm.first<<"\t"<<_sm.second<<endl;
        }
        if(0!= ::pthread_mutex_unlock(&_mutex))
        {
            fprintf(stderr, "SafeMap:write2file unlock failed\n");
            return -1;
        }
        retrun 0;
    }

private:
    map<string, int> _sm;
    pthread_mutex_t _mutex;
};

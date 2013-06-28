#include<iostream>
#include<map>
#include<ext/hash_map>
#include"timer.h"

using namespace std;
using namespace __gnu_cxx;

int main()
{
    hash_map<int, int> hm;
    map<int, int> m;
    Timer t;
    for(int i = 0; i < 10000000; i++)
    {
        hm.insert(make_pair(i, i)); //3148737us
    }
    cout<<t.timeDiff()<<endl;
    for(int i = 0; i < 10000000; i++)
    {
        m.insert(make_pair(i, i));
    }
    cout<<t.timeDiff()<<endl; //23450657us
}

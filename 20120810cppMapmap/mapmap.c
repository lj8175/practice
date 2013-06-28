#include<string>
#include<map>
#include<iostream>
#include<vector>

using namespace std;

class Mapmap
{
public:
  Mapmap():t_call(0), t_suc(0), pkgs(10, 0)
  {
  }

  int insert(string str, int pre, int cnt)
  {
    i = m.find(str);
    if(i == m.end())
    {
      map<int, int> tmp;
      tmp.insert(make_pair(pre,cnt));
      m.insert(make_pair(str, tmp));
    }
    else
    {
      sptr = &i->second;
      j = sptr->find(pre);
      if(j == sptr->end())
      {
        sptr->insert(make_pair(pre,cnt));
      }
      else
      {
        (*j).second += cnt;
      }
    }
  }

void addCall(int func, int cnt)
{
  pkgs[2*func]+=cnt;
}

void addSuc(int func, int cnt)
{
  pkgs[2*func+1]+=cnt;
}
int print()
{
  for(i=m.begin(); i!=m.end(); i++)
  {
    cout<<(*i).first<<":"<<endl;
    for(j=(*i).second.begin(); j!=(*i).second.end(); j++)
    {
      cout<<"\t"<<(*j).first<<":"<<(*j).second<<endl;
    }
  } 
}

void printCall()
{
  for(int i=0; i<pkgs.size(); i+=2)
  {
  cout<<"call:"<<pkgs[i]<<"successful:"<<pkgs[i+1]<<endl;
  }
}

private:
  map<string, map<int, int> > m;
  map<string, map<int, int> >::iterator i;
  map<int, int> *sptr;
  map<int, int>::iterator j;
  int t_call;
  int t_suc;
  vector<int> pkgs;
};

int main()
{
   Mapmap m;
   int i;
   for(i=0; i<10000; i++)
   {
     m.insert(string("10.1.152.140"), 1, 300); 
     m.insert(string("10.1.152.140"), 1, 200); 
     m.insert(string("10.1.152.140"), 2, 100); 
     m.insert(string("10.1.152.141"), 1, 100); 
  }
  m.print();
  m.addCall(0, 100);
  m.addCall(0, 100);
  m.addCall(1, 100);
  m.addCall(2, 200);
  m.addSuc(0, 30);
  m.addSuc(1, 50);
  m.addSuc(4, 80);
  m.printCall();
}
//9s -> 1s

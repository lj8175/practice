#include <string>
#include <iostream>
#include <list>
using namespace std;
int main(){
  list<string> l;
  l.push_back("b");
  l.push_back("c");
  l.push_front("a");

  cout<<*l.begin()<<endl;

  while(!l.empty()){
    cout<<l.front()<<" "<<"size="<<l.size()<<endl;
    l.pop_front();
  }
}

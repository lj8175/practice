#include <iostream>
using namespace std;

class test{
public:
  int num;
};

int main(){
  test t;
  test *t2 = new test;
  test *t3 = new test();
  cout<< t.num << endl;  // alloc on stack, random
  cout<< t2->num << endl; //alloc on heap , gcc -- 0 , dev c++ -- random
  cout<< t3->num << endl; //alloc on heap , gcc -- 0 , dev c++ -- 0
}

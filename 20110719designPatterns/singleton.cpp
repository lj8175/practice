#include <iostream>
using namespace std;

class Singleton{
public:
  static Singleton* getInstance();
  void print();
  void setVar(int i);
private:
  Singleton(){var=3;};
  static Singleton* instance;
  int var;
};

Singleton* Singleton::instance=NULL; //Should be care!

Singleton* Singleton::getInstance(){
  if(instance==NULL){
    instance=new Singleton();
  }
  return instance;
}
void Singleton::print(){
  cout<<"Hello Singleton: "<<var<<endl;
}
void Singleton::setVar(int i){
  var=i;
}

int main(){
  Singleton *s1=Singleton::getInstance();
  Singleton *s2=Singleton::getInstance();
  s1->setVar(5);
  s1->print();
  s2->print();
  return 0;
}

#include <iostream>
using namespace std;

class Baseable{
public:
  virtual void Work()=0;
};

class Base :public Baseable{

private:
  virtual void Work(){
    allocate();
    isAvailable();
  }
  virtual void allocate()=0;
  void isAvailable(){
    cout<<"Base::isAvailable()"<<endl;
  }
};

class A: public Base{
public:
  void Work(){
    allocate();
    isAvailable();
  }
protected:
  void allocate(){
    cout<<"A::allocate()"<<endl;
  }
private:
  void isAvailable(){
    cout<<"A::isAvailable()"<<endl;
  }
};

int main(){
  Baseable* a = new A;
  a->Work();
}

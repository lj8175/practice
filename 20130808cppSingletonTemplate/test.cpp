#include "singleton.h"
#include <iostream>

using namespace std;

class Test
{
public:
    friend class singleton_default<Test>;
    static Test& Instance() { return SINGLETON_REF(Test); }
    void Print()
    {
        cout << "test print" << endl;
    }
private:
    Test(){}
};


int main()
{
    SINGLETON_REF(Test).Print();
    Test::Instance().Print();
}

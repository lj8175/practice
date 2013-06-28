#include <stdio.h>

class A
{
public:
        A()
        {
                Print();
        }
        virtual void Print()
        {
                printf("A is constructed.\n");
        }
};
 
class B: public A
{
public:
        B()
        {
                Print();
        }
 
        virtual void Print()
        {
                printf("B is constructed.\n");
        }
};
 
int main()
{
        A* pA = new B();
        delete pA;
 
        return 0;
}

//A is constructed.
//B is constructed.

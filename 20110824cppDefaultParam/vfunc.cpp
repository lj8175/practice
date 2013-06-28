/*
 *output is "B::Fun with number 11"
 *
 * g++ -g -c vfunc.cpp
 * objdump -S vfunc.o
 */


#include <iostream>

class A
{
public:
    virtual void Fun(int number = 11)
    {
        std::cout << "A::Fun with number " << number << std::endl;
    }
};
 
class B: public A
{
public:
    virtual void Fun(int number = 22)
    {
        std::cout << "B::Fun with number " << number << std::endl;
    }
};
 
int main()
{
    B b;
    A &a = b;
    a.Fun();
}

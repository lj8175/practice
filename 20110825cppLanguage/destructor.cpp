#include <iostream>
 
class A
{
public:
        A()
        {
                std::cout << "A is created." << std::endl;
        }
 
        ~A()
        {
                std::cout << "A is deleted." << std::endl;
        }
};
 
class B : public A
{
public:
        B()
        {
                std::cout << "B is created." << std::endl;
        }
 
        ~B()
        {
                std::cout << "B is deleted." << std::endl;
        }
};
 
int main()
{
        A* pA = new B();
        delete pA;
 
        return 0;
}
//A is created.
//B is created.
//A is deleted.

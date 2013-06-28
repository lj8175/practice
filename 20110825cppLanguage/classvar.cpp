#include <iostream>
 
class A
{
private:
        int n1;
        int n2;
public:
        A(): n2(0), n1(n2 + 2)
        {
        }
 
        void Print()
        {
                std::cout << "n1: " << n1 << ", n2: " << n2 << std::endl;
        }
};
 
int main()
{
        A a;
        a.Print();
 
        return 0;
}

//n1: 32769, n2: 0

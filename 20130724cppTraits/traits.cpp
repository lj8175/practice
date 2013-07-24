#include <iostream>

using namespace std;

class CComplexObject // a demo class
{
public:
    void clone() { cout << "in clone" << endl; }
};

template<bool>  class Traits {};
template<> class Traits<true> {};
template<> class Traits<false> {};



// Solving the problem of choosing method
// To call by inner traits class
template <typename T, bool isClonable>
class XContainer
{
public:
    enum { Clonable = isClonable };

    void clone(T* pObj)
    {
        clone (Traits<isClonable> (), pObj);
    }
   
public:
   
        void clone(const Traits<true> &, T* pObj)
        {
            cout << "before cloning Clonable type" << endl;
            pObj->clone();
            cout << "after cloning Clonable type" << endl;
        }
        void clone(const Traits<false> &, T* pObj)
        {
            cout << "cloning non Clonable type" << endl;
        }
};

// The main program starts here
int main()
{
    int* p1 = 0;
    CComplexObject* p2 = 0;

    XContainer<int, false> n1;
    XContainer<CComplexObject, true> n2;

    n1.clone(p1);
    n2.clone(p2);
   
    return 0;
}

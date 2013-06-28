#include <iostream>
using namespace std;
class A{
public:
	A(){
		cout << "A" <<endl;	
	}	
};

class B{
public:
	B(){
		cout << "B" <<endl;	
	}	
};

class C :public B{
public:
	C(){
		cout << "C" <<endl;	
	}
	A a;
};

int main(){
	C c;	//output B A C
}


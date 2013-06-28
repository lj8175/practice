#include <iostream>
using namespace std;
class ClassA 
{ 
public: 
virtual ~ ClassA(){}; 
virtual void FunctionA(){}; 
};
 
class ClassB 
{ 
public: 
    virtual void FunctionB(){}; 
}; 

class ClassC : public ClassA,public ClassB 
{ 
public: 
}; 
   
ClassC aObject; 
ClassA* pA=&aObject; 
ClassB* pB=&aObject; 
ClassC* pC=&aObject; 

int main(){
	
	cout << pA <<endl;
	cout << pB <<endl;
	cout << pC <<endl;
	
	ClassA* pA2;
	//pA2=static_cast<ClassA*>(pB);  //invalid static_cast from type ¡®ClassB*¡¯ to type ¡®ClassA*'
	/*
  void* pVoid=static_cast<void*>(pB); 
  pA2=static_cast<ClassA*>(pVoid);  //OK
  */ 
  //pA2=pB; //invalid static_cast from type ¡®ClassB*¡¯ to type ¡®ClassA*'
  //pA2=static_cast<ClassA*>(static_cast<ClassC*>(pB)); //OK
  
  delete pA;
  //delete pB;
  //delete pC;  //all core , which safe ?

	
}

/*
0x406020
0x406024
0x406020
*/
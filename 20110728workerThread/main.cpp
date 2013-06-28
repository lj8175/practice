#include <iostream>
#include "componentbase.h"

using namespace std;

int main(){
    int i=0;
    ComponentBase* base = new ComponentBase();

    cout<<"Go to work!" << endl;
    base -> TransStateToExecuting();

    while(1){
        sleep(2);
        cout << "FillThisBuffer() : " << ++i <<endl;
        base -> FillThisBuffer();
    }
}

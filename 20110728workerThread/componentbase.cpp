#include "componentbase.h"
#include <iostream>

using namespace std;

ComponentBase::ComponentBase(){
    bufferwork = new WorkQueue();
}

int ComponentBase::TransStateToExecuting(){
    bufferwork->StartWork(true);
}

void ComponentBase::Work(){
    cout<<"Work: Hello World!" << endl;
}

void ComponentBase::FillThisBuffer(){
    bufferwork->ScheduleWork(this);
}

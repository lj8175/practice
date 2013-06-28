#ifndef __COMPONENTBASE_H
#define __COMPONENTBASE_H

#include "workqueue.h"

class ComponentBase :public WorkableInterface
{
public:
    ComponentBase();

    int TransStateToExecuting();
    void FillThisBuffer();
    
private:
    WorkQueue *bufferwork;

    /* implement WorkableInterface */
    virtual void Work(void);

};

#endif /* __COMPONENTBASE_H */

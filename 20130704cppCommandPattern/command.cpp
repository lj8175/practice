#ifndef __COMMAND_CPP_
#define __COMMAND_CPP_

class Command
{
public:
    virtual ~Command(){};

    virtual void Execute()=0;
protected:
    Command(){};
};

#endif

#ifndef __CONCRETECOMMAND_CPP_
#define __CONCRETECOMMAND_CPP_

#include "command.cpp"
#include "receiver.cpp"
class ConcreteCommand: public Command
{
public:
    ConcreteCommand(Receiver* rec)
    {
        m_rec = rec;
    }

    virtual void Execute()
    {
        m_rec->Action();
    }
private:
    Receiver* m_rec;
};


#endif

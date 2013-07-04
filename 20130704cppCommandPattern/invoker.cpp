#ifndef __INVOKER_CPP_
#define __INVOKER_CPP_

#include "command.cpp"
class Invoker
{
public:
    void SetCommand(Command *cmd)
    {
        m_cmd = cmd;
    }
    void RunCommand()
    {
        m_cmd->Execute();
    }

private:
    Command *m_cmd;
};

#endif

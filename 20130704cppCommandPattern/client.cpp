#include "command.cpp"
#include "concretecommand.cpp"
#include "receiver.cpp"
#include "invoker.cpp"
int main()
{
    Receiver *rec = new Receiver();
    Command *cmd = new ConcreteCommand(rec);
    
    Invoker *inv = new Invoker();
    inv->SetCommand(cmd);
    
    inv->RunCommand();
}

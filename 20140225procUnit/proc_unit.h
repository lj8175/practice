#ifndef PROC_UNIT_H_
#define PROC_UNIT_H_

#include "singleton.h"
#include <map>

using std::map;

class CProcObject
{
public:
    virtual void Run()=0;

protected:
    int m_pid;
};


class CProcUnit
{
public:
    friend class singleton_default<CProcUnit>;
    static CProcUnit& Instance() {return SINGLETON_REF(CProcUnit);}

    int Start(int argc, char* argv[]);

private:
    bool m_bjRun;
    map<int, CProcObject*> m_pobjMap;
    CProcUnit(){}
    int DealCmdOpt(int argc, char* argv[]);
};





#endif /* PROC_UNIT_H_ */

#ifndef PROC_UNIT_H_
#define PROC_UNIT_H_

#include "singleton.h"
#include <map>

using std::map;

class CProcObject
{
public:
    virtual void Run()=0;
};


class CProcUnit
{
public:
    friend class singleton_default<CProcUnit>;
    static CProcUnit& Instance() {return SINGLETON_REF(CProcUnit);}
    void AddProcObject(CProcObject *obj)
    {
        m_procMap[obj] = 0;
    }

    int Start(int argc, char* argv[]);

private:
    map<CProcObject*, int> m_procMap;
    CProcUnit(){}
    int DealCmdOpt(int argc, char* argv[]);
    static void SignalHandler(int signo);
    static bool m_stoped;
    void SetupSignal();
    void StartProcs();
    void MonitorProcs();
    void KillProcs();
};





#endif /* PROC_UNIT_H_ */

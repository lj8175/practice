#ifndef PROC_UNIT_H_
#define PROC_UNIT_H_

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
    static CProcUnit* Instance() {
        if(!m_instance) m_instance = new CProcUnit();
        return m_instance;
    }
    void AddProcObject(CProcObject *obj)
    {
        m_procMap[obj] = 0;
    }

    int Start(int argc, char* argv[]);

private:
    static CProcUnit* m_instance;
    map<CProcObject*, int> m_procMap;
    CProcUnit(){}
    int DealCmdOpt(int argc, char* argv[]);
    static void SignalHandler(int signo);
    static bool m_stoped;
    static bool m_isUnit;
    void SetupSignal();
    void StartProcs();
    void MonitorProcs();
    void KillProcs();
};





#endif /* PROC_UNIT_H_ */

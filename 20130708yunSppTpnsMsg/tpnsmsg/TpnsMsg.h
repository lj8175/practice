#ifndef __TPNS_MSG_BASE_H_
#define __TPNS_MSG_BASE_H_

#include <stdint.h>

#define CMD_LIMIT 256
#define HEADER_LEN 11

class CTpnsMsgBase
{
public:
    virtual ~CTpnsMsgBase() {}
    virtual CTpnsMsgBase* Clone() = 0;
    virtual int Unpack(uint8_t *buf, size_t len) = 0;
    virtual int Process() = 0;
};

class CTpnsMsgManager
{
public:
    CTpnsMsgManager();
    ~CTpnsMsgManager();
    
    void Register(uint8_t cmd, CTpnsMsgBase* msg);
    void UnRegister(uint8_t cmd);

    CTpnsMsgBase* GetMsg(uint8_t *buf, size_t len);

protected:
    int GetCmd(uint8_t *buf, size_t len, uint8_t& cmd);
    CTpnsMsgBase* BuildMsg(uint8_t cmd, uint8_t *buf, size_t len);
    
private:
    CTpnsMsgBase* m_msgMap[CMD_LIMIT];
};

#endif

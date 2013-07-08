#include <strings.h>
#include <stddef.h>

#include "TpnsMsg.h"

CTpnsMsgManager::CTpnsMsgManager()
{
    bzero(m_msgMap, sizeof(m_msgMap));
}

CTpnsMsgManager::~CTpnsMsgManager()
{
    for(int i = 0; i < CMD_LIMIT; ++i)
    {
        UnRegister(i);
    }
}

void CTpnsMsgManager::Register(uint8_t cmd, CTpnsMsgBase* pPrototype)
{
    UnRegister(cmd);
    m_msgMap[cmd] = pPrototype;
}

void CTpnsMsgManager::UnRegister(uint8_t cmd)
{
    CTpnsMsgBase* pPrototype = m_msgMap[cmd];
    if(NULL != pPrototype)
    {
        delete pPrototype;
    }
}

CTpnsMsgBase* CTpnsMsgManager::GetMsg(uint8_t *buf, size_t len)
{
    if(HEADER_LEN > len)
    {
        return NULL;
    }
    uint8_t cmd;
    int ret = GetCmd(buf, len, cmd);
    CTpnsMsgBase* pMsg = BuildMsg(cmd, buf + HEADER_LEN, len - HEADER_LEN);
    return pMsg;
}

int CTpnsMsgManager::GetCmd(uint8_t *buf, size_t len, uint8_t& cmd)
{
    cmd = buf[1]; 
    return 0;
}

CTpnsMsgBase* CTpnsMsgManager::BuildMsg(uint8_t cmd, uint8_t *buf, size_t len)
{
    CTpnsMsgBase* pPrototype = m_msgMap[cmd];
    if(NULL == pPrototype)
    {
        return NULL;
    }
    CTpnsMsgBase* pMsg = pPrototype->Clone();
    int ret = pMsg->Unpack(buf, len);
    if (ret) 
    {
        return NULL;
    }
    return pMsg;
}

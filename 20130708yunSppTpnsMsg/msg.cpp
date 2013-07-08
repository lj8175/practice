#include "msg.h"
#include "tpns/tpns.h"

CTpnsMsgBase* CMsg::Clone()
{
    return new CMsg;
}

int CMsg::Unpack(uint8_t *buf, size_t len)
{
    int ret = struct_TpnsPushReq_unpack((uint8_t *)buf, (int32_t*)&len, &m_tpr, NULL);
    return ret;
}

int CMsg::Process()
{
    return STATE_ID_GET;
}

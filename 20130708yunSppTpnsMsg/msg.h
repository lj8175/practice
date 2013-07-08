#ifndef __MSG_H__
#define __MSG_H__
#include "MsgBase.h"
#include "tpnsmsg/TpnsMsg.h"
#include "tpns.h"
#include <string>

using std::string;
using namespace TPNS_PUSH_PROTOCOL;

USING_ASYNCFRAME_NS;

#define STATE_ID_FINISHED   0
#define STATE_ID_GET        1

#define BASE_BUFF_SIZE 256

class CMsg
    : public CMsgBase, public CTpnsMsgBase
{
    public:
        string key;
        string data;
        int seq;
        int bid;
        int cache_op;

    TpnsPushReq m_tpr;

    virtual CTpnsMsgBase* Clone();
    virtual int Unpack(uint8_t *buf, size_t len);
    virtual int Process();
};


#endif

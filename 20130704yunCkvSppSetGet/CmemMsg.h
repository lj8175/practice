#ifndef __MSG_H__
#define __MSG_H__
#include "MsgBase.h"
#include <string>

using std::string;

USING_ASYNCFRAME_NS;

#define STATE_ID_FINISHED   0
#define STATE_ID_SET        1
#define STATE_ID_GET        2

#define BASE_BUFF_SIZE 256

class CmemMsg
    : public CMsgBase
{
    public:
        int m_setBid;
        string m_setKey;
        string m_setData;

        int m_getBid;
        string m_getKey;
        string m_getData;

        //do not need care
        int m_setSeq;
        int m_getSeq;
};


#endif

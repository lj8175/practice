#ifndef __MSG_H__
#define __MSG_H__
#include "MsgBase.h"
#include <string>

using std::string;

USING_ASYNCFRAME_NS;

#define STATE_ID_FINISHED   0
#define STATE_ID_GET        1

#define BASE_BUFF_SIZE 256

class CMsg
    : public CMsgBase
{
    public:
        string key;
        string data;
        int seq;
        int bid;
        int cache_op;
};


#endif

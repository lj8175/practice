#ifndef __CMEMGETSTATE_H__
#define __CMEMGETSTATE_H__
#include "IState.h"

USING_ASYNCFRAME_NS;

class CmemGetState
    : public IState
{
    public:
        virtual int HandleEncode(CAsyncFrame *pFrame,
                CActionSet *pActionSet,
                CMsgBase *pMsg) ;

        virtual int HandleProcess(CAsyncFrame *pFrame,
                CActionSet *pActionSet,
                CMsgBase *pMsg) ;
};

#endif

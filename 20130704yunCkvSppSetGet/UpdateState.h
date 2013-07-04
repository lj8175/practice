#ifndef __UPDATESTATE_H__
#define __UPDATESTATE_H__
#include "IState.h"

USING_ASYNCFRAME_NS;

class CUpdateState
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

#ifndef __GETSTATE_H__
#define __GETSTATE_H__
#include "IState.h"

USING_ASYNCFRAME_NS;

class CGetState
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

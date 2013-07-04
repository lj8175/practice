#ifndef __CMEMSETSTATE_H__
#define __CMEMSETSTATE_H__
#include "IState.h"

USING_ASYNCFRAME_NS;

class CmemSetState
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

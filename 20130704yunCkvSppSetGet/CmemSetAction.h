#ifndef __CMEMSETACTION_H_
#define __CMEMSETACTION_H_

#include "IAction.h"

USING_ASYNCFRAME_NS;

class CmemSetAction
: public IAction
{
    public:
        // 请求打包
        virtual int HandleEncode( CAsyncFrame *pFrame,
                char *buf,
                int &len,
                CMsgBase *pMsg);

        // 回应包完整性检查
        virtual int HandleInput( CAsyncFrame *pFrame,
            const char *buf,
            int len,
            CMsgBase *pMsg);

        // 回应包处理
        virtual int HandleProcess( CAsyncFrame *pFrame,
            const char *buf,
            int len,
            CMsgBase *pMsg);


        virtual int HandleError( CAsyncFrame *pFrame,
                int err_no,
                CMsgBase *pMsg);
};

#endif

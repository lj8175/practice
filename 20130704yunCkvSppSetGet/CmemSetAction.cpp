#include "trmem_cache_lib.h"
#include "CmemSetAction.h"
#include "CmemMsg.h"
#include "sppincl.h"

using namespace ssdasn;

// 请求打包
int CmemSetAction::HandleEncode( CAsyncFrame *pFrame,
        char *buf,
        int &len,
        CMsgBase *pMsg)
{
    CmemMsg *msg = (CmemMsg*)pMsg;
    int base_size = BASE_BUFF_SIZE;
    msg->m_setSeq = 1;
    base_size += msg->m_setData.size();
    base_size += msg->m_setKey.size();
    if(base_size > MAX_PACKET_SIZE)
    {
        return -1;
    }
    if (len < base_size)
    {
        return base_size;
    }

    char *pkt_ptr = NULL;
    int pkt_len = 0;
    TReqExt req;
    int ret = trmem_encode(msg->m_setBid, cache_op_set, msg->m_setSeq, msg->m_setKey, msg->m_setData, buf, len, pkt_ptr, pkt_len, -1, 0, 0, -1, &req);
    if (ret)
    {
        return -2;
    }
    len = pkt_len;
    memcpy(buf, pkt_ptr, pkt_len);
    return 0;
}

// 回应包完整性检查
int CmemSetAction::HandleInput( CAsyncFrame *pFrame,
        const char *buf,
        int len,
        CMsgBase *pMsg)
{
    int ret = trmem_pkt_complete((char *)buf, len);
    pFrame->FRAME_LOG( LOG_DEBUG, "CmemSetAction::HandleInput. buf len: %d\n", len);
    return ret;
}

// 回应包处理
int CmemSetAction::HandleProcess( CAsyncFrame *pFrame,
        const char *buf,
        int len,
        CMsgBase *pMsg)
{
    CmemMsg *msg = (CmemMsg*)pMsg;
    int retcode,ret_seq,ret_bid,op;
    string ret_key, ret_data;
    int cos;
    int ret = trmem_decode((char *)buf, len, ret_bid, op, ret_seq, ret_key,ret_data, retcode, cos, NULL);
    if (ret<0)
    {
        pFrame->FRAME_LOG( LOG_ERROR, "CmemSetAction::HandleProcess decode error");
        return ret;
    }
    if (ret_seq != msg->m_setSeq)
    {
        pFrame->FRAME_LOG( LOG_ERROR, "CmemSetAction::HandleProcess seq not match");
        return -1;
    }
    if (retcode)
    {
        pFrame->FRAME_LOG( LOG_ERROR, "CmemSetAction::HandleProcess return not succ :%d  ", retcode);
    }
    return retcode;
}


int CmemSetAction::HandleError( CAsyncFrame *pFrame,
        int err_no,
        CMsgBase *pMsg)
{
    pFrame->FRAME_LOG( LOG_ERROR, "CmemSetAction::HandleError. errno: %d\n", err_no);

    return 0;
}

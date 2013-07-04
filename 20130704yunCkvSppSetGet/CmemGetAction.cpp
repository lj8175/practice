#include "trmem_cache_lib.h"
#include "CmemGetAction.h"
#include "CmemMsg.h"
#include "sppincl.h"

using namespace ssdasn;

// 请求打包
int CmemGetAction::HandleEncode( CAsyncFrame *pFrame,
        char *buf,
        int &len,
        CMsgBase *pMsg)
{
    CmemMsg *msg = (CmemMsg*)pMsg;
    int base_size = BASE_BUFF_SIZE;
    msg->m_getSeq = 2;
    base_size += msg->m_getData.size();
    base_size += msg->m_getKey.size();
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
    int ret = trmem_encode(msg->m_getBid, cache_op_get, msg->m_getSeq, msg->m_getKey, msg->m_getData, buf, len, pkt_ptr, pkt_len, -1, 0, 0, -1, &req);
    if (ret)
    {
        return -2;
    }
    len = pkt_len;
    memcpy(buf, pkt_ptr, pkt_len);
    return 0;
}

// 回应包完整性检查
int CmemGetAction::HandleInput( CAsyncFrame *pFrame,
        const char *buf,
        int len,
        CMsgBase *pMsg)
{
    int ret = trmem_pkt_complete((char *)buf, len);
    pFrame->FRAME_LOG( LOG_DEBUG, "CmemGetAction::HandleInput. buf len: %d\n", len);
    return ret;
}

// 回应包处理
int CmemGetAction::HandleProcess( CAsyncFrame *pFrame,
        const char *buf,
        int len,
        CMsgBase *pMsg)
{
    CmemMsg *msg = (CmemMsg*)pMsg;
    int retcode,ret_seq,ret_bid,op;
    string ret_key, ret_data;
    int cos;
    int ret = trmem_decode((char *)buf, len, ret_bid, op, ret_seq, ret_key, ret_data, retcode, cos, NULL);
    if (ret<0)
    {
        pFrame->FRAME_LOG( LOG_ERROR, "CmemGetAction::HandleProcess decode error");
        return ret;
    }
    if (ret_seq != msg->m_getSeq)
    {
        pFrame->FRAME_LOG( LOG_ERROR, "CmemGetAction::HandleProcess seq not match");
        return -1;
    }
    if (retcode)
    {
        pFrame->FRAME_LOG( LOG_ERROR, "CmemGetAction::HandleProcess return not successful : %d  ", retcode);
    }
    else
    {
        msg->m_getData = ret_data;
    }
    pFrame->FRAME_LOG( LOG_DEBUG, "CmemGetAction::HandleProcess ret_data:%s  ", ret_data.c_str());
    return retcode;
}


int CmemGetAction::HandleError( CAsyncFrame *pFrame,
        int err_no,
        CMsgBase *pMsg)
{
    pFrame->FRAME_LOG( LOG_ERROR, "CmemGetAction::HandleError. errno: %d\n", err_no);

    return 0;
}

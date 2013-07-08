#include "trmem_cache_lib.h"
#include "GetInfo.h"
#include "msg.h"
#include "sppincl.h"

using namespace ssdasn;

// 请求打包
int CGetInfo::HandleEncode( CAsyncFrame *pFrame,
        char *buf,
        int &len,
        CMsgBase *pMsg)
{
    CMsg *msg = (CMsg*)pMsg;
    int base_size = BASE_BUFF_SIZE;
    base_size += msg->data.size();
    base_size += msg->key.size();
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
    int ret = trmem_encode(101020276, cache_op_set, msg->seq, msg->key, msg->data, buf, len, pkt_ptr, pkt_len, -1, 0, 0, -1, &req);
    if (ret)
    {
        return -2;
    }
    len = pkt_len;
    memcpy(buf, pkt_ptr, pkt_len);
    return 0;
}

// 回应包完整性检查
int CGetInfo::HandleInput( CAsyncFrame *pFrame,
        const char *buf,
        int len,
        CMsgBase *pMsg)
{
    int ret = trmem_pkt_complete((char *)buf, len);
    pFrame->FRAME_LOG( LOG_DEBUG, "CGetInfo::HandleInput. buf len: %d\n", len);
    return ret;
}

// 回应包处理
int CGetInfo::HandleProcess( CAsyncFrame *pFrame,
        const char *buf,
        int len,
        CMsgBase *pMsg)
{
    CMsg *msg = (CMsg*)pMsg;
    int retcode,ret_seq,ret_bid,op;
    string ret_key, ret_data;
    int cos;
    int ret = trmem_decode((char *)buf, len, ret_bid, op, ret_seq, ret_key,ret_data, retcode, cos, NULL);
    if (ret<0)
    {
        pFrame->FRAME_LOG( LOG_ERROR, "CGetInfo::HandleProcess decode error");
        return ret;
    }
    if ((ret_seq != msg->seq) || (retcode))
    {
        pFrame->FRAME_LOG( LOG_ERROR, "CGetInfo::HandleProcess seq not match");
        return -1;
    }
    if (retcode)
    {
        pFrame->FRAME_LOG( LOG_ERROR, "CGetInfo::HandleProcess return not successful");
    }
    return retcode;
}


int CGetInfo::HandleError( CAsyncFrame *pFrame,
        int err_no,
        CMsgBase *pMsg)
{
    pFrame->FRAME_LOG( LOG_ERROR, "CGetInfo::HandleError. errno: %d\n", err_no);

    return 0;
}

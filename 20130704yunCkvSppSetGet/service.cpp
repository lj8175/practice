//必须包含spp的头文件
#include "sppincl.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "CmemMsg.h"
#include "CmemSetState.h"
#include "CmemGetState.h"

#define MODULE_PROC_NUM	"module_proc_num"

char *format_time( time_t tm);


int Init(CAsyncFrame* pFrame, CMsgBase* pMsg)
{
    //CAsyncFrame *pFrame = (CAsyncFrame *)arg1;
    //CmemMsg *msg = (CmemMsg *) arg2;
    return STATE_ID_SET;
}

int Fini(CAsyncFrame* pFrame, CMsgBase* pMsg)
{
    //CAsyncFrame *pFrame = (CAsyncFrame *)arg1;
    CmemMsg *msg = (CmemMsg *) pMsg;

    std::string info;
    pMsg->GetDetailInfo(info);
    pFrame->FRAME_LOG( LOG_DEBUG, "%s\n", info.c_str()); 

    blob_type rspblob;
	rspblob.data = (char *)msg->m_getData.c_str();
	rspblob.len = msg->m_getData.size();
    pFrame->FRAME_LOG( LOG_DEBUG, "%s\n", msg->m_getData.c_str());
	pMsg->SendToClient(rspblob);

    return 0;
}

int OverloadProcess(CAsyncFrame* pFrame, CMsgBase* pMsg)
{
    //CAsyncFrame *pFrame = (CAsyncFrame *)arg1;
    //CmemMsg *msg = (CmemMsg *) pMsg;

    pFrame->FRAME_LOG( LOG_DEBUG, "Overload.\n" );

    blob_type rspblob;
	rspblob.data = "Overload";
	rspblob.len = sizeof("Overload");
	pMsg->SendToClient(rspblob);

    return 0;
}
//初始化方法（可选实现）
//arg1:	配置文件
//arg2:	服务器容器对象
//返回0成功，非0失败
extern "C" int spp_handle_init(void* arg1, void* arg2)
{
    //插件自身的配置文件
    const char* etc = (const char*)arg1;
    //服务器容器对象
    CServerBase* base = (CServerBase*)arg2;
    base->log_.LOG_P(LOG_DEBUG, "spp_handle_init, %s, %d\n", etc, base->servertype());
    
    if( base->servertype() == SERVER_TYPE_WORKER )
    {
        CAsyncFrame::Instance()->InitFrame2(base, 100, 0); 

        CAsyncFrame::Instance()->RegCallback(CAsyncFrame::CBType_Init, Init);
        CAsyncFrame::Instance()->RegCallback(CAsyncFrame::CBType_Fini, Fini);
        CAsyncFrame::Instance()->RegCallback(CAsyncFrame::CBType_Overload, OverloadProcess);

        CAsyncFrame::Instance()->AddState(STATE_ID_SET, new CmemSetState);
        CAsyncFrame::Instance()->AddState(STATE_ID_GET, new CmemGetState);
    }

    return 0;
}
//数据接收（必须实现）
//flow:	请求包标志
//arg1:	数据块对象
//arg2:	服务器容器对象
//返回正数表示数据已经接收完整且该值表示数据包的长度，0值表示数据包还未接收完整，负数表示出错
extern "C" int spp_handle_input(unsigned flow, void* arg1, void* arg2)
{
    //数据块对象，结构请参考tcommu.h
    blob_type* blob = (blob_type*)arg1;
    //extinfo有扩展信息
    TConnExtInfo* extinfo = (TConnExtInfo*)blob->extdata;
    //服务器容器对象
    CServerBase* base = (CServerBase*)arg2;
    base->log_.LOG_P(LOG_DEBUG, "spp_handle_input, %d, %d, %s, %s\n", 
            flow, 
            blob->len, 
            inet_ntoa(*(struct in_addr*)&extinfo->remoteip_), 
            format_time(extinfo->recvtime_));

    if( blob->len >= 0 )
    {
        return blob->len;
    }
    return 0;
}
//路由选择（可选实现）
//flow:	请求包标志
//arg1:	数据块对象
//arg2:	服务器容器对象
//返回值表示worker的组号

extern "C" int spp_handle_route(unsigned flow, void* arg1, void* arg2)
{
    //服务器容器对象
    CServerBase* base = (CServerBase*)arg2;
    base->log_.LOG_P(LOG_DEBUG, "spp_handle_route, %d\n", flow);
    //int route_no = 2;
    //return GROUPID(route_no);
    return 1;
}

//数据处理（必须实现）
//flow:	请求包标志
//arg1:	数据块对象
//arg2:	服务器容器对象
//返回0表示成功，非0失败
extern "C" int spp_handle_process(unsigned flow, void* arg1, void* arg2)
{
    //数据块对象，结构请参考tcommu.h
    blob_type* blob = (blob_type*)arg1;
    TConnExtInfo* extinfo = (TConnExtInfo*)blob->extdata;

    //服务器容器对象
    CServerBase* base = (CServerBase*)arg2;
    //数据来源的通讯组件对象
    CTCommu* commu = (CTCommu*)blob->owner;
   
    //int ret = 0;

	base->log_.LOG_P_PID(LOG_DEBUG, "spp_handle_process, %d, %d, %s, %s\n", 
            flow, 
            blob->len, 
            inet_ntoa(*(struct in_addr*)&extinfo->remoteip_),
            format_time(extinfo->recvtime_));

    CmemMsg *msg = new CmemMsg;
    msg->SetServerBase(base);
    msg->SetTCommu(commu);
    msg->SetFlow(flow);
    msg->SetInfoFlag(true);
    msg->SetMsgTimeout(0);
    msg->m_setBid = 101020276;
    msg->m_getBid = 101020276;
    msg->m_setKey = "key";
    msg->m_getKey = "key";
    msg->m_setData = string(blob->data,blob->len);
    CAsyncFrame::Instance()->Process( msg );

    return 0;
}
//析构资源（可选实现）
//arg1:	保留参数
//arg2:	服务器容器对象
extern "C" void spp_handle_fini(void* arg1, void* arg2)
{
    //服务器容器对象
    CServerBase* base = (CServerBase*)arg2;
    base->log_.LOG_P(LOG_DEBUG, "spp_handle_fini\n");

    if( base->servertype() == SERVER_TYPE_WORKER )
    {
        CAsyncFrame::Instance()->FiniFrame(); 
        CAsyncFrame::Destroy();
    }
}

char *format_time( time_t tm)
{
    static char str_tm[1024];
    struct tm tmm;
    memset(&tmm, 0, sizeof(tmm) );
    localtime_r((time_t *)&tm, &tmm);

    snprintf(str_tm, sizeof(str_tm), "[%04d-%02d-%02d %02d:%02d:%02d]",
            tmm.tm_year + 1900, tmm.tm_mon + 1, tmm.tm_mday,
            tmm.tm_hour, tmm.tm_min, tmm.tm_sec);

    return str_tm;
}

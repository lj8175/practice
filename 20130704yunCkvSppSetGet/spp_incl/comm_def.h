#ifndef _COMM_INFO_H
#define _COMM_INFO_H

#define PWD_BUF_LEN 256
#define TOKEN_BUF_LEN 300

//proxy statobj
#define ATOR_RECV_NUM   "ator_recv_num"
#define CTOR_RECV_NUM   "ctor_recv_num"
#define FAIL_ATOR_RECV  "fail_ator_recv"
#define FAIL_CTOR_RECV  "fail_ctor_recv"
#define PROXY_PID   "proxy_pid"
#define PROXY_FD    "proxy_fd"
#define PROXY_MEM   "proxy_mem"
#define PROXY_LOAD  "proxy_load"
#define PROXY_SRATE "proxy_srate"
#define PROXY_BRATE "proxy_brate"
#define PROXY_OVERLOAD "proxy_overload"

//worker statobj
//#define ATOR_RECV_NUM   "ator_recv_num"
#define ATOR_SEND_NUM   "ator_send_num"
#define AVG_PROC_TIME   "avg_proc_time"
#define MAX_PROC_TIME   "max_proc_time"
#define FAIL_PROC_NUM   "fail_proc_num"
#define BACK_PAG_LEN    "back_pag_len"  
#define MIN_PROC_TIME   "min_proc_time" 
#define FAIL_ATOR_SEND  "fail_ator_send"
#define WORKER_PID      "worker_pid"    
#define WORKER_FD       "worker_fd"     
#define WORKER_MEM      "worker_mem"    
#define WORKER_SRATE    "worker_srate"  
#define WORKER_BRATE    "worker_brate"  
#define WORKER_DELAY    "worker_delay"  

//ctrl statobj
#define NOTIFY_NUM   "notify_num"
//#define PROXY_PID   "proxy_pid"
//#define PROXY_FD    "proxy_fd"
//#define PROXY_MEM   "proxy_mem"
//#define PROXY_LOAD  "proxy_load"
//#define PROXY_SRATE "proxy_srate"
//#define PROXY_BRATE "proxy_brate"

namespace spp
{
    enum dll_mode {
        spp_mode = 0,
        sb_mode
    };

    enum CoreCheckPoint
    {
        CoreCheckPoint_SppFrame         = 0,
        CoreCheckPoint_HandleProcess    = 1,
    };

	namespace statdef
	{
		//add by aoxu, 2010-02-09
		//统计项索引，快速访问StatObj的数组下标
		typedef enum DefaultProxyStatObjIndex
		{
			PIDX_ATOR_RECV_NUM = 0,
			PIDX_CTOR_RECV_NUM,         //1
			PIDX_FAIL_ATOR_RECV,        //2
			PIDX_FAIL_CTOR_RECV,        //3
			PIDX_PROXY_PID,             //4
			PIDX_PROXY_FD,              //5
			PIDX_PROXY_MEM,             //6
			PIDX_PROXY_LOAD,            //7
			PIDX_PROXY_SRATE,           //8
			PIDX_PROXY_BRATE,           //9
			PIDX_PROXY_OVERLOAD         //10
		} proxy_stat_index;

		typedef enum DefaultWorkerStatObjIndex
		{
			WIDX_ATOR_RECV_NUM = 0,
			WIDX_ATOR_SEND_NUM,         //1
			WIDX_AVG_PROC_TIME,         //2
			WIDX_MAX_PROC_TIME,         //3
			WIDX_FAIL_PROC_NUM,         //4
			WIDX_BACK_PAG_LEN,          //5
			WIDX_MIN_PROC_TIME,         //6
			WIDX_FAIL_ATOR_SEND,        //7
			WIDX_WORKER_PID,            //8
			WIDX_WORKER_FD,             //9
			WIDX_WORKER_MEM,            //10
			WIDX_WORKER_SRATE,          //11
			WIDX_WORKER_BRATE,          //12
			WIDX_WORKER_DELAY           //13
		} worker_stat_index;

		typedef enum DefaultCtrlStatObjIndex
		{
			CIDX_NOTIFY_NUM = 0,
			CIDX_PROXY_PID,             //1
			CIDX_PROXY_FD,              //2
			CIDX_PROXY_MEM,             //3
			CIDX_PROXY_LOAD,            //4
			CIDX_PROXY_SRATE,           //5
			CIDX_PROXY_BRATE            //6
		} ctrl_stat_index;
	}
}

#endif


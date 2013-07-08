#ifndef __TPNS_HEADER_H_
#define __TPNS_HEADER_H_

#include <stdint.h>

#pragma pack(1)
enum TPNS_CMD   
{
    CMD_PUSH = 0,        //推送消息至客户端
    CMD_PULLCONFIG = 1,      //拉取配置
    CMD_REGISTER = 2,        //客户端注册
    CMD_UNREGISTER = 3,      //客户端注销
    CMD_RECONNECT = 4,       //客户端掉线重连
    CMD_HEARTBEAT = 5,       //客户端心跳
    CMD_SPEEDTEST = 6,       //客户端测速上报
    CMD_UNINSTALL = 7,       //客户端卸载反馈
};

enum TPNS_TYPE
{
    TYPE_REQ = 0,       //请求包
    TYPE_RSP = 1,           //回应包
};

typedef struct TpnsHeaderStruct
{
    uint8_t version;
    uint8_t cmd:7;
    uint8_t type:1; //最高位 0请求包 1响应包
} TpnsHeader;

typedef struct TpnsContentStruct
{
    uint32_t length;
    uint32_t seq;
    uint8_t rspCode;
} TpnsContent;

#pragma pack()

#endif

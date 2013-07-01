#include <stdint.h>

#pragma pack(1)
enum TPNS_CMD   
{
       CMD_PUSH,        //推送消息至客户端
       CMD_PULLCONFIG,      //拉取配置
       CMD_REGISTER,        //客户端注册
       CMD_UNREGISTER,      //客户端注销
       CMD_RECONNECT,       //客户端掉线重连
       CMD_HEARTBEAT,       //客户端心跳
       CMD_SPEEDTEST,       //客户端测速上报
       CMD_UNINSTALL,       //客户端卸载反馈
};
struct TpnsCommonHeaderStruct
{
    uint8_t version;
    uint8_t cmd:7;
    uint8_t flag:1; //最高位 0请求包 1响应包
};
typedef struct TpnsCommonHeaderStruct TpnsCommonHeader;

struct TpnsHeaderStruct
{
    uint32_t length;
    uint32_t seq;
    uint8_t rspCode;
};
typedef struct TpnsHeaderStruct TpnsHeader;



#pragma pack()

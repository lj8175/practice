#include <stdint.h>

#pragma pack(1)
struct TpnsCommonHeaderStruct
{
    uint8_t version;
    uint8_t cmd:7;
    uint8_t flag:1; //最高位 0请求包 1响应包
    uint16_t length;
};
typedef struct TpnsCommonHeaderStruct TpnsCommonHeader;

struct TpnsReqHeaderStruct
{
    uint32_t seq;
};
typedef struct TpnsReqHeaderStruct TpnsReqHeader;

struct TpnsRspHeaderStruct
{
    uint32_t seq;
    uint8_t rspCode;
};
typedef struct TpnsRspHeaderStruct TpnsRspHeader;
#pragma pack()

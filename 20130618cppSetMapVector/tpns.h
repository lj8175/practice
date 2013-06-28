#include <stdint.h>

#pragma pack(1)
struct TpnsCommonHeader_Struct
{
    uint8_t version;
    uint8_t cmd:7;
    uint8_t flag:1; //最高位 0请求包 1响应包
    uint16_t length;
};
typedef struct TpnsCommonHeader_Struct TpnsCommonHeader;
#pragma pack()

#ifndef __m3u8_monitor__AmfData__
#define __m3u8_monitor__AmfData__

#include "Utils.h"
#include <stdio.h>
#include <stdint.h>
#include <iostream>


typedef enum {
    AMF_NUMBER = 0, AMF_BOOLEAN, AMF_STRING, AMF_OBJECT,
    AMF_MOVIECLIP,		/* reserved, not used */
    AMF_NULL, AMF_UNDEFINED, AMF_REFERENCE, AMF_ECMA_ARRAY, AMF_OBJECT_END,
    AMF_STRICT_ARRAY, AMF_DATE, AMF_LONG_STRING, AMF_UNSUPPORTED,
    AMF_RECORDSET,		/* reserved, not used */
    AMF_XML_DOC, AMF_TYPED_OBJECT,
    AMF_AVMPLUS,		/* switch to AMF3 */
    AMF_INVALID = 0xff
}AMFDataType;

//enum AmfType {
//    AMF_NUMBER = 0, AMF_BOOLEAN, AMF_STRING, AMF_OBJECT,
//    AMF_MOVIECLIP,		/* reserved, not used */
//    AMF_NULL, AMF_UNDEFINED, AMF_REFERENCE, AMF_ECMA_ARRAY, AMF_OBJECT_END,
//    AMF_STRICT_ARRAY, AMF_DATE, AMF_LONG_STRING, AMF_UNSUPPORTED,
//    AMF_RECORDSET,		/* reserved, not used */
//    AMF_XML_DOC, AMF_TYPED_OBJECT,
//    AMF_AVMPLUS,		/* switch to AMF3 */
//    AMF_INVALID = 0xff
//};

class AmfObject;


class AmfData {
public:
    
    uint8_t *name; //属性名称
    
    uint8_t type; //Amf数据类型
    uint32_t dataLen; //数据长度
    
    union {
        bool bVal;
        uint32_t iVal;
        double fVal;
        AmfObject *oVal;
        uint8_t *sVal;
    }val;
    
    AmfData();
    ~AmfData();
    
    AmfData &operator = (AmfData &src);
    
    int amf_data_set(char *name, uint8_t type, void *val);
    
    uint8_t *get_val_string();
    double get_val_double();
    AmfObject *get_val_object();
    
    int amf_data_decode(uint8_t *buf, uint32_t len, bool hasName = false);
    
    static uint8_t *amf_encode_string(uint8_t *buf, uint8_t *str, uint32_t bufLen, uint32_t strLen);
    static uint8_t *amf_decode_string(uint8_t *strBuf, uint8_t *buf, uint32_t strBufLen, uint32_t bufLen, bool isLongStr = false);
    static uint8_t *amf_encode_name_string(uint8_t *buf, uint8_t *key, uint32_t keyLen, uint8_t *str, uint32_t bufLen, uint32_t strLen);
    
    static uint8_t *amf_encode_boolean(uint8_t *buf, bool val, uint32_t bufLen);
    static uint8_t *amf_encode_double(uint8_t *buf, double val, uint32_t bufLen);
    static uint8_t *amf_encode_int16(uint8_t *buf, uint16_t val, uint32_t bufLen);
    static uint8_t *amf_encode_int32(uint8_t *buf, uint32_t val, uint32_t bufLen);
    
    static bool amf_decode_boolean(uint8_t *buf, uint32_t bufLen);
    static double amf_decode_double(uint8_t *buf, uint32_t len);
    static uint16_t amf_decode_int16(uint8_t *buf, uint32_t bufLen);
    static uint32_t amf_decode_int24(uint8_t *buf, uint32_t bufLen);
    static uint32_t amf_decode_int32(uint8_t *buf, uint32_t bufLen);
};


class AmfObject {

public:
    uint32_t proCount;
#define AMF_OBJ_MAX_PROPERTY 256
    AmfData *property[AMF_OBJ_MAX_PROPERTY];
    
    AmfObject();
    ~AmfObject();
    
    AmfObject &operator = (AmfObject &src);
    
    int amf_encode_obj(uint8_t *buf, uint32_t len);
    int amf_decode_obj(uint8_t *buf, uint32_t len, bool hasName = false);
    
    AmfData *amf_property_by_name(uint8_t *name);
    
    void amf_dump_obj(int dp = 0);
};

#endif /* defined(__m3u8_monitor__AmfData__) */

#include "AmfData.h"
#include <stdlib.h>
#include <memory.h>


AmfData::AmfData()
{
    dataLen = 0;
    name = NULL;
    type = AMF_INVALID;
}

AmfData::~AmfData()
{
    switch (type) {
        case AMF_STRING:
        case AMF_LONG_STRING:
            delete []val.sVal;
            break;
        case AMF_OBJECT:
            delete val.oVal;
            break;
    }
    
    if(name) {
        delete []name;
        name = NULL;
    }
    
    dataLen = 0;
}


AmfData &AmfData::operator = (AmfData &src)
{
    this->~AmfData();
    
    if(src.name != NULL) {
        name = new uint8_t[strlen((char*)src.name) + 1];
        strcpy((char*)name, (char*)src.name);
    }
    
    type = src.type;
    dataLen = src.dataLen;
    
    switch (type) {
        case AMF_STRING:
        case AMF_LONG_STRING:
            val.sVal = new uint8_t[src.dataLen + 1];
            strcpy((char*)val.sVal, (char*)src.val.sVal);
            break;
        case AMF_OBJECT:
            val.oVal = new AmfObject;
            *val.oVal = *src.val.oVal;
            break;
        default:
            memcpy(&val, &src.val, sizeof(val));
            break;
    }
    
    return *this;
}

int AmfData::amf_data_set(char *name, uint8_t type, void *val)
{
    if(name) {
        this->name = new uint8_t[strlen(name) + 1];
        strcpy((char*)this->name, name);
    }
    
    this->type = type;
    
    switch (type) {
        case AMF_STRING:
        case AMF_LONG_STRING:
            dataLen = (uint32_t)strlen((char*)val);
            this->val.sVal = new uint8_t[dataLen + 1];
            strcpy((char*)this->val.sVal, (char*)val);
            break;
        case AMF_NULL:
            break;
        case AMF_NUMBER:
            dataLen = 8;
            this->val.fVal = *(double*)val;
            break;
        case AMF_BOOLEAN:
            dataLen = 1;
            this->val.bVal = *(bool*)val;
            break;
        case AMF_OBJECT:
            this->val.oVal = (AmfObject*)val;
            break;
    }
    
    return 0;
}

/*
 返回AmfData的string值,如果该AmfData不是字符串类型,则直接返回空字符串
 */
uint8_t *AmfData::get_val_string()
{
    uint8_t *r = (uint8_t*)"";
    if(AMF_STRING == type ||
       AMF_LONG_STRING == type) {
        r = val.sVal;
    }
    return r;
}

double AmfData::get_val_double()
{
    if(AMF_NUMBER == type) {
        return val.fVal;
    }
    return 0;
}

AmfObject *AmfData::get_val_object()
{
    if(AMF_OBJECT == type) {
        return val.oVal;
    }
    return NULL;
}


int AmfData::amf_data_decode(uint8_t *buf, uint32_t len, bool hasName)
{
    uint8_t *t;
    uint8_t *p = buf;
    uint32_t strLen;
    uint32_t leftLen = len;
    uint16_t nameLen = 0;
    int size = 0;
    
    if(hasName) {
        //当解析嵌套的object时，其内部为name : value的形式，因此解析value前需要解析name
        if(leftLen < 2) { return -1; }
        nameLen = AmfData::amf_decode_int16(p, len);
        p += 2;
        leftLen -= 2;
        if(leftLen < nameLen) { return -1; }
        
        name = new uint8_t[nameLen + 1];
        name[nameLen] = '\0';
        memcpy(name, p, nameLen);
        p += nameLen;
        leftLen -= nameLen;
    }
    
    if(leftLen < 1) { return -1; }
    type = *p++;
    leftLen--;
    
    switch (type) {
        case AMF_NUMBER:
            if(leftLen < 8) { return -1; }
            dataLen = 8;
            val.fVal = AmfData::amf_decode_double(p, 8);
            p += 8;
            leftLen -= 8;
            break;
            
        case AMF_BOOLEAN:
            if(leftLen < 1) { return -1; }
            dataLen = 1;
            val.bVal = AmfData::amf_decode_boolean(p, 1);
            p += 1;
            leftLen -= 1;
            break;
            
        case AMF_STRING:
            if(leftLen < 2) { return -1; }
            strLen = AmfData::amf_decode_int16(p, 2);
            
            if(leftLen < 2 + strLen) { return -1; }
            dataLen = strLen;
            val.sVal = new uint8_t[strLen + 1];
            val.sVal[strLen] = '\0';
            t = AmfData::amf_decode_string(val.sVal, p, strLen, leftLen);
            leftLen -= t - p;
            p = t;
            break;
            
        case AMF_OBJECT: {
            AmfObject *obj = new AmfObject;
            if((size = obj->amf_decode_obj(p, leftLen, true)) == -1) {
                delete obj;
                return -1;
            }
            dataLen = size;
            val.oVal = obj;
            p += size;
            leftLen -= size;
        }break;
            
        case AMF_NULL:
            break;
            
        case AMF_ECMA_ARRAY: {
            if(leftLen < 4) { return -1; }
            p += 4;
            leftLen -= 4;
            AmfObject *obj = new AmfObject;
            if((size = obj->amf_decode_obj(p, leftLen, true)) == -1) {
                delete obj;
                return -1;
            }
            
            dataLen = size;
            val.oVal = obj;
            p += size;
            leftLen -= size;
            type = AMF_OBJECT;
        }break;
            
        case AMF_LONG_STRING:
            if(leftLen < 4) { return -1; }
            strLen = AmfData::amf_decode_int32(p, 4);
            
            if(leftLen < 4 + strLen) { return -1; }
            dataLen = strLen;
            val.sVal = new uint8_t[strLen + 1];
            val.sVal[strLen] = '\0';
            t = AmfData::amf_decode_string(val.sVal, p, strLen, leftLen, true);
            leftLen -= t - p;
            p += t - p;
            break;
            
        default:
            printf("暂未支持的AMF Type.\n");
            break;
    }
        
    return p - buf;
}

uint8_t* AmfData::amf_encode_string(uint8_t *buf, uint8_t *str, uint32_t bufLen, uint32_t strLen)
{
    if(strLen < 65535) {
        if(strLen + 3 > bufLen) { return NULL; }
        *buf = AMF_STRING;
        amf_encode_int16(buf + 1, (uint16_t)strLen, bufLen - 1);
        buf += 3;
    } else {
        if(strLen + 5 > bufLen) { return NULL; }
        *buf = AMF_LONG_STRING;
        amf_encode_int32(buf + 1, strLen, bufLen - 1);
        buf += 5;
    }
    
    memcpy(buf, str, strLen);
    return buf + strLen;
}

uint8_t* AmfData::amf_decode_string(uint8_t *strBuf, uint8_t *buf, uint32_t strBufLen, uint32_t bufLen, bool isLongStr)
{
    uint32_t strLen = 0;
    uint32_t unRead = bufLen;
    uint8_t *p = buf;
    
    if(isLongStr) {
        if(unRead < 4) { return NULL; }
        strLen = amf_decode_int32(p, bufLen);
        p += 4;
        unRead -= 4;
    } else {
        if(unRead < 2) { return NULL; }
        strLen = amf_decode_int16(p, bufLen);
        p += 2;
        unRead -= 2;
    }
    
    if(strLen > unRead || strLen > strBufLen) {
        return NULL;
    }
    
    memcpy(strBuf, p, strLen);
    p += strLen;
    return p;
}

uint8_t* AmfData::amf_encode_name_string(uint8_t *buf, uint8_t *key, uint32_t keyLen, uint8_t *str, uint32_t bufLen, uint32_t strLen)
{
    if(keyLen + 2 >= bufLen) {
        return NULL;
    }
    
    uint8_t *last = amf_encode_int16(buf, (uint16_t)keyLen, bufLen);
    memcpy(last, key, keyLen);
    last += keyLen;
    
    return amf_encode_string(last, str, (uint32_t)(bufLen + buf - last), strLen);
}

uint8_t* AmfData::amf_encode_boolean(uint8_t *buf, bool val, uint32_t bufLen)
{
    if(bufLen < 1) { return NULL; }
    *buf = val;
    return buf + 1;
}


uint8_t* AmfData::amf_encode_double(uint8_t *buf, double val, uint32_t bufLen)
{
    if(bufLen < 8) { return NULL; }
    *buf++ = AMF_NUMBER;	/* type: Number */
    if(!Utils::local_is_little_endian()) {
        memcpy(buf, (uint8_t*)&val, 8);
    } else {
        uint8_t *v = (uint8_t*)&val;
        for(int i = 0; i < 8; i++) {
            buf[i] = v[8 - i - 1];
        }
    }
    return buf + 8;
}

uint8_t* AmfData::amf_encode_int16(uint8_t *buf, uint16_t val, uint32_t bufLen)
{
    if(bufLen < 2) { return NULL; }
    Utils::to_big_endian(buf, (uint8_t*)&val, 2, 2);
    return buf + 2;
}

uint8_t* AmfData::amf_encode_int32(uint8_t *buf, uint32_t val, uint32_t bufLen)
{
    if(bufLen < 4) { return NULL; }
    Utils::to_big_endian(buf, (uint8_t*)&val, 4, 4);
    return buf + 4;
}

bool AmfData::amf_decode_boolean(uint8_t *buf, uint32_t bufLen)
{
    if(bufLen < 1) { return NULL; }
    return *buf != 0;
}

double AmfData::amf_decode_double(uint8_t *buf, uint32_t len)
{
    double t = 0;
    if(len < 8) { return 0; }
    if(!Utils::local_is_little_endian()) {
        memcpy(&t, buf, 8);
    } else {
        uint8_t *v = (uint8_t*)&t;
        for(int i = 0; i < 8; i++) {
            v[i] = buf[8 - i - 1];
        }
    }
    return t;
}

uint16_t AmfData::amf_decode_int16(uint8_t *buf, uint32_t bufLen)
{
    uint16_t ret;
    if(bufLen < 2) { return 0; }
    Utils::big_endian_to_local((uint8_t*)&ret, buf, 2, 2);
    return ret;
}

uint32_t AmfData::amf_decode_int24(uint8_t *buf, uint32_t bufLen)
{
    uint32_t ret;
    if(bufLen < 3) { return 0; }
    Utils::big_endian_to_local((uint8_t*)&ret, buf, 4, 3);
    return ret;
}


uint32_t AmfData::amf_decode_int32(uint8_t *buf, uint32_t bufLen)
{
    uint32_t ret;
    if(bufLen < 4) { return 0; }
    Utils::big_endian_to_local((uint8_t*)&ret, buf, 4, 4);
    return ret;
}




AmfObject::AmfObject()
{
    proCount = 0;
    memset(property, 0, AMF_OBJ_MAX_PROPERTY * sizeof(void*));
}

AmfObject::~AmfObject()
{
    for(uint32_t i = 0; i < proCount; i++) {
        delete property[i];
        property[i] = NULL;
    }
    proCount = 0;
}

AmfObject &AmfObject::operator = (AmfObject &src)
{
    this->~AmfObject();
    
    proCount = src.proCount;
    for(uint32_t i = 0; i < proCount; i++) {
        property[i] = new AmfData;
        *property[i] = *src.property[i];
    }
    
    return *this;
}

int AmfObject::amf_encode_obj(uint8_t *buf, uint32_t len)
{
    uint8_t *p = buf;
    uint32_t leftLen = len;
    uint16_t nameLen;
    int size;
    
    for(int i = 0; i < proCount; i++) {
        
        AmfData *pro = property[i];
        
        if(pro->name != NULL) {
            nameLen = strlen((char*)pro->name);
            if(leftLen < 2 + nameLen) { return -1; }
            AmfData::amf_encode_int16(p, nameLen, leftLen);
            
            memcpy(p + 2, pro->name, nameLen);
            p += nameLen + 2;
            leftLen -= nameLen + 2;
        }
        
        if(leftLen < 1) { return -1; }
        *p++ = pro->type;
        leftLen--;
        
        switch (pro->type) {
            case AMF_NUMBER:
                if(leftLen < 8) { return -1; }
//                AmfData::amf_encode_double(p, pro->val.fVal, leftLen);
                Utils::to_big_endian(p, (uint8_t*)&pro->val.fVal, 8, 8);
                p += 8;
                leftLen -= 8;
                break;
                
            case AMF_BOOLEAN:
                if(leftLen < 1) { return -1; }
                AmfData::amf_encode_boolean(p, pro->val.bVal, leftLen);
                p += 1;
                leftLen -= 1;
                break;
                
            case AMF_STRING:
                if(leftLen < 2 + pro->dataLen) { return -1; }
                AmfData::amf_encode_int16(p, pro->dataLen, leftLen);
                memcpy(p + 2, pro->val.sVal, pro->dataLen);
                
                leftLen -= 2 + pro->dataLen;
                p += 2 + pro->dataLen;
                break;
                
            case AMF_OBJECT: {
                if((size = pro->val.oVal->amf_encode_obj(p, leftLen)) == -1) { return -1; }
                p += size;
                leftLen -= size;
                
                if(leftLen < 3) { return -1; }
                *p++ = 0;
                *p++ = 0;
                *p++ = AMF_OBJECT_END;
                leftLen -= 3;
                
            }break;
                
            case AMF_NULL:
                break;
                
//            case AMF_ECMA_ARRAY: {
//                
//                if(leftLen < 4) { return -1; }
//                p += 4;
//                leftLen -= 4;
//                
//                AmfObject *obj = new AmfObject;
//                if((size = obj->amf_decode_obj(p, len - 4, true)) == -1) {
//                    delete obj;
//                    return -1;
//                }
//                
//                dataLen = size;
//                val.oVal = obj;
//                p += size;
//                type = AMF_OBJECT;
//            }break;
                
            case AMF_LONG_STRING:
                if(leftLen < 4 + pro->dataLen) { return -1; }
                AmfData::amf_encode_int32(p, pro->dataLen, leftLen);
                memcpy(p + 4, pro->val.sVal, pro->dataLen);
                
                p += 4 + pro->dataLen;
                leftLen -= 4 + pro->dataLen;
                break;
                
            case AMF_UNDEFINED:
                break;
                
            default:
                printf("暂未支持的AMF Type.\n");
                break;
        }
    }
    
    return len - leftLen;
}

int AmfObject::amf_decode_obj(uint8_t *buf, uint32_t len, bool hasName)
{
    uint8_t *p;
    int left;
    int nParsed;
    
    p = buf;
    left = len;
    nParsed = 0;
    proCount = 0;
    
    while(left >= 0) {
        
        if(0 == left) {
            return nParsed;
        }
        
        if(left > 2 && AmfData::amf_decode_int24(p, 3) == AMF_OBJECT_END) {
            nParsed += 3;
            return nParsed;
        }
        
        AmfData *pro = new AmfData;
        
        int size = pro->amf_data_decode(p, left, hasName);
        
        if(size < 0) {
            delete pro;
            break;
        }
        
        if(proCount == AMF_OBJ_MAX_PROPERTY - 1) {
            delete pro;
            printf("Error: amf object property num limit.\n");
            return -1;
        }
        
        property[proCount++] = pro;
        nParsed += size;
        left -= size;
        p += size;
    }
    
    return -1;
}

AmfData *AmfObject::amf_property_by_name(uint8_t *name)
{
    for(uint32_t i = 0; i < proCount; i++) {
        if(property[i]->name != NULL && strcmp((char*)name, (char*)property[i]->name) == 0) {
            return property[i];
        }
    }
    return NULL;
}

//输出AmfObject
void AmfObject::amf_dump_obj(int dp)
{
    int i, k;
    
    printf("{\n");
    
    for(i = 0; i < proCount; i++) {
        AmfData *pro = property[i];
        for(k = 0; k <= dp; k++) { putchar('\t'); }
        if(pro->name != NULL) {
            printf("%s : ", pro->name);
        }
        
        switch (pro->type) {
            case AMF_NUMBER:
                printf("%f", pro->val.fVal);
                break;
                
            case AMF_BOOLEAN:
                printf("%s", pro->val.bVal ? "true" : "false");
                break;
                
            case AMF_STRING:
            case AMF_LONG_STRING:
                printf("%s", pro->val.sVal);
                break;
                
            case AMF_OBJECT:
                pro->val.oVal->amf_dump_obj(dp + 1);
                break;
                
            case AMF_NULL:
                printf("NULL");
                break;
                
            case AMF_ECMA_ARRAY:
                break;
                
            default:
                printf("暂未支持的AMF Type");
                break;
        }
        
        if(i < proCount - 1) {
            putchar(',');
        }
        putchar('\n');
    }
    
    for(i = 0; i < dp; i++) {
        putchar('\t');
    }
    printf("}");
    if(dp == 0) {
        printf("\n");
    }

}


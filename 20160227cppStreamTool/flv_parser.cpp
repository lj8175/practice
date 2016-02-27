#include "flv_parser.h"
#include "FileStream.h"
#include "Utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <unistd.h>
#include <sys/times.h>



int flv_header::flv_parse_header(Stream &stream)
{
    uint8_t header[FLV_HEADER_SIZE];
    
    if(stream.read((char*)header, FLV_HEADER_SIZE) != FLV_HEADER_SIZE) {
        return -1;
    }
    
    if(header[0] != 'F' || header[1] != 'L' || header[2] != 'V') {
        return -1;
    }
    
    version = header[3];
    flag = header[4];
    
    memcpy(flvHeaderBytes, header, FLV_HEADER_SIZE);

    return 0;
}


/*
 flv tag
 */
flv_tag::flv_tag()
{
    dataSize = 0;
    
    data = NULL;
    tagBytesData = NULL;
    videoHeader = NULL;
    audioHeader = NULL;
    scriptTag = NULL;
}

flv_tag::~flv_tag()
{
    if(tagBytesData) {
        delete []tagBytesData;
    }
    if(videoHeader) {
        delete videoHeader;
    }
    if(audioHeader) {
        delete audioHeader;
    }
    if(scriptTag) {
        delete scriptTag;
    }
    
    data = NULL;
    tagBytesData = NULL;
    videoHeader = NULL;
    audioHeader = NULL;
    scriptTag = NULL;
}


int flv_tag::flv_parse_tag(Stream &stream)
{
    uint8_t tagHeader[FLV_TAG_HEADER_SIZE];
    
    if(stream.read((char*)tagHeader, FLV_TAG_HEADER_SIZE) != FLV_TAG_HEADER_SIZE) {
        return -1;
    }
    
    tagType = tagHeader[0] & 0x1F;
    
    Utils::big_endian_to_local((uint8_t*)&dataSize, tagHeader + 1, 4, 3);
    
    Utils::big_endian_to_local((uint8_t*)&timestamp, tagHeader + 4, 4, 3);
    timestamp |= ((uint32_t)tagHeader[7]) << 24;
    
    Utils::big_endian_to_local((uint8_t*)&streamID, tagHeader + 8, 4, 3);
    
    data = NULL;
    
    tagBytesData = new uint8_t[FLV_TAG_HEADER_SIZE + dataSize + 4];
    
    if(NULL == tagBytesData) {
        fprintf(stderr, "alloc tag memory failed.\n");
        return -1;
    }
    
    memcpy(tagBytesData, tagHeader, FLV_TAG_HEADER_SIZE);
    
    if(dataSize) {
        data = tagBytesData + FLV_TAG_HEADER_SIZE;
        if(stream.read((char*)data, dataSize) != dataSize) {
            return -1;
        }
    }
    
    uint8_t buf[4];
    if(stream.read((char*)buf, 4) != 4) {
        return -1;
    }
    
    Utils::big_endian_to_local((uint8_t*)&tagSize, buf, 4, 4);
    memcpy(tagBytesData + FLV_TAG_HEADER_SIZE + dataSize, buf, 4);
    
    switch(tagType) {
        case FLV_AUDIO_TAG:
            audioHeader = new AudioCodecHeader;
            AudioCodec::parse_audio_header(audioHeader, data, dataSize);
            printf("audio tag [timestamp=%u] [len=%u].\n", timestamp, dataSize);
            break;
        case FLV_VIDEO_TAG:
            videoHeader = new VideoCodecHeader;
            VideoCodec::parse_video_header(videoHeader, data, dataSize);
//            if(videoHeader->frameType == KEY_FRAME)
            printf("video tag [timestamp=%u] [frameType=%s] [len=%u].\n", timestamp,
                   videoHeader->frameType == KEY_FRAME ? "keyframe" : (videoHeader->frameType == INTER_FRAME ? "interframe" : "other"),
                   dataSize);
            break;
            
        case FLV_SCRIPT_TAG:
			printf("\n/---------------\n收到metadata.\n----------------/\n");
		/*
            scriptTag = new flv_script_tag;
            scriptTag->parse_script_tag(data, dataSize);
          */  break;
            
    }
    
    return 0;
}


/*
 script tag
 */
int flv_script_tag::flv_script_string_parser(uint8_t *buf, uint32_t bufLen, char *strBuf, uint32_t strMaxLen)
{
    int pos = 0;
    uint32_t strLen;
    
    Utils::big_endian_to_local((uint8_t*)&strLen, buf, 2, 2);
    pos += 2;

    if(strLen + 1 > strMaxLen) {
        fprintf(stderr, "Error: string too long. func=%s \n", __FUNCTION__);
        return -1;
    }
    
    strBuf[strLen] = '\0';
    memcpy(strBuf, buf + 2, strLen);
    
    return pos + strLen;
}

int flv_script_tag::flv_script_array_parser(uint8_t *buf, uint32_t bufLen)
{
    int pos = 0, ret;
    uint32_t arrayLen, i;
    char strBuf[1000];
    
    Utils::big_endian_to_local((uint8_t*)&arrayLen, buf, 4, 4);
    pos += 4;
    
    for(i = 0; i < arrayLen; i++) {
        
        if((ret = flv_script_string_parser(buf + pos, bufLen - pos, strBuf, 1000)) < 0) {
            return -1;
        }
        
        pos += ret;
        
        printf("%s : ", strBuf);
        
        if((ret = flv_script_data_parser(buf + pos, bufLen - pos)) < 0) {
            return -1;
        }
        pos += ret;
        
        printf("\n");
    }
    
    return pos;
}



int flv_script_tag::flv_script_data_parser(uint8_t *sBuf, uint32_t sLen)
{
    int pos = 0, ret;
    char strBuf[1000];
    
    ret = 0;
    switch (sBuf[pos++]) {
        case 0: //double
//            printf("%0.2f", flv_stream::read_double(sBuf + pos));
            ret = 8;
            break;
        case 1: //boolean
            ret = 1;
            break;
        case 2: //string
            ret = flv_script_string_parser(sBuf + pos, sLen - pos, strBuf, 1000);
            printf("%s", strBuf);
            break;
        case 3: //object
            break;
        case 4:
            break; //MovieClip
        case 5: //null
            break;
        case 7: //Reference
            break;
        case 8: //ECMA array
            ret = flv_script_array_parser(sBuf + pos, sLen - pos);
            break;
        case 9: //Object end marker
            break;
        case 10: //Strict array
            break;
        case 11: //Date
            break;
        case 12: //Long string
            break;
        default: //undefine
            break;
    }
    
    if(ret < 0) {
        return -1;
    }
    pos += ret;
    
    return pos;
}


int flv_script_tag::parse_script_tag(uint8_t *buf, uint32_t len)
{
    int pos = 0;
    
    //分析onMetaData等
    if((pos = flv_script_data_parser(buf, len)) < 0) {
        return -1;
    }
    
//    printf("%s\n", buf);
    
    if((pos = flv_script_data_parser(buf + pos, len - pos)) < 0) {
        return -1;
    }
    
    return 0;
}


clock_t clock_ms()
{
    struct tms t;
    static clock_t clk_tck;
    if (!clk_tck) clk_tck = sysconf(_SC_CLK_TCK);
    return times(&t) * 1000 / clk_tck;
}



/*
 flv parser
*/
int flv_parser::parse_flv(Stream &stream)
{
    if(header.flv_parse_header(stream)) {
        return -1;
    }
    
    if(stream.read(NULL, 4) != 4) { //跳过PreviousTagSize0
        return -1;
    }
    
    FileStream fs;
    
    if(dumpFileName[0] != '\0') {
        if(fs.open(dumpFileName)) {
            printf("Error: 输出文件路径不存在.\n");
            return -1;
        }
        fs.write((char*)header.flvHeaderBytes, 9);
        fs.write("\0\0\0\0", 4);
    }
    
    flv_tag *tag;
    while(!stream.end()) {
        tag = new flv_tag;
        
        if(tag->flv_parse_tag(stream)) {
            delete tag;
            return -1;
        }
        
        if(dumpFileName[0] != '\0') {
            fs.write((char*)tag->tagBytesData, tag->tagSize);
            uint8_t buf[4];
            Utils::to_big_endian(buf, (uint8_t*)&tag->tagSize, 4, 4);
            fs.write((char*)buf, 4);
        }

        delete tag;
        
        if(stopFlag) { break; }
    }
    
    fs.close();

    return 0;
}


int flv_parser::repair_flv_file(const char *filePath)
{
    char newFilePath[256];
    char flvHeader[FLV_HEADER_SIZE];
    
    FileStream is, os;
    
    sprintf(newFilePath, "%s.repair.flv", filePath);
    
    is.open(filePath);
    os.open(newFilePath);
    
    if(is.read(flvHeader, FLV_HEADER_SIZE) != FLV_HEADER_SIZE) {
        return -1;
    }
    
    is.read(NULL, 4);
    os.write(flvHeader, FLV_HEADER_SIZE);
    os.write("\0\0\0\0", 4);
    
    flv_tag *tag;
    for(;;) {
        tag = new flv_tag;
        
        if(tag->flv_parse_tag(is)) {
            delete tag;
            break;
        }
        
        os.write((char*)tag->tagBytesData, tag->tagSize);
        uint8_t buf[4];
        Utils::to_big_endian(buf, (uint8_t*)&tag->tagSize, 4, 4);
        os.write((char*)buf, 4);
        
        delete tag;
    }
    
    is.close();
    os.close();
    
    return 0;
}

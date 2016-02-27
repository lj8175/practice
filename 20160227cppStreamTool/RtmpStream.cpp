#include "Utils.h"
#include "AmfData.h"
#include "RtmpStream.h"
#include "AudioCodec.h"
#include "VideoCodec.h"
#include "FileStream.h"
#include "flv_parser.h"
#include <stdlib.h>
#include <memory.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <assert.h>
#include <errno.h>


RtmpStream::RtmpStream()
{
    isPlaying = false;
    isPushMode = false;
    streamId = 0;

    sockfd = 0;
    chunkBufLen = 0;
    
    invokeId = 0;
    inChunkSize = outChunkSize = RTMP_DEFAULT_CHUNKSIZE;
    
    memset(recvChunkMap, 0, sizeof(void*) * CHUNK_STREAM_ID_LIMIT);
    memset(sendMsgHeaderCache, 0, sizeof(void*) * CHUNK_STREAM_ID_LIMIT);
    memset(recvMsgHeaderCache, 0, sizeof(void*) * CHUNK_STREAM_ID_LIMIT);

    callList = NULL;
}

RtmpStream::~RtmpStream()
{
    close();
    isPlaying = false;
    isPushMode = false;
}

//rtmp拉流
int RtmpStream::test(const char *rtmpUrl)
{
    if(open(rtmpUrl)) {
        return -1;
    }
    
    const char flvHeader[] = {
        'F', 'L', 'V',
        1, 5, 0,
        0, 0, 9
    };
    
    uint8_t flvTagHeader[11];
    FileStream fs;
    
    if(dumpFileName[0] != '\0') {
        if(fs.open(dumpFileName)) {
            printf("Error: 输出文件路径不存在.\n");
            return -1;
        }
        fs.write(flvHeader, 9);
        fs.write("\0\0\0\0", 4);
    }
    
    RtmpMsg *m;
    while(!rtmp_recv_message(&m)) {
//        printf("Recv rtmp message [msgType=0x%02X] [csId=0x%02X] [ts=%u] [len=%u].\n", m->header.msgType, m->header.chunkStreamId, m->header.timestamp, m->header.payloadLen);
        
        if(m->header.msgType == RTMP_VIDEO_MSG) {
            VideoCodecHeader vHeader;
            VideoCodec::parse_video_header(&vHeader, m->msgData, m->header.payloadLen);
            
            printf("video tag [timestamp=%u] [frameType=%s] [len=%u].\n", m->header.timestamp,
                   vHeader.frameType == KEY_FRAME ? "keyframe" : (vHeader.frameType == INTER_FRAME ? "interframe" : "other"),
                   m->header.payloadLen);
            
        } else if(m->header.msgType == RTMP_AUDIO_MSG) {
            AudioCodecHeader aHeader;
            AudioCodec::parse_audio_header(&aHeader, m->msgData, m->header.payloadLen);
            printf("audio tag [timestamp=%u] [len=%u].\n", m->header.timestamp, m->header.payloadLen);
        }
        
        if(dumpFileName[0] != '\0' && (m->header.msgType == RTMP_VIDEO_MSG ||
           m->header.msgType == RTMP_AUDIO_MSG/* ||
           m->header.msgType == RTMP_NOTIFY_MSG*/)) { //只dump音视频帧和metadata
            
            memset(flvTagHeader, 0, 11);
            flvTagHeader[0] = m->header.msgType;
            Utils::to_big_endian(flvTagHeader + 1, (uint8_t*)&m->header.payloadLen, 3, 4);
            Utils::to_big_endian(flvTagHeader + 4, (uint8_t*)&m->header.timestamp, 3, 4);
            flvTagHeader[7] = m->header.timestamp >> 24;
            
            fs.write((char*)flvTagHeader, 11); //write this tag header
            fs.write((char*)m->msgData, m->header.payloadLen); //write tag body
            
            uint8_t buf[4];
            uint32_t tagSize = 11 + m->header.payloadLen;
            Utils::to_big_endian(buf, (uint8_t*)&tagSize, 4, 4);
            fs.write((char*)buf, 4); //write this tag size
        }
        
        delete m;
        
        if(stopFlag) { break; }
    }
    
    fs.close();
    close();
    return 0;
}


//rtmp推流
int RtmpStream::push()
{
    char *pushUrl = "rtmp://192.168.1.110/live/test";//"rtmp://121.40.96.231/hls/mystream";
    
    isPushMode = true;
    
    if(open(pushUrl)) {
        return -1;
    }
    
    const char *fileList[] = {
        "/Users/hongduoxing/Movies/ads.flv",
        "/Users/hongduoxing/Movies/2015-10-27 13-09-14_http.flv",
        "/Users/hongduoxing/Movies/2015-10-27 13-12-16_http.flv",
        "/Users/hongduoxing/Movies/2015-10-25 22-37-57.flv"
    };
    
    uint32_t prev_flv_last_ts = 0;
    
repeat:
    for(int i = 0; i < sizeof(fileList)/sizeof(fileList[0]); i++) {
        printf("开始推%s.\n", fileList[i]);
        FileStream fs;
        
        if(fs.open(fileList[i])) {
            return -1;
        }
        
        fs.seek(STREAM_SET, 9 + 4); //跳过flvHeader和prevTagSize0

        uint32_t last_frame_ts = 0;
        uint32_t base_frame_ts_not0 = 0;
        uint64_t base_push_clock = Utils::clock_ms();
        
        while(!fs.end()) {
            flv_tag tag;
            if(tag.flv_parse_tag(fs)) {
                break;
            }
            
            if(tag.tagType != 0x08 &&
               tag.tagType != 0x09) {
                continue;
            }
            
            if(base_frame_ts_not0 == 0 && tag.timestamp > 0) {
                base_frame_ts_not0 = tag.timestamp;
            }
            
            if(last_frame_ts < tag.timestamp) {
                last_frame_ts = tag.timestamp;
            }
            
            for(;tag.timestamp - base_frame_ts_not0 > Utils::clock_ms() - base_push_clock + 10;) {
                usleep(5000);
            }

            tag.timestamp = tag.timestamp - base_frame_ts_not0 + prev_flv_last_ts;
            
            RtmpMsg msg;
            msg.header.msgType = (RtmpMsgType)tag.tagType;
            msg.header.payloadLen = tag.dataSize;
            msg.header.timestamp = tag.timestamp;
            msg.header.streamId = streamId;
            msg.header.chunkStreamId = 0x04;
            msg.msgData = new uint8_t[tag.dataSize];
            memcpy(msg.msgData, tag.data, tag.dataSize);
            
            if(rtmp_send_message(&msg)) {
                return RTMP_ERROR;
            }
        }
        
        fs.close();
        
        prev_flv_last_ts += last_frame_ts;
    }
    
    goto repeat;
    
    rtmp_send_fc_unpublish();
    rtmp_send_delete_stream();
    
    return 0;
}

#pragma -mark interface

int RtmpStream::open(const char *rtmpUrl)
{
    isPlaying = false;
    
    if(parse_url(rtmpUrl, streamUrl, streamIdUrl)) {
        goto err;
    }
    
    if(rtmp_create_connection()) {
        goto err;
    }
    
    RtmpMsg *m;
    while(!isPlaying && !rtmp_recv_message(&m)) { //驱动接收发送响应命令消息
        delete m;
    }
    
    if(!isPlaying) {
        goto err;
    }
    
//    //设置发送chunk分块长度,对Adobe Flash Media Server无效
//    if(!rtmp_send_change_chunk_size(4096)) {
//        outChunkSize = 4096;
//    }

    return RTMP_OK;
err:
    close();
    return RTMP_ERROR;
}

int RtmpStream::close()
{
    if(sockfd > 0) {
        ::close(sockfd);
        sockfd = 0;
    }
    
    //释放未合并为message的chunk
    for(int i = 0; i < CHUNK_STREAM_ID_LIMIT; i++) {
        RtmpMsgChunk *cur = recvChunkMap[i];
        
        while(cur) {
            RtmpMsgChunk *next = cur->next;
            delete cur;
            cur = next;
        }
    }
    
    //释放remote call list
    RtmpRemoteCall *t, *call = callList;
    while(call) {
        t = call->next;
        delete call;
        call = t;
    }
    callList = NULL;
    
    //释放message header cache
    for(int i = 0; i < CHUNK_STREAM_ID_LIMIT; i++) {
        if(sendMsgHeaderCache[i]) {
            delete sendMsgHeaderCache[i];
        }
        
        if(sendMsgHeaderCache[i]) {
            delete recvMsgHeaderCache[i];
        }
        
        sendMsgHeaderCache[i] = NULL;
        recvMsgHeaderCache[i] = NULL;
    }

    return 0;
}

int RtmpStream::read(char *buf, int len, bool wait)
{
    return 0;
}

int RtmpStream::write(const char *buf, int len)
{
    return 0;
}

int RtmpStream::end()
{
    return 0;
}

#pragma mark rtmp handshake

//解析rtmp url
int RtmpStream::parse_url(const char *url, string &streamUrl, string &streamId)
{
    int len = strlen(url);
    
    if(len < 8) {
        return -1;
    }
    
    char buf[1024];
    char protocol[8] = {0};
    const char *p, *t, *k;

    memcpy(protocol, url, 7);
    
    if(strcasecmp(protocol, "rtmp://")) {
        printf("Error: rtmp url错误.\n");
        return RTMP_ERROR;
    }
    
    /* parse application
     * rtmp://host[:port]/app[/appinstance][/...]
     * application = app[/appinstance]
     */
    
    if((p = strstr(url + 7, "/")) == NULL) {
        return RTMP_ERROR;
    }
    
    //获得hostname和端口号
    t = url + 7;
    memcpy(buf, t, p - t);
    buf[p - t] = '\0';
    
    if((t = strstr(buf, ":")) == NULL) {
        host = buf;
        port = 1935;
    } else {
        buf[t - buf] = '\0';
        host = buf;
        sscanf(t + 1, "%d", &port);
        if(port > 65535) {
            return -1;
        }
    }
    
    //获得rtmpUrl appUrl
    t = p + 1;
    p = strstr(t, "/");
    if(p) {
//        memcpy(buf, t, p - t);
//        buf[p - t] = '\0';
//        appUrl = buf;
        
        k = t + strlen(t);
        while(*--k != '/');
        memcpy(buf, t, k - t);
        buf[k - t] = '\0';
        appUrl = buf;
        
//        memcpy(buf, url, p - url);
//        buf[p - url] = '\0';
//        rtmpUrl = buf;
        
        memcpy(buf, url, k - url);
        buf[k - url] = '\0';
        rtmpUrl = buf;
    } else {
        appUrl = t;
        rtmpUrl = url;
    }
    
    streamUrl = p + 1;
    
    //获得streamUrl和streamId
    while((p = strstr(t, "/")) != NULL) {
        t = p + 1;
    }
    
    memcpy(buf, url, t - url);
    buf[t - url] = '\0';
    
//    streamUrl = buf;
    streamId = t;
    
    return RTMP_OK;
}


int RtmpStream::rtmp_create_connection()
{
    if(rtmp_socket_connect()) {
        return RTMP_ERROR;
    }
    
    if(rtmp_client_handshake()) {
        printf("Error: rtmp handshake failed\n");
        return RTMP_ERROR;
    }
    
    if(rtmp_send_connect()) {
        printf("Error: rtmp send connect message failed\n");
        return RTMP_ERROR;
    }
    
    return RTMP_OK;
}

int RtmpStream::rtmp_socket_connect()
{
    uint32_t ip = Utils::get_host_ip(host, NULL);
    
    if((sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
        return RTMP_ERROR;
    }
    
    struct sockaddr_in conn_addr;
    bzero(&conn_addr, sizeof(conn_addr));
    conn_addr.sin_family = AF_INET;
    conn_addr.sin_port = htons(port);
    conn_addr.sin_addr = *(struct in_addr *)&ip;
    
    if(-1 == connect(sockfd, (struct sockaddr *)&conn_addr, sizeof(conn_addr))) {
        printf("Error: socket connect failed. file=%s function=%s line=%d\n", __FILE__, __FUNCTION__, __LINE__);
        return RTMP_ERROR;
    }
    
    int on = 1;
//    int set = 1;
//    setsockopt(sockfd, SOL_SOCKET, SO_NOSIGPIPE, (void *)&set, sizeof(int)); //屏蔽掉sigpipe信号
    setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, (char *) &on, sizeof(on));

    return RTMP_OK;
}

int RtmpStream::rtmp_client_handshake()
{
#define RTMP_HANDSHAKE_PKG_SIZE 1536

    int i;
    long ret;
    uint32_t unixTS;
    uint32_t serverTime;
    uint8_t buf1[RTMP_HANDSHAKE_PKG_SIZE + 1];
    uint8_t buf2[RTMP_HANDSHAKE_PKG_SIZE + 1];
    
    printf("Rtmp client handshake start.\n");
    
    buf1[0] = 0x03; //version=0x03
    unixTS = (uint32_t)Utils::clock_ms();
    Utils::to_big_endian(buf1 + 1, (uint8_t*)&unixTS, 4, 4);
    memset(buf1 + 5, 0, 4);
    for(i = 9; i < RTMP_HANDSHAKE_PKG_SIZE + 1; ++i) {
        buf1[i] = rand() % 256;
    }

    if((ret = socket_send(sockfd, buf1, RTMP_HANDSHAKE_PKG_SIZE + 1)) <= 0) {
        printf("Error: socket send failed. ret=%ld file=%s function=%s line=%d\n", ret, __FILE__, __FUNCTION__, __LINE__);
        return RTMP_ERROR;
    }
    
    printf("Rtmp client handshake send c0 & c1 ok\n");
    
    if((ret = socket_recv(sockfd, buf2, RTMP_HANDSHAKE_PKG_SIZE + 1, true)) != RTMP_HANDSHAKE_PKG_SIZE + 1) {
        printf("Error: socket recv failed. ret=%ld file=%s function=%s line=%d\n", ret, __FILE__, __FUNCTION__, __LINE__);
        return RTMP_ERROR;
    }
    
    if(buf2[0] != buf1[0]) {
        return RTMP_ERROR;
    }
    
    Utils::big_endian_to_local((uint8_t*)&serverTime, buf2 + 1, 4, 4);
    
//    printf("%s: Server Uptime : %d\n", __FUNCTION__, serverTime);
//    printf("%s: FMS Version   : %d.%d.%d.%d\n", __FUNCTION__, buf2[5], buf2[6], buf2[7], buf2[8]);

    printf("Rtmp client handshake recv s0 & s1 ok\n");
    
    if((ret = socket_send(sockfd, buf2 + 1, RTMP_HANDSHAKE_PKG_SIZE)) <= 0) {
        printf("Error: socket send failed. ret=%ld file=%s function=%s line=%d\n", ret, __FILE__, __FUNCTION__, __LINE__);
        return RTMP_ERROR;
    }
    
    printf("Rtmp client handshake send c2 ok\n");

    if((ret = socket_recv(sockfd, buf2, RTMP_HANDSHAKE_PKG_SIZE, true)) != RTMP_HANDSHAKE_PKG_SIZE) {
        printf("Error: socket recv failed. ret=%ld file=%s function=%s line=%d\n", ret, __FILE__, __FUNCTION__, __LINE__);
        return RTMP_ERROR;
    }
    
    printf("Rtmp client handshake recv s2 ok\n");

    if(memcmp(buf1 + 1, buf2, RTMP_HANDSHAKE_PKG_SIZE) != 0) {
        printf("Warning c2 not match s2.\n");
//        return -1;
    }
    
    printf("Rtmp client handshake finish\n");
    
    return RTMP_OK;
}


#pragma mark package rtmp invoke msg

//发送command message(connect)
int RtmpStream::rtmp_send_connect()
{
    RtmpMsg msg;
    const char *cmd = "connect";
    
    msg.header.chunkStreamId = 0x03;
    msg.header.msgType = RTMP_INVOKE_MSG; //Invoke like remoting call
    msg.header.timestamp = 0;
    msg.header.streamId = 0;
    msg.msgData = new uint8_t[4096];
    
    uint8_t *body = msg.msgData;
    
    body = AmfData::amf_encode_string(body, (uint8_t*)cmd, 4096, (uint32_t)strlen(cmd));
    body = AmfData::amf_encode_double(body, (double)++invokeId, 1024);
    *body++ = AMF_OBJECT;
    //填充AMF数据
    
    body = AmfData::amf_encode_name_string(body, (uint8_t*)"app", 3, (uint8_t*)appUrl.c_str(), 4096, (uint32_t)appUrl.length());
    body = AmfData::amf_encode_name_string(body, (uint8_t*)"tcUrl", 5, (uint8_t*)rtmpUrl.c_str(), 4096, (uint32_t)rtmpUrl.length());
    
    *body++ = 0;
    *body++ = 0;			/* end of object - 0x00 0x00 0x09 */
    *body++ = AMF_OBJECT_END;
    
    msg.header.payloadLen = (uint32_t)(body - msg.msgData);
    
    return rtmp_send_message(&msg);
}

//发送command message(createStream)
int RtmpStream::rtmp_send_create_stream()
{
    RtmpMsg msg;
    const char *cmd = "createStream";
    
    msg.header.chunkStreamId = 0x03;
    msg.header.msgType = RTMP_INVOKE_MSG; //Invoke like remoting call
    msg.header.timestamp = 0;
    msg.msgData = new uint8_t[4096];
    
    uint8_t *body = msg.msgData;
    
    body = AmfData::amf_encode_string(body, (uint8_t*)cmd, 4096, (uint32_t)strlen(cmd));
    body = AmfData::amf_encode_double(body, (double)++invokeId, 1024);
    *body++ = AMF_NULL;

    msg.header.payloadLen = (uint32_t)(body - msg.msgData);
    
    return rtmp_send_message(&msg);
}


int RtmpStream::rtmp_send_server_bw()
{
    RtmpMsg msg;
    
    msg.header.chunkStreamId = 0x02;
    msg.header.msgType = RTMP_SERVER_BW_MSG;
    msg.header.timestamp = 0;
    msg.msgData = new uint8_t[4096];
    
    uint8_t *body = msg.msgData;
    AmfData::amf_encode_int32(body, serverBW, 4);
    
    msg.header.payloadLen = 4;
    
    return rtmp_send_message(&msg);
}

int RtmpStream::rtmp_send_check_bw()
{
    RtmpMsg msg;
    const char *cmd = "_checkbw";
    
    msg.header.chunkStreamId = 0x03;
    msg.header.msgType = RTMP_INVOKE_MSG;
    msg.header.timestamp = 0;
    msg.msgData = new uint8_t[4096];
    
    uint8_t *body = msg.msgData;
   
    body = AmfData::amf_encode_string(body, (uint8_t*)cmd, 4096, (uint32_t)strlen(cmd));
    body = AmfData::amf_encode_double(body, (double)++invokeId, 1024);
    *body++ = AMF_NULL;
    
    msg.header.payloadLen = (uint32_t)(body - msg.msgData);
    
    return rtmp_send_message(&msg);
}

int RtmpStream::rtmp_send_play()
{
    RtmpMsg msg;
    const char *cmd = "play";
    
    msg.header.chunkStreamId = 0x08;
    msg.header.msgType = RTMP_INVOKE_MSG; //Invoke like remoting call
    msg.header.streamId = streamId;
    msg.header.timestamp = 0;
    msg.msgData = new uint8_t[4096];
    
    uint8_t *body = msg.msgData;
    
    body = AmfData::amf_encode_string(body, (uint8_t*)cmd, 4096, (uint32_t)strlen(cmd));
    body = AmfData::amf_encode_double(body, (double)++invokeId, 1024);
    *body++ = AMF_NULL;
    
    body = AmfData::amf_encode_string(body, (uint8_t*)streamIdUrl.c_str(), 4096, (uint32_t)streamIdUrl.length());
    body = AmfData::amf_encode_double(body, 0.0, 1024);
    
    msg.header.payloadLen = (uint32_t)(body - msg.msgData);
    
    return rtmp_send_message(&msg);
}

int RtmpStream::rtmp_send_release_stream()
{
    RtmpMsg msg;
    const char *cmd = "releaseStream";
    
    msg.header.chunkStreamId = 0x03;
    msg.header.msgType = RTMP_INVOKE_MSG; //Invoke like remoting call
    msg.header.timestamp = 0;
    msg.msgData = new uint8_t[4096];
    
    uint8_t *body = msg.msgData;
    
    body = AmfData::amf_encode_string(body, (uint8_t*)cmd, 4096, (uint32_t)strlen(cmd));
    body = AmfData::amf_encode_double(body, (double)++invokeId, 1024);
    *body++ = AMF_NULL;
    
    body = AmfData::amf_encode_string(body, (uint8_t*)streamIdUrl.c_str(), 4096, (uint32_t)streamIdUrl.length());
    
    msg.header.payloadLen = (uint32_t)(body - msg.msgData);
    
    return rtmp_send_message(&msg);
}

int RtmpStream::rtmp_send_delete_stream()
{
    RtmpMsg msg;
    const char *cmd = "deleteStream";
    
    msg.header.chunkStreamId = 0x03;
    msg.header.msgType = RTMP_INVOKE_MSG; //Invoke like remoting call
    msg.header.timestamp = 0;
    msg.msgData = new uint8_t[4096];
    
    uint8_t *body = msg.msgData;
    
    body = AmfData::amf_encode_string(body, (uint8_t*)cmd, 4096, (uint32_t)strlen(cmd));
    body = AmfData::amf_encode_double(body, (double)++invokeId, 1024);
    *body++ = AMF_NULL;
    body = AmfData::amf_encode_double(body, (double)streamId, 1024);
    
    msg.header.payloadLen = (uint32_t)(body - msg.msgData);
    
    return rtmp_send_message(&msg);
}

int RtmpStream::rtmp_send_fc_publish()
{
    RtmpMsg msg;
    const char *cmd = "FCPublish";
    
    msg.header.chunkStreamId = 0x03;
    msg.header.msgType = RTMP_INVOKE_MSG; //Invoke like remoting call
    msg.header.timestamp = 0;
    msg.header.streamId = 0;
    msg.msgData = new uint8_t[4096];
    
    uint8_t *body = msg.msgData;
    
    body = AmfData::amf_encode_string(body, (uint8_t*)cmd, 4096, (uint32_t)strlen(cmd));
    body = AmfData::amf_encode_double(body, (double)++invokeId, 1024);
    *body++ = AMF_NULL;
    
    body = AmfData::amf_encode_string(body, (uint8_t*)streamIdUrl.c_str(), 4096, (uint32_t)streamIdUrl.length());
    
    msg.header.payloadLen = (uint32_t)(body - msg.msgData);
    
    return rtmp_send_message(&msg);
}

int RtmpStream::rtmp_send_fc_unpublish()
{
    RtmpMsg msg;
    const char *cmd = "FCUnpublish";
    
    msg.header.chunkStreamId = 0x03;
    msg.header.msgType = RTMP_INVOKE_MSG; //Invoke like remoting call
    msg.header.timestamp = 0;
    msg.header.streamId = 0;
    msg.msgData = new uint8_t[4096];
    
    uint8_t *body = msg.msgData;
    
    body = AmfData::amf_encode_string(body, (uint8_t*)cmd, 4096, (uint32_t)strlen(cmd));
    body = AmfData::amf_encode_double(body, (double)++invokeId, 1024);
    *body++ = AMF_NULL;
    
    body = AmfData::amf_encode_string(body, (uint8_t*)streamIdUrl.c_str(), 4096, (uint32_t)streamIdUrl.length());
    
    msg.header.payloadLen = (uint32_t)(body - msg.msgData);
    
    return rtmp_send_message(&msg);
}

int RtmpStream::rtmp_send_publish()
{
    RtmpMsg msg;
    const char *cmd = "publish";
    
    msg.header.chunkStreamId = 0x04;
    msg.header.msgType = RTMP_INVOKE_MSG; //Invoke like remoting call
    msg.header.streamId = streamId;
    msg.header.timestamp = 0;
    msg.msgData = new uint8_t[4096];
    
    uint8_t *body = msg.msgData;
    
    body = AmfData::amf_encode_string(body, (uint8_t*)cmd, 4096, (uint32_t)strlen(cmd));
    body = AmfData::amf_encode_double(body, (double)++invokeId, 1024);
    *body++ = AMF_NULL;
    
    body = AmfData::amf_encode_string(body, (uint8_t*)streamIdUrl.c_str(), 4096, (uint32_t)streamIdUrl.length());
    body = AmfData::amf_encode_string(body, (uint8_t*)"live", 4096, 4);
    
    msg.header.payloadLen = (uint32_t)(body - msg.msgData);
    
    return rtmp_send_message(&msg);
}

int RtmpStream::rtmp_send_change_chunk_size(uint32_t newChunkSize)
{
    RtmpMsg msg;
    
    msg.header.chunkStreamId = 0x03;
    msg.header.msgType = RTMP_CHANGE_CHUNK_SIZE_MSG; //Invoke like remoting call
    msg.header.streamId = streamId;
    msg.header.timestamp = 0;
    msg.msgData = new uint8_t[4096];
    
    uint8_t *body = msg.msgData;
    
//    body = AmfData::amf_encode_double(body, (double)newChunkSize, 1024);
    Utils::to_big_endian(body, (uint8_t*)&newChunkSize, 4, 4);
    body += 4;
    
    
    msg.header.payloadLen = (uint32_t)(body - msg.msgData);
    
    return rtmp_send_message(&msg);
}

#pragma mark send & recv rtmp message


//发送rtmp消息
int RtmpStream::rtmp_send_message(RtmpMsg *msg)
{
    RtmpMsgChunk chunk;
    bool first = true;
    int leftSize = msg->header.payloadLen;
    uint32_t sendSize = 0;
    uint8_t buf[6638];
    
    chunk.baseHeader.chunkStreamId = msg->header.chunkStreamId;
    chunk.chunkHeader.msgTypeId = msg->header.msgType;
    chunk.chunkHeader.timestamp.timestamp = msg->header.timestamp;
    chunk.chunkHeader.msgLen = msg->header.payloadLen;
    chunk.chunkHeader.msgStreamId = msg->header.streamId;
    
    chunk.chunkBody = new uint8_t[outChunkSize];
    
    while(leftSize > 0) {
        //切分msg为chunk发送
        if(first) {
            first = false;
            chunk.baseHeader.chunkHeaderType = RTMP_MSG_CHUNK_HEADER_11_BYTE;
        } else {
            chunk.baseHeader.chunkHeaderType = RTMP_MSG_CHUNK_HEADER_0_BYTE;
        }
        
        int size = min((int)outChunkSize, (int)leftSize);
        memcpy(chunk.chunkBody, msg->msgData + sendSize, size);
        
        int chunkPkgSize = chunk_encode(&chunk, buf, sizeof(buf), size);
        
        long ret;
        if((ret = socket_send(sockfd, buf, chunkPkgSize)) <= 0) {
            printf("Error: send chunk failed. file=%s function=%s line=%d\n", __FILE__, __FUNCTION__, __LINE__);
            return RTMP_ERROR;
        }
        
        leftSize -= size;
        sendSize += size;
    }

    rtmp_msg_send_handle(msg);
    
    return RTMP_OK;
}

//接收rtmp消息
int RtmpStream::rtmp_recv_message(RtmpMsg **msg)
{
    *msg = NULL;
    /*
     不断收取chunk，相同chunkStreamId的合并为一个message
     */
    
    RtmpMsgChunk *chunk = NULL;
    
    for(;;) {
        bool needRecv = (chunkBufLen == 0);
        
        if(NULL == chunk) {
            chunk = new RtmpMsgChunk;
        }
        
        if(chunkBufLen > 0) {
            int nParsed;
            if((nParsed = chunk_decode(chunk, recvChunkBuf, chunkBufLen)) == RTMP_NEED_MORE_DATA) {
                needRecv = true;
            } else if(nParsed > 0) {
                chunkBufLen -= nParsed;
                memmove(recvChunkBuf, recvChunkBuf + nParsed, chunkBufLen);
                
                //将本次收到的chunk链接在对应chunkStreamId的链表上(头插),然后检测能否将该链表上的所有chunk合并为message
                chunk->next = recvChunkMap[chunk->baseHeader.chunkStreamId];
                recvChunkMap[chunk->baseHeader.chunkStreamId] = chunk;
                
                RtmpMsg *newMsg = try_merge_chunks(recvChunkMap + chunk->baseHeader.chunkStreamId);
                if(newMsg) {
                    if(rtmp_msg_recv_handle(newMsg)) {
                        delete newMsg;
                        return -1;
                    }
                    
                    *msg = newMsg;
                    break;
                }
                chunk = NULL;
            } else {
                delete chunk;
                return RTMP_ERROR;
            }
        }
        
        if(needRecv) {
            long ret;
            if((ret = socket_recv(sockfd, recvChunkBuf + chunkBufLen, RTMP_RECV_CHUNK_BUF_LEN - chunkBufLen)) <= 0) {
                printf("Error: socket recv failed. file=%s function=%s line=%d\n", __FILE__, __FUNCTION__, __LINE__);
                return RTMP_ERROR;
            }
            chunkBufLen += (uint32_t)ret;
        }
    }
     
    return RTMP_OK;
}

#pragma mark rtmp message handle


//处理发送的message
int RtmpStream::rtmp_msg_send_handle(RtmpMsg *msg)
{
    if(RTMP_INVOKE_MSG == msg->header.msgType) {
        //将remote call消息挂在callList链表中，以便在收到remote call response时处理对应的回复消息
        
        AmfObject obj;
        if(obj.amf_decode_obj(msg->msgData, msg->header.payloadLen) != msg->header.payloadLen) {
            printf("Error: Amf object decode failed.\n");
            return RTMP_ERROR;
        }
        
        RtmpRemoteCall *node = new RtmpRemoteCall;
        node->obj = obj;
        node->next = callList;
        callList = node;
    }
    return 0;
}

//收到rtmp messge后处理消息
int RtmpStream::rtmp_msg_recv_handle(RtmpMsg *msg)
{
    switch (msg->header.msgType) {
        case RTMP_CHANGE_CHUNK_SIZE_MSG:
            rtmp_chunk_size_msg_handle(msg);
            break;
        case RTMP_BYTE_READ_MSG:
            break;
        case RTMP_PING_MSG:
            rtmp_ping_msg_handle(msg);
            break;
        case RTMP_SERVER_BW_MSG:
            rtmp_server_bandwidth_msg_handle(msg);
            break;
        case RTMP_CLIENT_BW_MSG:
            rtmp_client_bandwidth_msg_handle(msg);
            break;
        case RTMP_AUDIO_MSG:
        case RTMP_VIDEO_MSG:
            break;
        case RTMP_NOTIFY_MSG:
            rtmp_notify_msg_handle(msg);
            break;
        case RTMP_SHARE_OBJ_MSG:
            break;
        case RTMP_INVOKE_MSG:
            rtmp_invoke_msg_handle(msg);
            break;
        case RTMP_STREAM_DATA_MSG:
            printf("recv rtmp stream data msg.\n");
            break;
        default:
            break;
    }
    return 0;
}

/*
 处理chunk size change消息
 */
int RtmpStream::rtmp_chunk_size_msg_handle(RtmpMsg *msg)
{
    if(msg->header.payloadLen >= 4) {
        inChunkSize = AmfData::amf_decode_int32(msg->msgData, msg->header.payloadLen);
    }
    return RTMP_OK;
}

/*
 处理ping消息
 */
int RtmpStream::rtmp_ping_msg_handle(RtmpMsg *msg)
{
//    printf("recv rtmp ping msg.\n");
    uint16_t type;
    type = AmfData::amf_decode_int16(msg->msgData, 2);
//    printf("%d %d.\n", type, AmfData::amf_decode_int32(msg->msgData + 2, 4));
    return RTMP_OK;
}

/*
 处理server bandwidth消息
 */
int RtmpStream::rtmp_server_bandwidth_msg_handle(RtmpMsg *msg)
{
    serverBW = AmfData::amf_decode_int32(msg->msgData, msg->header.payloadLen);
    return RTMP_OK;
}

/*
 处理client bandwidth消息
 */
int RtmpStream::rtmp_client_bandwidth_msg_handle(RtmpMsg *msg)
{
    clientBW = AmfData::amf_decode_int32(msg->msgData, msg->header.payloadLen);
    return RTMP_OK;
}

/*
 处理notify消息
 */
int RtmpStream::rtmp_notify_msg_handle(RtmpMsg *msg)
{
    //收到metaData
    
    return RTMP_OK;
}

//处理收到的invoke message
int RtmpStream::rtmp_invoke_msg_handle(RtmpMsg *invMsg)
{
    uint8_t *pBody = invMsg->msgData;
    
//    printf("recv rtmp remote call msg.\n");
    
    if(*pBody != 0x02) {
        printf("Error: invoke msg format error.\n");
        return RTMP_ERROR;
    }
    
    //    for(int i = 0; i < invMsg->header.payloadLen; i++) {
    //        printf("%c", (char)invMsg->msgData[i]);
    //    }
    //
    //    printf("\n");
    
    AmfObject obj;
    if(obj.amf_decode_obj(invMsg->msgData, invMsg->header.payloadLen) != invMsg->header.payloadLen) {
        printf("Error: Amf object decode failed.\n");
        return RTMP_ERROR;
    }
    
    const char *type = (char*)obj.property[0]->get_val_string();
    
//    printf("Dump %s.\n", type);
//    obj.amf_dump_obj();
    
    if(strcmp("_result", type) == 0) {
        
        uint32_t callId = (uint32_t)obj.property[1]->get_val_double();
//        printf("remote call %d response.\n", callId);
        
        RtmpRemoteCall *node = find_call_with_id(callId);
        
        if(NULL == node) {
            printf("未匹配的remote call response.\n");
            return RTMP_ERROR;
        }
        
        if(strcmp("connect", (char*)node->obj.property[0]->get_val_string()) == 0) {
            //connect消息的response
            
            if(!isPushMode) { //拉流模式
                rtmp_send_server_bw();
            } else { //推流模式
                rtmp_send_release_stream();
                rtmp_send_fc_publish();
            }
            
            rtmp_send_create_stream();
                        
        } else if(strcmp("createStream", (char*)node->obj.property[0]->get_val_string()) == 0) {
            streamId = (uint32_t)obj.property[3]->val.fVal; //获取streamId
            if(!isPushMode) {
                rtmp_send_play();
            } else {
                rtmp_send_publish();
            }
        } else if(strcmp("play", (char*)node->obj.property[0]->get_val_string()) == 0) {
            isPlaying = true;
        } else if(strcmp("publish", (char*)node->obj.property[0]->get_val_string()) == 0) {
            isPlaying = true;
        }
        
        remove_call_with_id(callId);
    } else if(strcmp("onBWDone", type) == 0) {
        if(1) {
            rtmp_send_check_bw();
        }
    } else if(strcmp("onStatus", type) == 0){
        AmfObject *subObj = obj.property[3]->val.oVal;
        AmfData *code = subObj->amf_property_by_name((uint8_t*)"code");
        
        if(strcmp((char*)code->val.sVal, "NetStream.Play.Start") == 0 ||
           strcmp((char*)code->val.sVal, "NetStream.Publish.Start") == 0) {
            isPlaying = true;
        }
        
        printf("recv status code %s.\n", code->val.sVal);
    }
    return RTMP_OK;
}


#pragma mark rtmp chunks

/*
 检查chunk列表能否合并为一个message
 */
RtmpMsg *RtmpStream::try_merge_chunks(RtmpMsgChunk **headChunk)
{
    uint32_t bodySize = 0;
    RtmpMsgChunk *p = *headChunk;
    RtmpMsgChunk *firstChunk = NULL;
    
    while(NULL != p) {
        firstChunk = p;
        bodySize += p->chunkBodyLen;
        p = p->next;
    }
    
    if(/*(firstChunk->baseHeader.chunkHeaderType == RTMP_MSG_CHUNK_HEADER_7_BYTE ||
       firstChunk->baseHeader.chunkHeaderType == RTMP_MSG_CHUNK_HEADER_11_BYTE) &&*/
       bodySize == firstChunk->chunkHeader.msgLen) { //chunk可以合并
        
        RtmpMsg *msg = new RtmpMsg;
        msg->header.msgType = firstChunk->chunkHeader.msgTypeId;
        msg->header.streamId = firstChunk->chunkHeader.msgStreamId;
        msg->header.chunkStreamId = firstChunk->baseHeader.chunkStreamId;
        msg->header.timestamp = firstChunk->chunkHeader.timestamp.timestamp;
        msg->header.payloadLen = bodySize;
        
        if(bodySize > 0) {
            msg->msgData = new uint8_t[bodySize];
        }
        
        //合并每个chunk的body
        uint32_t leftBody = bodySize;
        RtmpMsgChunk *t;
        p = *headChunk;
        while(p) {
            memcpy(msg->msgData + leftBody - p->chunkBodyLen, p->chunkBody, p->chunkBodyLen);
            leftBody -= p->chunkBodyLen;
            t = p->next;
            delete p;
            p = t;
        }
        
        *headChunk = NULL;
        return msg;
    }
    
    return NULL;
}



int RtmpStream::update_send_msg_header_cache(RtmpMsg *msg)
{
    return RTMP_OK;
}

//更新recv message header cache
int RtmpStream::update_recv_msg_header_cache(RtmpMsgChunk *chunk)
{
    if(recvChunkMap[chunk->baseHeader.chunkStreamId] != NULL) {
        /*
         只有是message的第一个chunk才更新cache
         */
        return RTMP_OK;
    }
    
    RtmpCacheMsgHeader *header = recvMsgHeaderCache[chunk->baseHeader.chunkStreamId];
    if(NULL == header) {
        header = recvMsgHeaderCache[chunk->baseHeader.chunkStreamId] = new RtmpCacheMsgHeader;
    }
    
    switch (chunk->baseHeader.chunkHeaderType) {
        case RTMP_MSG_CHUNK_HEADER_11_BYTE:
            header->timestampDelta = 0;
            header->chunkStreamId = chunk->baseHeader.chunkStreamId;
            header->msgType = chunk->chunkHeader.msgTypeId;
            header->payloadLen = chunk->chunkHeader.msgLen;
            header->streamId = chunk->chunkHeader.msgStreamId;
            header->timestamp = chunk->chunkHeader.timestamp.timestamp;
            break;
            
        case RTMP_MSG_CHUNK_HEADER_7_BYTE:
            header->chunkStreamId = chunk->baseHeader.chunkStreamId;
            header->msgType = chunk->chunkHeader.msgTypeId;
            header->payloadLen = chunk->chunkHeader.msgLen;
            header->timestampDelta = chunk->chunkHeader.timestamp.timestampDelta;
            header->timestamp += header->timestampDelta;
            
            chunk->chunkHeader.timestamp.timestamp = header->timestamp;
            break;
            
        case RTMP_MSG_CHUNK_HEADER_3_BYTE:
            header->chunkStreamId = chunk->baseHeader.chunkStreamId;
            header->timestampDelta = chunk->chunkHeader.timestamp.timestampDelta;
            header->timestamp += header->timestampDelta;
            
            chunk->chunkHeader.msgLen = header->payloadLen;
            chunk->chunkHeader.msgTypeId = header->msgType;
            chunk->chunkHeader.msgStreamId = header->streamId;
            chunk->chunkHeader.timestamp.timestamp = header->timestamp;
            break;
            
        case RTMP_MSG_CHUNK_HEADER_0_BYTE:
            header->chunkStreamId = chunk->baseHeader.chunkStreamId;
            if(header->timestampDelta > 0) {
                header->timestamp += header->timestampDelta;
            }
            
            chunk->chunkHeader.msgLen = header->payloadLen;
            chunk->chunkHeader.msgTypeId = header->msgType;
            chunk->chunkHeader.msgStreamId = header->streamId;
            chunk->chunkHeader.timestamp.timestamp = header->timestamp;
            break;
    }
    
    return RTMP_OK;
}

RtmpRemoteCall *RtmpStream::find_call_with_id(uint32_t callId)
{
    RtmpRemoteCall *prev = NULL;
    RtmpRemoteCall *node = callList;
    
    while(node != NULL) {
        uint32_t t = (uint32_t)node->obj.property[1]->get_val_double();
        
        if(t == callId) {
            return node;
        }
        
        prev = node;
        node = node->next;
    }
    return NULL;
}

int RtmpStream::remove_call_with_id(uint32_t callId)
{
    RtmpRemoteCall *prev = NULL;
    RtmpRemoteCall *node = callList;
    
    while(node != NULL) {
        uint32_t t = (uint32_t)node->obj.property[1]->get_val_double();
        
        if(t == callId) {
            if(prev) {
                prev->next = node->next;
            } else {
                callList = node->next;
            }
            delete node;
            return RTMP_OK;
        }
        
        prev = node;
        node = node->next;
    }
    return RTMP_ERROR;
}


int RtmpStream::parse_chunk_base_header(RtmpMsgChunk *chunk, uint8_t *buf, int len)
{
    if(len <= 0) { return RTMP_NEED_MORE_DATA; }
    
    int csid = buf[0] & 0x3F;
    int headerLen = 1;
    chunk->baseHeader.chunkHeaderType = (buf[0] & 0xC0) >> 6; // 获得chunkHeaderType
    switch(csid) {
        case 0:
            if(len < 2) { return RTMP_NEED_MORE_DATA; }
            csid = buf[1] + 64; //chunkStreamId为13bits
            headerLen = 2;
            break;
        case 1:
            if(len < 3) { return RTMP_NEED_MORE_DATA; }
            csid = buf[2] * 256 + buf[1] + 64; //chunkStreamId为21bits
            headerLen = 3;
            break;
        default: //chunkStreamId为5bits
            break;
    }
    chunk->baseHeader.chunkStreamId = csid;
    return headerLen;
}


int RtmpStream::parse_chunk_header(RtmpMsgChunk *chunk, uint8_t *buf, int len)
{
    int chunkHeaderLen = 0;
    
    chunk->chunkHeader.timestamp.timestamp = 0;
    
    switch(chunk->baseHeader.chunkHeaderType) {
        case RTMP_MSG_CHUNK_HEADER_11_BYTE: //chunkHeader长11字节
            if(len < 11) { return RTMP_NEED_MORE_DATA; }
            
            chunkHeaderLen = 11;
            chunk->chunkHeader.msgTypeId = (RtmpMsgType)buf[6];
            Utils::big_endian_to_local((uint8_t*)&chunk->chunkHeader.timestamp, buf, sizeof(chunk->chunkHeader.timestamp), 3);
            Utils::big_endian_to_local((uint8_t*)&chunk->chunkHeader.msgLen, buf + 3, sizeof(chunk->chunkHeader.msgLen), 3);
            Utils::little_endian_to_local((uint8_t*)&chunk->chunkHeader.msgStreamId, buf + 7, sizeof(chunk->chunkHeader.msgStreamId), 4);
            
            break;
        case RTMP_MSG_CHUNK_HEADER_7_BYTE: //chunkHeader长7字节
            if(len < 7) { return RTMP_NEED_MORE_DATA; }
            
            chunkHeaderLen = 7;
            chunk->chunkHeader.msgTypeId = (RtmpMsgType)buf[6];
            Utils::big_endian_to_local((uint8_t*)&chunk->chunkHeader.timestamp, buf, sizeof(chunk->chunkHeader.timestamp), 3);
            Utils::big_endian_to_local((uint8_t*)&chunk->chunkHeader.msgLen, buf + 3, sizeof(chunk->chunkHeader.msgLen), 3);
            
            break;
        case RTMP_MSG_CHUNK_HEADER_3_BYTE: //chunkHeader长3字节
            if(len < 3) { return RTMP_NEED_MORE_DATA; }
            
            chunkHeaderLen = 3;
            Utils::big_endian_to_local((uint8_t*)&chunk->chunkHeader.timestamp, buf, sizeof(chunk->chunkHeader.timestamp), 3);
            
            break;
            
        case RTMP_MSG_CHUNK_HEADER_0_BYTE: //chunkHeader长0字节
            break;
        default:
            printf("chunk header type error!\n");
            break;
    }
    
    if(chunk->chunkHeader.timestamp.timestamp >= 0xFFFFFF) {
        if(len < chunkHeaderLen + 4) {
            return RTMP_NEED_MORE_DATA;
        }
        Utils::big_endian_to_local((uint8_t*)&chunk->chunkHeader.timestamp, buf + chunkHeaderLen, sizeof(chunk->chunkHeader.timestamp), 4);
        chunkHeaderLen += 4;
    }
    
    return chunkHeaderLen;
}

int RtmpStream::chunk_decode(RtmpMsgChunk *chunk, uint8_t *buf, int len)
{
    int ret = 0;
    int nParsed = 0;
    
    if((ret = parse_chunk_base_header(chunk, buf, len)) < 0) {
        return ret;
    }
    
    nParsed += ret;
    
    if((ret = parse_chunk_header(chunk, buf + nParsed, len - nParsed)) < 0) {
        return ret;
    }
    
    nParsed += ret;
    
    if(chunk->chunkHeader.timestamp.timestamp == 0xFFFFFF &&
       chunk->baseHeader.chunkHeaderType != RTMP_MSG_CHUNK_HEADER_0_BYTE) {
        Utils::big_endian_to_local((uint8_t*)&chunk->chunkHeader.timestamp.timestamp, buf + nParsed, 4, 4);
        nParsed += 4;
    }
    
    bool useCacheHeader = false;
    if(recvChunkMap[chunk->baseHeader.chunkStreamId] == NULL &&
       (chunk->baseHeader.chunkHeaderType == RTMP_MSG_CHUNK_HEADER_3_BYTE ||
        chunk->baseHeader.chunkHeaderType == RTMP_MSG_CHUNK_HEADER_0_BYTE )) {
           //header 重用逻辑
           useCacheHeader = true;
           //               printf("触发header重用逻辑.\n");
           
           RtmpCacheMsgHeader *cache = recvMsgHeaderCache[chunk->baseHeader.chunkStreamId];
           if(cache == NULL) {
               printf("没有可用的cache header [csid=%d].\n", chunk->baseHeader.chunkStreamId);
               return RTMP_ERROR;
           }
           
           chunk->chunkHeader.msgLen = cache->payloadLen;
    }
    
    int nBody = inChunkSize;
    
    if(!useCacheHeader && chunk->baseHeader.chunkHeaderType != RTMP_MSG_CHUNK_HEADER_11_BYTE &&
       chunk->baseHeader.chunkHeaderType != RTMP_MSG_CHUNK_HEADER_7_BYTE) {
        nBody = min(nBody, (int)get_left_chunk_body_size(chunk->baseHeader.chunkStreamId));
    } else {
        nBody = min(nBody, (int)chunk->chunkHeader.msgLen);
    }
    
    if(len - nParsed < nBody) {
        return RTMP_NEED_MORE_DATA;
    }
    
    update_recv_msg_header_cache(chunk); //更新cache header
    
    chunk->chunkBodyLen = nBody;
    chunk->make_sure_buffer_size(chunk->chunkBodyLen);
    memcpy(chunk->chunkBody, buf + nParsed, nBody);
    
    nParsed += nBody;
    
    return nParsed;
}

int RtmpStream::chunk_encode(RtmpMsgChunk *chunk, uint8_t *buf, int len, int bodyLen)
{
    uint8_t *p = buf;
    
    //序列化chunk base header
    uint32_t chunkStreamIdLen = 1;
    uint8_t csid[3];
    if(chunk->baseHeader.chunkStreamId > 319) {
        chunkStreamIdLen = 3;
        csid[0] = 1;
        uint16_t t = chunk->baseHeader.chunkStreamId - 64;
        Utils::to_big_endian(csid + 1, (uint8_t*)&t, 2, 2);
    } else if(chunk->baseHeader.chunkStreamId >=64) {
        chunkStreamIdLen = 2;
        csid[0] = 0;
        uint8_t t = chunk->baseHeader.chunkStreamId - 64;
        csid[1] = t;
    } else {
        csid[0] = chunk->baseHeader.chunkStreamId;
    }
    
    *p++ = (chunk->baseHeader.chunkHeaderType<<6) | csid[0];
    if(chunkStreamIdLen > 1) {
        *p++ = csid[1];
        if(chunkStreamIdLen > 2) {
            *p++ = csid[2];
        }
    }
    
    bool extendTsFlag = false;
    uint32_t ts = chunk->chunkHeader.timestamp.timestamp;
    
    if(ts >= 0xFFFFFF) {
        ts = 0xFFFFFF;
        extendTsFlag = true;
    }
    
    //编码chunk header
    switch(chunk->baseHeader.chunkHeaderType) {
        case RTMP_MSG_CHUNK_HEADER_11_BYTE:
            Utils::to_big_endian(p, (uint8_t*)&ts, 3, 4);
            p += 3;
            Utils::to_big_endian(p, (uint8_t*)&chunk->chunkHeader.msgLen, 3, 4);
            p += 3;
            *p++ = chunk->chunkHeader.msgTypeId;
            Utils::to_little_endian(p, (uint8_t*)&chunk->chunkHeader.msgStreamId, 4, 4);
            p += 4;
            break;
        case RTMP_MSG_CHUNK_HEADER_7_BYTE:
            Utils::to_big_endian(p, (uint8_t*)&ts, 3, 4);
            p += 3;
            Utils::to_big_endian(p, (uint8_t*)&chunk->chunkHeader.msgLen, 3, 4);
            p += 3;
            *p++ = chunk->chunkHeader.msgTypeId;
            break;
        case RTMP_MSG_CHUNK_HEADER_3_BYTE:
            Utils::to_big_endian(p, (uint8_t*)&ts, 3, 4);
            p += 3;
            break;
        case RTMP_MSG_CHUNK_HEADER_0_BYTE:
            break;
    }
    
    if(extendTsFlag && chunk->baseHeader.chunkHeaderType != RTMP_MSG_CHUNK_HEADER_0_BYTE) {
        Utils::to_big_endian(p, (uint8_t*)&chunk->chunkHeader.timestamp.timestamp, 4, 4);
        p += 4;
    }
    
    memcpy(p, chunk->chunkBody, bodyLen);
    p += bodyLen;
    
    return (int)(p - buf);
}

uint32_t RtmpStream::get_left_chunk_body_size(uint32_t chunkStreamId)
{
    if(NULL == recvChunkMap[chunkStreamId]) {
        return 0;
    }
    
    uint32_t bodySize = 0;
    RtmpMsgChunk *first = NULL;
    RtmpMsgChunk *chunk = recvChunkMap[chunkStreamId];
    
    while(NULL != chunk) {
        first = chunk;
        bodySize += chunk->chunkBodyLen;
        chunk = chunk->next;
    }
    
    return first->chunkHeader.msgLen - bodySize;
}

#pragma mark socket

long RtmpStream::socket_recv(int sockfd, uint8_t *buf, uint32_t len, bool wait)
{
    long ret;
    long nRecv = 0;
    
    do {
        ret = recv(sockfd, buf + nRecv, len, 0);
        if(ret <= 0) {
            return ret;
        }
        
        nRecv += ret;
        len -= ret;
    }while(wait && len > 0);
    
    return nRecv;
}


long RtmpStream::socket_send(int sockfd, uint8_t *buf, uint32_t len)
{
    long ret = send(sockfd, buf, len, 0);
    if(ret <= 0) {
        int err = errno;
        printf("%s, failed to connect socket. %d (%s).\n",
                 __FUNCTION__, err, strerror(err));
    }
    
    return ret;
}


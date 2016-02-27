#ifndef __m3u8_monitor__RtmpStream__
#define __m3u8_monitor__RtmpStream__

#include "Stream.h"
#include "AmfData.h"
#include "Utils.h"
#include <iostream>
#include <algorithm>
#include <stdio.h>
using namespace std;


enum RtmpMsgType {
    RTMP_CHANGE_CHUNK_SIZE_MSG = 0x01,
    RTMP_UNKNOWN_1_MSG,
    RTMP_BYTE_READ_MSG,
    RTMP_PING_MSG,
    RTMP_SERVER_BW_MSG,
    RTMP_CLIENT_BW_MSG,
    RTMP_UNKNOWN_2_MSG,
    
    RTMP_AUDIO_MSG = 0x08,
    RTMP_VIDEO_MSG,
    
    RTMP_FLEX_SEND_MSG = 0x0F,
    RTMP_FLEX_SHARE_OBJ_MSG,
    RTMP_FLEX_TYPE_MSG,
    
    RTMP_NOTIFY_MSG,
    RTMP_SHARE_OBJ_MSG,
    
    RTMP_INVOKE_MSG,
    RTMP_STREAM_DATA_MSG,
    
    RTMP_NO_VALUE = 0xFF //未初始化的标记
};


struct RtmpMsgHeader {
    RtmpMsgType msgType;
    uint32_t payloadLen;
    uint32_t timestamp;
    
    uint32_t streamId;
    uint32_t chunkStreamId;
    
    RtmpMsgHeader() {
        msgType = RTMP_NO_VALUE;
        payloadLen = 0;
        timestamp = 0;
        streamId = 0;
        chunkStreamId = 0;
    }
};


struct RtmpCacheMsgHeader : RtmpMsgHeader {
    uint32_t timestampDelta;
    
    RtmpCacheMsgHeader() {
        timestampDelta = 0;
    }
};

struct RtmpMsg {
    RtmpMsgHeader header;
    uint8_t *msgData;
    
    RtmpMsg()
    {
        msgData = NULL;
    }
    
    ~RtmpMsg()
    {
        if(msgData) {
            delete []msgData;
            msgData = NULL;
        }
    }
};

#define RTMP_MSG_CHUNK_HEADER_11_BYTE 0 //11B
#define RTMP_MSG_CHUNK_HEADER_7_BYTE 1 //7B
#define RTMP_MSG_CHUNK_HEADER_3_BYTE 2 //3B
#define RTMP_MSG_CHUNK_HEADER_0_BYTE 3 //0B

struct RtmpMsgChunkBaseHeader {
    uint8_t chunkHeaderType;
    uint32_t chunkStreamId;
};

struct RtmpMsgChunkHeader {
    union {
        uint32_t timestamp;
        uint32_t timestampDelta;
    }timestamp;
    uint32_t msgLen;
    RtmpMsgType msgTypeId;

#define CHUNK_STREAM_ID_LIMIT 65600
    uint32_t msgStreamId;
    
};


struct RtmpMsgChunk {
    
private:
    uint32_t chunkBodyBufSize;
public:
    RtmpMsgChunkBaseHeader baseHeader;
    RtmpMsgChunkHeader chunkHeader;
    
    uint32_t chunkBodyLen; //chunkBody内的数据长度
    uint8_t *chunkBody;
    
    RtmpMsgChunk *next;
    
    RtmpMsgChunk()
    {
        chunkBodyLen = chunkBodyBufSize = 0;
        chunkBody = NULL;
        next = NULL;
    }
    ~RtmpMsgChunk()
    {
        if(chunkBody) {
            delete []chunkBody;
            chunkBody = NULL;
        }
        chunkBodyBufSize = 0;
    }
    
    int make_sure_buffer_size(uint32_t size)
    {
        if(size > chunkBodyBufSize) {
            if(chunkBody) {
                delete []chunkBody;
            }
            chunkBody = new uint8_t[size];
            chunkBodyBufSize = size;
        }
        return 0;
    }

};

struct RtmpRemoteCall {
    AmfObject obj;
    RtmpRemoteCall *next;
};

class RtmpStream : public Stream {

protected:
    
    string rtmpUrl;
    string streamUrl;
    string streamIdUrl;
    string appUrl;
    
    string host;
    
    int port;
    int sockfd;
    
    bool isPlaying;
    bool isPushMode;
    
//rtmp的一些参数集合
#define RTMP_DEFAULT_CHUNKSIZE	128
    uint32_t streamId;
    
    uint32_t inChunkSize; //接收chunk的分块长度
    uint32_t outChunkSize; //发送chunk的分块长度
    
    uint32_t clientBW; //本地带宽
    uint32_t serverBW; //server带宽
    
    RtmpMsgChunk *recvChunkMap[CHUNK_STREAM_ID_LIMIT]; //保存已接收但未合并为message的chunk
    
    
    uint32_t invokeId; //当前remote call的id
    
#define RTMP_RECV_CHUNK_BUF_LEN (65536 * 2 + 3 + 11 + 4) //rtmp chunk body 最大长度65536,加上base header,chunk header, extendTimestamp
    uint8_t recvChunkBuf[RTMP_RECV_CHUNK_BUF_LEN]; //用于接收所有chunk的缓冲区
    uint32_t chunkBufLen; //当前recvChunkBuf内的数据长度
    
    RtmpRemoteCall *callList; //保存已经发送，但还没有回复的remote call
    
    RtmpCacheMsgHeader *sendMsgHeaderCache[CHUNK_STREAM_ID_LIMIT]; //缓存发送的message header
    RtmpCacheMsgHeader *recvMsgHeaderCache[CHUNK_STREAM_ID_LIMIT]; //缓存接收的message header
    
    
//function
    int rtmp_create_connection();
    
    int parse_url(const char*, string&, string&);
    int rtmp_socket_connect();
    
    int rtmp_client_handshake();
    int rtmp_send_connect();
    int rtmp_send_create_stream();
    int rtmp_send_server_bw();
    int rtmp_send_check_bw();
    int rtmp_send_play();
    
    int rtmp_send_fc_publish();
    int rtmp_send_fc_unpublish();
    int rtmp_send_release_stream();
    int rtmp_send_delete_stream();
    
    int rtmp_send_publish();
    
    int rtmp_send_change_chunk_size(uint32_t newChunkSize);
//    int rtmp_set_acknowledge_window_size();

//rtmp接收和发送消息
    int rtmp_send_message(RtmpMsg *msg);
public:
    int rtmp_recv_message(RtmpMsg **msg);
protected:
    
    RtmpMsg *try_merge_chunks(RtmpMsgChunk **headChunk);
    
    int rtmp_msg_send_handle(RtmpMsg *msg);
    int rtmp_msg_recv_handle(RtmpMsg *msg);
    
//rtmp在收到非音视频消息的处理函数
    int rtmp_chunk_size_msg_handle(RtmpMsg *msg);
    int rtmp_ping_msg_handle(RtmpMsg *msg);
    int rtmp_server_bandwidth_msg_handle(RtmpMsg *msg);
    int rtmp_client_bandwidth_msg_handle(RtmpMsg *msg);
    int rtmp_notify_msg_handle(RtmpMsg *msg);
    virtual int rtmp_invoke_msg_handle(RtmpMsg *invMsg);
    
//缓存rtmp发送和接收消息header的函数
    int update_send_msg_header_cache(RtmpMsg *msg);
    int update_recv_msg_header_cache(RtmpMsgChunk *chunk);
    
    RtmpRemoteCall *find_call_with_id(uint32_t callId);
    int remove_call_with_id(uint32_t callId);
    

//处理chunk
    
#define RTMP_OK 0
#define RTMP_ERROR -1
#define RTMP_NEED_MORE_DATA -2
    
    //解析chunk base header
    int parse_chunk_base_header(RtmpMsgChunk *chunk, uint8_t *buf, int len);
    //解析chunk header
    int parse_chunk_header(RtmpMsgChunk *chunk, uint8_t *buf, int len);
    //解析chunk结构
    int chunk_decode(RtmpMsgChunk *chunk, uint8_t *buf, int len);
    //将chunk序列化为二进制串
    int chunk_encode(RtmpMsgChunk *chunk, uint8_t *buf, int len, int bodyLen);
    uint32_t get_left_chunk_body_size(uint32_t chunkStreamId);
    
//简单封装下send和recv函数
    long socket_send(int sockfd, uint8_t *buf, uint32_t len);
    long socket_recv(int sockfd, uint8_t *buf, uint32_t len, bool wait=false);

public:
    RtmpStream();
    ~RtmpStream();
    
    int open(const char*);
    int close();
    
    int read(char*, int, bool = true);
    int write(const char*, int);
    
    int end();
    
    int test(const char *rtmpUrl);
    int push();
};


#endif /* defined(__m3u8_monitor__RtmpStream__) */

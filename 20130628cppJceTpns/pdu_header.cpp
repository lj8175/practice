#include <stdio.h>
#include <stdint.h>

#include "pdu_header.h"

static void host2net(struct pdu_protocol_header* header);
static void net2host(struct pdu_protocol_header* header);

/*
输入：
	header qzone包头指针
	buffer 打包目标缓冲区
	len	  目标缓冲区大小
输出:
	返回值 0 成功， <0 失败
	len   打包后使用缓冲区大小
*/
int pdu_header_pack1(struct pdu_protocol_header* header, uint8_t* buffer, uint32_t *len)
{
    if (header == NULL || buffer == NULL || len == NULL) {
        return PDU_ERR_ARG_INVALID;
    }

    if (*len <= sizeof(struct pdu_protocol_header) + 2) {//加2为ISDProtocolSOH EOT
        return PDU_ERR_BUFFER_NOT_ENOUGH;
    }

    *buffer = ISDProtocolSOH ;

    struct pdu_protocol_header *dest = (struct pdu_protocol_header*)(buffer + 1);
    *dest = *header;

    *len = sizeof(struct pdu_protocol_header) + 1;

    return 0;
}

/*
输入：
	buffer 打包目标缓冲区,传入的缓冲区至少要有一个字节的额外空间来放置ETX
	buf_len	  目标缓冲区大小
	used_len  已经使用的大小，即来用设置包头的包长字段
输出:
	返回值 0 成功， <0 失败
	len   打包后使用缓冲区大小,一般为传入长度+1,用来旋转ETX
*/
int pdu_header_pack2(uint8_t* buffer, uint32_t *buf_len, uint32_t *used_len)
{
    if (buffer == NULL || buf_len == NULL || used_len == NULL) {
        return PDU_ERR_ARG_INVALID;
    }

    if (*buf_len < *used_len + 1) {
        return PDU_ERR_BUFFER_NOT_ENOUGH;
    }

    struct pdu_protocol_header *dest = (struct pdu_protocol_header*)(buffer + 1);

    dest -> len = *used_len + 1;

	host2net(dest);

    *(buffer + *used_len) = ISDProtocolEOT;

    *used_len += 1;

    return 0;

}

/*
输入：
	buffer 待解包的缓冲区
	len    待解包缓冲区长度
	header qzone包头指针
输出：
	返回值 0 成功， <0 失败
	end_len 尾增区长度
*/
int pdu_header_unpack(uint8_t* buffer, uint32_t *len, struct pdu_protocol_header* header, uint32_t* end_len)
{
    if (buffer == NULL || len == NULL || header == NULL || end_len == NULL) {
        return PDU_ERR_ARG_INVALID;
    }

    if (*len < sizeof(struct pdu_protocol_header) + 2 ||
                                            *buffer != ISDProtocolSOH ||
                                            *(buffer + *len - 1) != ISDProtocolEOT) {
        return PDU_ERR_WRONG_PKG;
    }

    *header = *(struct pdu_protocol_header*)(buffer + 1);

	net2host((struct pdu_protocol_header*)(header));

    *end_len = 1;

    *len = sizeof(struct pdu_protocol_header) + 1;

    return 0;
}

//不检查结束符，主要是用来单解包头的时候使用
int pdu_header_unpack2(uint8_t* buffer, uint32_t *len, struct pdu_protocol_header* header, uint32_t* end_len)
{
    if (buffer == NULL || len == NULL || header == NULL || end_len == NULL) {
        return PDU_ERR_ARG_INVALID;
    }

    if (*len < sizeof(struct pdu_protocol_header) + 1 ||
                                            *buffer != ISDProtocolSOH ) {
        return PDU_ERR_WRONG_PKG;
    }

    *header = *(struct pdu_protocol_header*)(buffer + 1);

	net2host((struct pdu_protocol_header*)(header));

    *end_len = 0;

    *len = sizeof(struct pdu_protocol_header) + 1;

    return 0;
}

static void host2net(struct pdu_protocol_header* header)
{
	header->cmd = (uint32_t)(htonl(header->cmd));
	header->seq = (uint32_t)(htonl(header->seq));
	header->key = (uint32_t)(htonl(header->key));
	header->len = (uint32_t)(htonl(header->len));
	header->checksum = (uint16_t)(htons(header->checksum));
	header->response_info = (uint16_t)(htons(header->response_info));
}
static void net2host(struct pdu_protocol_header* header)
{
	header->cmd = (uint32_t)(ntohl(header->cmd));
	header->seq = (uint32_t)(ntohl(header->seq));
	header->key = (uint32_t)(ntohl(header->key));
	header->len = (uint32_t)(ntohl(header->len));
	header->checksum = (uint16_t)(ntohs(header->checksum));
	header->response_info = (uint16_t)(ntohs(header->response_info));
}

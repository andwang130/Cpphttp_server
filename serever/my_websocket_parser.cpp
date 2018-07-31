//
// Created by meidai on 18-7-27.
//
#include "my_websocket_parser.h"
#include <string.h>
#include<netinet/in.h>
#include <map>
//https://github.com/BottleHe/c-demo/blob/master/websocket/websocket.c
/**
 +------------------------------------------------------------------------------
 * @desc        	: 解析接收到的数据包
 +------------------------------------------------------------------------------
 * @access	      	: public
 * @author	      	: bottle<bottle@fridayws.com>
 * @since       	: 16-05-11
 * @param       	: unsigned char* buf 接收到的数据内容
 * @param           : size_t length 接收的数据长度
 * @param           : WSHeader* 头部存放结构体
 * @return      	: int 成功返回0
 +------------------------------------------------------------------------------
**/
int parsePack(unsigned char* buf, size_t length, WSHeader* header)
{
    header->mark.fin = buf[0] >> 7;  // 或使用 buf[0] & 0x80
    header->mark.rsv1 = buf[0] & 0x40;
    header->mark.rsv2 = buf[0] & 0x20;
    header->mark.rsv3 = buf[0] & 0x10;
    header->mark.opcode = buf[0] & 0xF;
    header->mark.mask = buf[1] >> 7;
    header->mark.payloadlen = buf[1] & 0x7F;
    header->headlength = 2;
    header->reallength = header->mark.payloadlen;
    if (header->mark.payloadlen == 126) // 如果payload length 值为 0x7E的话
    {
        UINT16 tmp16 = 0; // 我们使用后面的 2 个字节存放实际数据长度
        memcpy(&tmp16, buf + 2, 2);
        header->reallength = ntohs(tmp16);  // 网络字节序转本地字节序
        header->headlength += 2;
    }
    else if (header->mark.payloadlen == 127) // 如果payload length 值为 0x7F的话
    {
        UINT64 tmp64 = 0; // 我们使用后续的 8 个字节存放实际数据长度
        memcpy(&tmp64, buf + 2, 8);
        header->reallength = ntohl(tmp64); // 网络字节序转本地字节序
        header->headlength += 8;
    }
    memset(header->mask, 0, 4);
    if (header->mark.mask)
    {
        memcpy(header->mask, buf + header->headlength, 4);
        header->headlength += 4;
    }
    return 0;
}

/**
+------------------------------------------------------------------------------
* @desc        	: 获取消息体
+------------------------------------------------------------------------------
* @access	      	: public
* @since       	: 16-05-17
* @param       	: const int cfd client fd
* @param           : const unsigned char* buf 消息buf
* @param           : size_t bufsize 消息长度
* @param           : unsigned char* container 获取到的消息体存放地址
* @param           : const WSHeader* pHeader 头结构体地址
* @return      	: int
+------------------------------------------------------------------------------
**/
int getPackPayloadData(const int cfd, const unsigned char* buf, size_t bufsize, unsigned char* container, const WSHeader* pHeader)
{
    memset(container, 0, pHeader->reallength + 1);
    int readlength = 0;
    int recvlength = 0;
    int count = 0;
    char *_buf = (char*)calloc(bufsize, sizeof(char)); // 动态分配足够大空间
    if (pHeader->mark.mask) // 如果有掩码
    {
        readlength = bufsize - pHeader->headlength;
        int x = 0;
        memcpy(container, buf + pHeader->headlength, pHeader->reallength > readlength ? readlength : pHeader->reallength);
        while(pHeader->reallength > readlength)
        {
            memset(_buf, 0, bufsize);
            count = recv(cfd, _buf, bufsize, MSG_DONTWAIT);
            recvlength = (pHeader->reallength - readlength) > bufsize ? bufsize : (pHeader->reallength - readlength);
            memcpy(container + readlength, _buf, recvlength);
            readlength += recvlength;
        }
        for (x = 0; x < pHeader->reallength; ++x)
            *(container + x) ^= pHeader->mask[x % 4];
    }
    else
    {
        readlength = bufsize - pHeader->headlength;
        memcpy(container, buf + pHeader->headlength, pHeader->reallength > readlength ? readlength : pHeader->reallength);
        while(pHeader->reallength > readlength)
        {
            memset(_buf, 0, bufsize);
            count = recv(cfd, _buf, bufsize, MSG_DONTWAIT);
            recvlength = pHeader->reallength - readlength > bufsize ? bufsize : pHeader->reallength - readlength;
            memcpy(container + readlength, _buf, recvlength);
            readlength += recvlength;
        }
    }
    free(_buf);
    _buf = NULL;
    return 0;
}
/**
 +------------------------------------------------------------------------------
 * @desc        	: 对发送数据打包
 +------------------------------------------------------------------------------
 * @author      	: Bottle<bottle.friday@gmail.com>
 * @since       	: 2016-05-11
 * @param       	: const unsigned char* message 需要发送的消息体
 * @param           : size_t len 发送数据长度
 * @param           : BYTE fin 是否是结束消息 (1 bit)
 * @param           : BYTE opcode 消息类型(4 bit) 共15种类型
 * @param           : BYTE mask (是否需要做掩码运算 1 bit)
 * @param           : unsigned char** send 输出参数, 存放处理好的数据包
 * @param           : size_t* slen 输出参数, 记录数据包的长度
 * @return      	: int 成功返回0
 +------------------------------------------------------------------------------
**/
int packData(const unsigned char* message, size_t len, BYTE fin, BYTE opcode, BYTE mask, unsigned char** send, size_t* slen)
{
    int headLength = 0;
    // 基本一个包可以发送完所有数据
    *slen = len;
    if (len < 126) // 如果不需要扩展长度位, 两个字节存放 fin(1bit) + rsv[3](1bit) + opcode(4bit); mask(1bit) + payloadLength(7bit);
        *slen += 2;
    else if (len < 0xFFFF) // 如果数据长度超过126 并且小于两个字节, 我们再用后面的两个字节(16bit) 表示 UINT16
        *slen += 4;
    else // 如果数据更长的话, 我们使用后面的8个字节(64bit)表示 UINT64
        *slen += 8;
    // 判断是否有掩码
    if (mask & 0x1) // 判断是不是1
        *slen += 4; // 4byte 掩码位
    // 长度已确定, 现在可以重新分配内存
    *send = (unsigned char*)realloc((void*)*send, *slen);
    // 做数据设置
    memset(*send, 0, *slen);
    **send = fin << 7;
    **send = **send | (0xF & opcode); //处理opcode
    *(*send + 1) = mask << 7;
    if (len < 126)
    {
        *(*send + 1) = *(*send + 1) | len;
        //start += 2;
        headLength += 2;
    }
    else if (len < 0xFFFF)
    {
        *(*send + 1) = *(*send + 1) | 0x7E; // 设置第二个字节后7bit为126
        UINT16 tmp = htons((UINT16)len);
        //UINT16 tmp = len;
        memcpy(*send + 2, &tmp, sizeof(UINT16));
        headLength += 4;
    }
    else
    {
        *(*send + 1) = *(*send + 1) | 0x7F; // 设置第二个字节后为7bit 127
        UINT64 tmp = htonl((UINT64)len);
        //UINT64 tmp = len;
        memcpy(*send + 2, &tmp, sizeof(UINT64));
        headLength += 10;
    }
    // 处理掩码
    if (mask & 0x1)
    {
        // 因协议规定, 从服务器向客户端发送的数据, 一定不能使用掩码处理. 所以这边省略
        headLength += 4;
    }
    memcpy((*send) + headLength, message, len);
    *(*send + (*slen - 1)) = '\0';
    return 0;
}
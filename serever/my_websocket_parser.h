//
// Created by meidai on 18-7-27.
//

#ifndef UNTITLED5_MY_WEBSOCKET_PARSER_H
#define UNTITLED5_MY_WEBSOCKET_PARSER_H

#endif //UNTITLED5_MY_WEBSOCKET_PARSER_H

#include <iostream>

using namespace std;
typedef unsigned char BYTE; // 定义一个BYTE类型
typedef unsigned short UINT16; // 定义一个UINT16类型
typedef unsigned long UINT64; // 定义一个UINT64类型


typedef struct _WebSocketMark {
    BYTE fin:1;
    BYTE rsv1:1;
    BYTE rsv2:1;
    BYTE rsv3:1;
    BYTE opcode:4;
    BYTE mask:1;
    BYTE payloadlen:7;
} WSMark;

typedef struct _WebSocketHeader {
    WSMark mark;
    UINT64 reallength;
    unsigned char mask[4];
    unsigned short headlength;
} WSHeader;
int parsePack(unsigned char* buf, size_t length, WSHeader* header);
int getPackPayloadData(const int cfd, const unsigned char* buf, size_t bufsize, unsigned char* container, const WSHeader* pHeader);
int packData(const unsigned char* message, size_t len, BYTE fin, BYTE opcode, BYTE mask, unsigned char** send, size_t* slen);


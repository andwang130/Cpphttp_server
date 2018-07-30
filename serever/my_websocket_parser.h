//
// Created by meidai on 18-7-27.
//

#ifndef UNTITLED5_MY_WEBSOCKET_PARSER_H
#define UNTITLED5_MY_WEBSOCKET_PARSER_H

#endif //UNTITLED5_MY_WEBSOCKET_PARSER_H

#include <iostream>

using namespace std;
enum WS_Status
{
    WS_STATUS_CONNECT = 0,
    WS_STATUS_UNCONNECT = 1,
};

enum WS_FrameType
{
    WS_EMPTY_FRAME = 0xF0,
    WS_ERROR_FRAME = 0xF1,
    WS_TEXT_FRAME   = 0x01, //0x1表示文本数据帧
    WS_BINARY_FRAME = 0x02, //0x2表示二进制数据帧
    WS_PING_FRAME = 0x09,  //ping
    WS_PONG_FRAME = 0x0A,   //pong
    WS_OPENING_FRAME = 0xF3,
    WS_CLOSING_FRAME = 0x08

};
struct WS_type{
    int fin;
    int mask;
    int payloadLength;
    int opcode;
    int payloadFieldExtraBytes;
    int lent;
};
class websocket_parser
{
public:
    int wsDecodeFrame(string inFrame, string &outMessage);
    int get_lengt(string inFrame);
    int wsEncodeFrame(string inMessage, string &outFrame, enum WS_FrameType frameType);
};
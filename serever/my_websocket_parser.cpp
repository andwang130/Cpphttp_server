//
// Created by meidai on 18-7-27.
//
#include "my_websocket_parser.h"
#include <string.h>
#include<netinet/in.h>
#include <map>
int websocket_parser::wsEncodeFrame(string inMessage, string &outFrame, enum WS_FrameType frameType)
{
    int ret = WS_EMPTY_FRAME;
    const uint32_t messageLength = inMessage.size();
    if (messageLength > 32767)
    {
        // 暂不支持这么长的数据
        return WS_ERROR_FRAME;
    }

    uint8_t payloadFieldExtraBytes = (messageLength <= 0x7d) ? 0 : 2;
    // header: 2字节, mask位设置为0(不加密), 则后面的masking key无须填写, 省略4字节
    uint8_t frameHeaderSize = 2 + payloadFieldExtraBytes;
    uint8_t *frameHeader = new uint8_t[frameHeaderSize];
    memset(frameHeader, 0, frameHeaderSize);
    // fin位为1, 扩展位为0, 操作位为frameType
    frameHeader[0] = static_cast<uint8_t>(0x80 | frameType);

    // 填充数据长度
    if (messageLength <= 0x7d)
    {
        frameHeader[1] = static_cast<uint8_t>(messageLength);
    }
    else
    {
        frameHeader[1] = 0x7e;
        uint16_t len = htons(messageLength);
        memcpy(&frameHeader[2], &len, payloadFieldExtraBytes);
    }

    // 填充数据
    uint32_t frameSize = frameHeaderSize + messageLength;
    char *frame = new char[frameSize + 1];
    memcpy(frame, frameHeader, frameHeaderSize);
    memcpy(frame + frameHeaderSize, inMessage.c_str(), messageLength);
    frame[frameSize] = '\0';
    outFrame = frame;

    delete[] frame;
    delete[] frameHeader;
    return ret;
}
int websocket_parser::get_lengt(string inFrame)
{

    struct WS_type oput_type;
    int ret = WS_OPENING_FRAME;
    const char *frameData = inFrame.c_str();
    const int frameLength = inFrame.size();
    if (frameLength < 2)
    {
        ret = WS_ERROR_FRAME;
    }
    // 检查扩展位并忽略
    if ((frameData[0] & 0x70) != 0x0)
    {
        ret = WS_ERROR_FRAME;
    }

    // fin位: 为1表示已接收完整报文, 为0表示继续监听后续报文
    ret = (frameData[0] & 0x80);
    oput_type.fin=ret;
    if ((frameData[0] & 0x80) != 0x80)
    {

        ret = WS_ERROR_FRAME;
    }

    // mask位, 为1表示数据被加密
    oput_type.mask=frameData[1] & 0x80;
    if ((frameData[1] & 0x80) != 0x80)
    {
        ret = WS_ERROR_FRAME;
    }

    // 操作码
    uint16_t payloadLength = 0;
    uint8_t payloadFieldExtraBytes = 0;
    uint8_t opcode = static_cast<uint8_t >(frameData[0] & 0x0f);
    if (opcode == WS_TEXT_FRAME)
    {
        // 处理utf-8编码的文本帧
        payloadLength = static_cast<uint16_t >(frameData[1] & 0x7f);
        if (payloadLength == 0x7e)
        {
            uint16_t payloadLength16b = 0;
            payloadFieldExtraBytes = 2;
            memcpy(&payloadLength16b, &frameData[2], payloadFieldExtraBytes);
            payloadLength = ntohs(payloadLength16b);

        }
        else if (payloadLength == 0x7f)
        {
            // 数据过长,暂不支持
            uint16_t payloadLength16b = 0;
            payloadFieldExtraBytes = 4;
            memcpy(&payloadLength16b, &frameData[2], payloadFieldExtraBytes);
            payloadLength = ntohs(payloadLength16b);


        }
    }

    else if (opcode == WS_BINARY_FRAME || opcode == WS_PING_FRAME || opcode == WS_PONG_FRAME)
    {
        // 二进制/ping/pong帧暂不处理
    }
    else if (opcode == WS_CLOSING_FRAME)
    {
        ret = WS_CLOSING_FRAME;
    }
    else
    {
        ret = WS_ERROR_FRAME;
    }
    oput_type.payloadLength=payloadLength;
    oput_type.opcode=opcode;
    oput_type.payloadFieldExtraBytes=payloadFieldExtraBytes;

}

int websocket_parser::wsDecodeFrame(string inFrame, string &outMessage)
{

        struct WS_type ws_type;
        int ret = WS_OPENING_FRAME;


        const char *frameData = inFrame.c_str();
        const int frameLength = inFrame.size();


        if (frameLength < 2)
        {
            ret = WS_ERROR_FRAME;
        }


        // 检查扩展位并忽略
        if ((frameData[0] & 0x70) != 0x0)
        {
            ret = WS_ERROR_FRAME;
        }


        // fin位: 为1表示已接收完整报文, 为0表示继续监听后续报文
        ret = (frameData[0] & 0x80);
        ws_type.fin=ret;
//        if ((frameData[0] & 0x80) != 0x80)
//        {
//            ret = WS_ERROR_FRAME;
//        }


        // mask位, 为1表示数据被加密
        int mask=(frameData[1] & 0x80);
//        if ((frameData[1] & 0x80) != 0x80)
//        {
//            ret = WS_ERROR_FRAME;
//        }


        // 操作码
        uint16_t payloadLength = 0;
        uint8_t payloadFieldExtraBytes = 0;
        uint8_t opcode = static_cast<uint8_t>(frameData[0] & 0x0f);


        if (opcode == WS_TEXT_FRAME)
        {
            // 处理utf-8编码的文本帧
            payloadLength = static_cast<uint16_t>(frameData[1] & 0x7f);


            if (payloadLength == 0x7e)
            {
                uint16_t payloadLength16b = 0;


                payloadFieldExtraBytes = 2;


                memcpy(&payloadLength16b, &frameData[2], payloadFieldExtraBytes);


                payloadLength = ntohs(payloadLength16b);
            }
            else if (payloadLength == 0x7f)
            {
                // 数据过长,暂不支持
                uint16_t payloadLength16b = 0;
                payloadFieldExtraBytes = 4;
                memcpy(&payloadLength16b, &frameData[2], payloadFieldExtraBytes);
                payloadLength = ntohs(payloadLength16b);

            }
        }
        else if (opcode == WS_BINARY_FRAME || opcode == WS_PING_FRAME || opcode == WS_PONG_FRAME)
        {
            // 二进制/ping/pong帧暂不处理
        }
        else if (opcode == WS_CLOSING_FRAME)
        {
            ret = WS_CLOSING_FRAME;
        }
        else
        {
            ret = WS_ERROR_FRAME;
        }


        // 数据解码
        const char *maskingKey = &frameData[2 + payloadFieldExtraBytes];
        char *payloadData = new char[payloadLength + 1];
        if ((mask != 1) && (payloadLength > 0)) {
            // header: 2字节, masking key: 4字节


            memset(payloadData, 0, payloadLength + 1);
            memcpy(payloadData, &frameData[2 + payloadFieldExtraBytes + 4], payloadLength);
        }
        else{

            for (int i = 0; i < payloadLength; i++)
            {
                payloadData[i] = payloadData[i] ^ maskingKey[i % 4];
            }


        }
        outMessage = payloadData;
         delete[] payloadData;

        int lent=2+payloadFieldExtraBytes+payloadLength-frameLength;
         ws_type.mask=mask;
         ws_type.payloadFieldExtraBytes=payloadFieldExtraBytes;
         ws_type.payloadLength=payloadLength;
         ws_type.lent=lent;
        return ret;
    }

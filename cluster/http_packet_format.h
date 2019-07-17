#ifndef HTTP_PACKET_FORMAT_H
#define HTTP_PACKET_FORMAT_H

#include <qglobal.h>
#include <map>

namespace ZP_Cluster
{
#pragma pack(push, 1)

    //报文由三个部分组成：
    //  对报文进行描述的起始行
    //  包含属性的首部块
    //  包含数据的主体部分(可选)

    //请求行
    //Request-Line = Method SP Request-URL SP HTTP-Version CRLF
    typedef struct tag_request_line
    {
        quint32 method;         //客户端希望服务器对资源执行的操作。比如 GET、POST 等
        quint8 request_URL[1];  //请求 URL
        quint8 http_version[16];//报文所使用的 HTTP 版本
    } RequestLine;

    //状态行
    //Status-Line = HTTP-Version SP Status-Code SP Reason-Phrase CRLF
    typedef struct tag_status_line
    {
        quint8 http_version[16]; //报文所使用的 HTTP 版本
        quint32 status_code;     //描述请求过程所发生的情况的数字
        quint8 reason_phrase[32];//数字状态码的文字描述版本
    } StatusLine;

    typedef std::map<std::string, std::string> header_type;
    typedef header_type::iterator header_iter_type;
    typedef struct tag_http_packet_format
    {
        union union_start_line
        {
            RequestLine sl_request_line;
            StatusLine sl_status_line;
        } start_line;
        header_type headers;
        std::string header_field;
        std::string entity_body;
    } HTTP_PACKET_FORMAT;

#pragma pack(pop)
}

#endif // HTTP_PACKET_FORMAT_H

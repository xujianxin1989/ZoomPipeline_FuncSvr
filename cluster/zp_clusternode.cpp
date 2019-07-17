#include "zp_clusternode.h"
#include "zp_clusterterm.h"
#include <assert.h>
#include <QHostInfo>
#include <QXmlStreamReader>

namespace ZP_Cluster
{
    zp_ClusterNode::zp_ClusterNode(zp_ClusterTerm* pTerm,
                                   QObject* pSocket,
                                   QObject* parent) :
		ZPTaskEngine::zp_plTaskBase(parent),
		bTermSet(false),
		m_pTerm(pTerm),
		m_pSocket(pSocket)
	{
		m_currentReadOffset = 0;
		m_currentMessageSize = 0;
        m_nPortLAN = 0;
        m_nPortPub = 0;
		m_last_Report = QDateTime::currentDateTime();
		m_nRemoteClientNums = 0;

        parser_init();
        m_responsePacket = nullptr;
        m_requestPacket = nullptr;
	}

	quint32 zp_ClusterNode::clientNums() const
	{
        return m_nRemoteClientNums;
    }

    HTTP_PACKET_FORMAT *zp_ClusterNode::requestPacket() const
    {
        return m_requestPacket;
    }

    HTTP_PACKET_FORMAT *zp_ClusterNode::responsePacket() const
    {
        return m_responsePacket;
    }

    void zp_ClusterNode::parser_init()
    {
        http_parser_settings_init(&m_settings);
        m_settings.on_message_begin = on_message_begin_callback;
        m_settings.on_url = on_url_callback;
        m_settings.on_status = on_status_callback;
        m_settings.on_header_field = on_header_field_callback;
        m_settings.on_header_value = on_header_value_callback;
        m_settings.on_headers_complete = on_headers_complete_callback;
        m_settings.on_body = on_body_callback;
        m_settings.on_message_complete = on_message_complete_callback;
    }

    int zp_ClusterNode::on_message_begin_callback(http_parser* parser)
    {
        zp_ClusterNode *node = (zp_ClusterNode *)parser->data;
        //我们先假定 http_version 为 HTTP/1.1，所以暂时不对该字段进行处理
        if (parser->type == HTTP_REQUEST)
        {
            node->m_requestPacket = new HTTP_PACKET_FORMAT();
            strcpy((char*)node->m_requestPacket->start_line.sl_request_line.http_version, "HTTP/1.1");
        }
        else
        {
            node->m_responsePacket = new HTTP_PACKET_FORMAT();
            strcpy((char*)node->m_responsePacket->start_line.sl_status_line.http_version, "HTTP/1.1");
        }

        return 0;
    }

    int zp_ClusterNode::on_url_callback(http_parser* parser, const char* buf, size_t len)
    {
        zp_ClusterNode* node = (zp_ClusterNode*)parser->data;
        memcpy(node->m_requestPacket->start_line.sl_request_line.request_URL, buf, len);
        return 0;
    }

    int zp_ClusterNode::on_status_callback(http_parser* parser, const char* buf, size_t len)
    {
        zp_ClusterNode* node = (zp_ClusterNode*)parser->data;
        memcpy(node->m_responsePacket->start_line.sl_status_line.reason_phrase, buf, len);
        return 0;
    }

    int zp_ClusterNode::on_header_field_callback(http_parser* parser, const char* buf, size_t len)
    {
        zp_ClusterNode* node = (zp_ClusterNode*)parser->data;
        if (parser->type == HTTP_REQUEST)
        {
            node->m_requestPacket->header_field.assign(buf, len);
        }
        else
        {
            node->m_responsePacket->header_field.assign(buf, len);
        }

        return 0;
    }

    int zp_ClusterNode::on_header_value_callback(http_parser* parser, const char* buf, size_t len)
    {
        zp_ClusterNode* node = (zp_ClusterNode*)parser->data;
        if (parser->type == HTTP_REQUEST)
        {
            node->m_requestPacket->headers.insert(std::make_pair(node->m_requestPacket->header_field, std::string(buf, len)));
        }
        else
        {
            node->m_responsePacket->headers.insert(std::make_pair(node->m_responsePacket->header_field, std::string(buf, len)));
        }
        return 0;
    }

    int zp_ClusterNode::on_headers_complete_callback(http_parser* parser)
    {
        zp_ClusterNode* node = (zp_ClusterNode*)parser->data;
        if (parser->type == HTTP_REQUEST)
        {
            node->m_requestPacket->start_line.sl_request_line.method = parser->method;
        }
        else
        {
            node->m_responsePacket->start_line.sl_status_line.status_code = parser->status_code;
        }
        return 0;
    }

    int zp_ClusterNode::on_body_callback(http_parser* parser, const char* buf, size_t len)
    {
        zp_ClusterNode* node = (zp_ClusterNode*)parser->data;
        node->m_responsePacket->entity_body.append(buf, len);
        return 0;
    }

    int zp_ClusterNode::on_message_complete_callback(http_parser* parser)
    {
        if (parser->type == HTTP_RESPONSE)
        {
            zp_ClusterNode* node = (zp_ClusterNode*)parser->data;
            node->deal_current_message_block();
        }
        return 0;
    }

    void zp_ClusterNode::sendOptionsPackage()
    {
        //Request 消息
        //Request = Request-Line
        //          *(( general-header
        //            | request-header（跟本次请求相关的一些header）
        //            | entity-header ) CRLF)（跟本次请求相关的一些header）
        //          CRLF
        //          [ message-body ]
        //请求行的定义：
        //Request-Line = Method SP Request-URL SP HTTP-Version CRLF
        //方法的定义
        //Method = "OPTIONS" | "GET" | "HEAD" | "POST" | "PUT" | "DELETE" | "TRACE" | "CONNECT" | extension-method
        //"OPTIONS" = 用来查询针对请求 URI 指定的资源支持的方法
        //"GET" = 用来请求访问已被 URI 识别的资源
        //"HEAD" = 用于确认 URI 的有效性及资源更新的日期时间等
        //"POST" = 用来传输实体的主体，虽然 "GET" 方法也可以传输实体的主体，但一般不用 "GET" 方法进行传输
        //         而是用 "POST" 方法。"POST" 的主要目的并不是获取响应的主体内容
        //"PUT" = 用来传输文件
        //"DELETE" = 用来删除文件
        //"TRACE" = 追踪路径，让 Web 服务器将之前的请求通信环回给客户端
        //"CONNECT" = 要求在与代理服务器通信时建立隧道，实现用隧道协议进行 TCP 通信
        //资源地址的定义
        //Request-URI = "*" | absoluteURI | abs_path | authotity（CONNECT）

        //QString strRequest = QString("POST /WebServices/WeatherWebService.asmx/getWeatherbyCityName HTTP/1.1\r\n"
        //                             "Host: www.webxml.com.cn\r\n"
        //                             "User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:50.0) Gecko/20100101 Firefox/50.0\r\n"
        //                             "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
        //                             "Accept-Language: zh-CN,zh;q=0.8,en-US;q=0.5,en;q=0.3\r\n"
        //                             "Accept-Encoding: gzip, deflate\r\n"
        //                             "Connection: keep-alive\r\n"
        //                             "Content-Type: application/x-www-form-urlencoded\r\n"
        //                             "Content-Length: 30\r\n\r\n"
        //                             "theCityName=%E5%8C%97%E4%BA%AC");

        QString strRequest = QString("OPTIONS * HTTP/1.1\r\n"
                                     "Host: www.webxml.com.cn\r\n\r\n");
        m_parser.data = this;
        http_parser_init(&m_parser, HTTP_REQUEST);
        int parsed = http_parser_execute(&m_parser, &m_settings, strRequest.toStdString().c_str(), strRequest.length());
        assert(parsed == strRequest.length());

        QByteArray array = strRequest.toLatin1();
        emit evt_SendDataToClient(socket(), array);
    }

    int zp_ClusterNode::parse_http_packet(const QByteArray& block, int offset)
    {
        const int blockLength = block.length();
        const char* dataptr = block.constData();
        m_parser.data = this;
        http_parser_init(&m_parser, HTTP_RESPONSE);
        while (offset < blockLength)
        {            
            int parsed = http_parser_execute(&m_parser,
                                             &m_settings,
                                             dataptr + offset,
                                             blockLength - offset);
            offset += parsed;
        }
        return offset;
    }

	QString zp_ClusterNode::termName()
	{
		return m_strTermName;
	}

	QHostAddress zp_ClusterNode::addrLAN()
	{
		return m_addrLAN;
	}

	int zp_ClusterNode::portLAN()
	{
		return m_nPortLAN;
	}

	QHostAddress zp_ClusterNode::addrPub()
	{
		return m_addrPub;
	}

	int zp_ClusterNode::portPub()
	{
		return m_nPortPub;
	}

    QObject* zp_ClusterNode::socket()
	{
        return m_pSocket;
	}

	int zp_ClusterNode::run()
	{
        if (bTermSet)
		{
			return 0;
		}
        int nCurSz = -1;
        //nMessageBlockSize 是静态变量，表示最多处理几个块就释放 CPU 给其他节点
		int nMessage = m_nMessageBlockSize;
        while (--nMessage >= 0 && nCurSz != 0)
		{
			QByteArray block;
			m_mutex_rawData.lock();
            if (m_list_rawData.size())
            {
                block = *m_list_rawData.begin();
            }
			m_mutex_rawData.unlock();

            if (!block.isEmpty())
			{
                //对信息进行初步处理，输入当前队列的首部、处理偏移，返回新的处理偏移
                //m_currentReadOffset = filter_message(block, m_currentReadOffset);
                m_currentReadOffset = parse_http_packet(block, m_currentReadOffset);
				if (m_currentReadOffset >= block.size())
				{
					m_mutex_rawData.lock();
                    if (!m_list_rawData.empty())
                    {
                        m_list_rawData.pop_front();
                    }
					else
                    {
                        assert(false);
                    }
					m_currentReadOffset = 0;
					m_mutex_rawData.unlock();
				}
			}
			else
			{
				m_mutex_rawData.lock();
                if (!m_list_rawData.empty())
                {
                    m_list_rawData.pop_front();
                }
				m_mutex_rawData.unlock();
			}
			m_mutex_rawData.lock();
            nCurSz = m_list_rawData.size();
			m_mutex_rawData.unlock();
		}

		m_mutex_rawData.lock();
        nCurSz = m_list_rawData.size();
		m_mutex_rawData.unlock();

        return nCurSz == 0 ? 0 : -1;
	}

	//push new binary data into queue
    int zp_ClusterNode::push_new_data(const QByteArray& array)
	{
		int res;
		m_mutex_rawData.lock();
        m_list_rawData.push_back(array);
        res = m_list_rawData.size();
		m_mutex_rawData.unlock();
		m_last_Report = QDateTime::currentDateTime();
		return res;
	}

    //!deal one message, affect m_currentReadOffset, m_currentMessageSize, m_currentHeader
	//!return bytes Used.
    int zp_ClusterNode::filter_message(const QByteArray& block, int offset)
	{
        const int blockLength = block.length();
        const char* dataptr = block.constData();
        while (offset < blockLength)
		{            
			//Recieve First 2 byte
            //先确保信息的头标志被接收
            while (m_currentMessageSize < 2 && offset < blockLength)
			{
				m_currentBlock.push_back(dataptr[offset++]);
				m_currentMessageSize++;
			}
            //First 2 byte not complete
            if (m_currentMessageSize < 2)
            {
                continue;
            }
            if (m_currentMessageSize == 2)
			{
                const char* headerptr = m_currentBlock.constData();
                memcpy((void *)&m_currentHeader, headerptr, 2);
			}

            //判断头两个字节是不是1234
			if (m_currentHeader.Mark == 0x1234)
            {
                quint32 tagheaderSize = sizeof(CROSS_SVR_MSG::tag_header);
                //试图接收完整的头部信息
                if ((quint32)m_currentMessageSize < tagheaderSize && offset < blockLength)
				{
                    int nCopy = tagheaderSize - m_currentMessageSize;
                    if (nCopy > blockLength - offset)
                    {
                        nCopy = blockLength - offset;
                    }
                    QByteArray aryCopy(dataptr + offset, nCopy);
                    m_currentBlock.push_back(aryCopy);
                    m_currentMessageSize += nCopy;
                    offset += nCopy;
				}
                //如果头部还没收完则返回
                if ((quint32)m_currentMessageSize < tagheaderSize)
                {
                    continue;
                }
                //除了头部以外，还有数据可用，并且头部刚刚接收完
                else if ((quint32)m_currentMessageSize == tagheaderSize)
				{
                    //保存头部
                    const char* headerptr = m_currentBlock.constData();
                    memcpy((void*)&m_currentHeader, headerptr, tagheaderSize);

					//continue reading if there is data left behind
                    //继续处理后续的载荷
                    if (blockLength > offset)
					{
                        //确定还有多少字节没有接收
                        qint32 bitLeft = m_currentHeader.data_length;
                        //继续接收载荷
                        if (bitLeft > 0 && blockLength > offset)
						{
                            int nCopy = bitLeft;
                            if (nCopy > blockLength - offset)
                            {
                                nCopy = blockLength - offset;
                            }
                            QByteArray aryCopy(dataptr + offset, nCopy);
                            m_currentBlock.push_back(aryCopy);
                            m_currentMessageSize += nCopy;
                            bitLeft -= nCopy;
                            offset += nCopy;
						}
						//deal block, may be send data as soon as possible;
						deal_current_message_block();
                        if (bitLeft > 0)
                        {
                            continue;
                        }
						//This Message is Over. Start a new one.
						m_currentMessageSize = 0;
						m_currentBlock = QByteArray();
						continue;
					}
				}
                //除了头部以外，还有数据可用
				else
				{
                    if (blockLength > offset)
					{
                        //确定还有多少字节没有接收
                        qint32 bitLeft = m_currentHeader.data_length + tagheaderSize - m_currentMessageSize;
                        //while (bitLeft > 0 && blocklen > offset)
                        //继续接收载荷
                        if (bitLeft > 0 && blockLength > offset)
						{
                            int nCopy = bitLeft;
                            if (nCopy > blockLength - offset)
                            {
                                nCopy = blockLength - offset;
                            }
                            QByteArray aryCopy(dataptr + offset, nCopy);
                            m_currentBlock.push_back(aryCopy);
                            m_currentMessageSize += nCopy;
                            bitLeft -= nCopy;
                            offset += nCopy;
						}
						//deal block, may be processed as soon as possible;
						deal_current_message_block();
                        if (bitLeft > 0)
                        {
                            continue;
                        }
						//This Message is Over. Start a new one.
						m_currentMessageSize = 0;
						m_currentBlock = QByteArray();
						continue;
					}
                } //end if there is more bytes to append
			} //end deal trans message
			else
			{
                const char* ptrCurData = m_currentBlock.constData();
                emit evt_Message(this, tr("Client Send a unknown start Header %1 %2. Close client immediately.")
                                 .arg((int)(ptrCurData[0])).arg((int)(ptrCurData[1])));
				m_currentMessageSize = 0;
				m_currentBlock = QByteArray();
                offset = blockLength;
                emit evt_Close_client(socket());
			}
        } //end while block len > offset

		return offset;
	}

	//in Trans-Level, do nothing.
	int zp_ClusterNode::deal_current_message_block()
	{
        if (m_responsePacket->start_line.sl_status_line.status_code != 200)
        {
            emit evt_Message(this, QString("Service request results incorrect, reason phrase:%1 ")
                                   .arg((const char*)m_responsePacket->start_line.sl_status_line.reason_phrase));
            emit evt_Close_client(this->socket());
            return 0;
        }

        //!0 - DELETE
        //!1 - GET
        //!2 - HEAD
        //!3 - POST
        //!4 - PUT
        //!5 - CONNECT
        //!6 - OPTIONS
        //!7 - TRACE
        switch (m_requestPacket->start_line.sl_request_line.method)
        {
            case 6://OPTIONS
            {
                //Web 服务支持的方法
                header_iter_type it = m_responsePacket->headers.find("Allow");
                if (it == m_responsePacket->headers.end())
                {
                    emit evt_Message(this, "The response is not in the correct format.");
                    break;
                }
                std::string str = it->second;
                QString strMethod = QString(str.data());
                if (!strMethod.contains("POST"))
                {
                    emit evt_Message(this, "This Web service does not support the OPTION method.");
                    break;
                }

                it = m_requestPacket->headers.find("Host");
                if (it == m_requestPacket->headers.end())
                {
                    emit evt_Message(this, "The request is not in the correct format.");
                    break;
                }
                str = it->second;
                QString strName = QString(str.data());
                if (strName == m_pTerm->termName())
                {
                    emit evt_Message(this, tr("Can not connect to it-self, Loopback connections is forbidden."));
                    emit evt_Close_client(socket());
                    break;
                }
                m_strTermName = strName;
                m_nPortLAN = 80; //The default port 80
                //根据主机名，使用类 QHostInfo 来获取该主机的 IP 地址
                QString ipAddress;
                QHostInfo info = QHostInfo::fromName(m_strTermName);
                foreach (const QHostAddress& address, info.addresses())
                {
                    if (address.protocol() != QAbstractSocket::IPv4Protocol)
                    {
                        continue;
                    }
                    ipAddress = address.toString();
                    break;
                }
                m_addrLAN = QHostAddress(ipAddress);
                m_nPortPub = 80;
                m_addrPub = QHostAddress(QHostAddress::LocalHost);
                if (m_pTerm->registNewServer(this))
                {
                    emit evt_NewSvrConnected(m_strTermName);
                }
                else
                {
                    m_strTermName.clear();
                    emit evt_Message(this, tr("Info: New Svr already registed. Ignored.") + strName);
                    emit evt_Close_client(socket());
                }
                break;
            }
            case 3://POST
            {
                QString strBody = QString(m_responsePacket->entity_body.data());
                if (strBody.isEmpty())
                {
                    emit evt_Message(this, "Message body is empty.");
                    break;
                }
                bool bResult = false;
                QXmlStreamReader xmlReader(strBody);
                while (xmlReader.atEnd())
                {
                    if (xmlReader.hasError())
                    {
                        emit evt_Message(this, "Info: Xml error.");
                        break;
                    }
                    else if (xmlReader.isStartElement())
                    {
                        if (xmlReader.name() == "string")
                        {
                            bResult = xmlReader.readElementText().toInt() > 0;
                            xmlReader.clear();
                        }
                        else
                        {
                            xmlReader.readNext();
                        }
                    }
                    else
                    {
                        xmlReader.readNext();
                    }
                }
                xmlReader.clear();
                emit evt_Message(this, bResult ? "Info: Data upload complete."
                                               : "Info: Data upload failed.");

                break;
            }
            case 7://TRACE
            {
                break;
            }
            default:
                emit evt_Message(this, tr("Info: Unknown Msg Type got."));
                emit evt_Close_client(socket());
                break;
        }

//        quint32 tagHeaderSize = sizeof(CROSS_SVR_MSG::tag_header);
//        quint32 bytesLeft = tagHeaderSize + m_currentHeader.data_length - m_currentMessageSize;
//        if (0 != bytesLeft)
//        {
//            return 0;
//        }
//        const CROSS_SVR_MSG* pMsg = (const CROSS_SVR_MSG*)m_currentBlock.constData();
//        switch (m_currentHeader.message_type)
//		{
//            case 0x00://Heart Beating
//                m_nRemoteClientNums = pMsg->payload.heartBeating.nClients;
//                break;
//            case 0x01://BasicInfo, when connection established, this message should be used
//            {
//                QString strName((const char*)pMsg->payload.basicInfo.name);
//                if (strName == m_pTerm->termName())
//                {
//                    emit evt_Message(this, tr("Can not connect to it-self, Loopback connections is forbidden."));
//                    emit evt_Close_client(socket());
//                    break;
//                }

//                m_strTermName = strName;
//                m_nPortLAN = pMsg->payload.basicInfo.port_LAN;
//                m_addrLAN = QHostAddress((const char*)pMsg->payload.basicInfo.address_LAN);
//                m_nPortPub = pMsg->payload.basicInfo.port_Pub;
//                m_addrPub = QHostAddress((const char*)pMsg->payload.basicInfo.address_Pub);
//                if (m_pTerm->registNewServer(this))
//                {
//                    emit evt_NewSvrConnected(m_strTermName);
//                    m_pTerm->broadcastServers();
//                }
//                else
//                {
//                    m_strTermName.clear();
//                    emit evt_Message(this, tr("Info: New Svr already registed. Ignored.") + strName);
//                    emit evt_Close_client(socket());
//                }

//                break;
//            }
//            case 0x02://Server broadcast messages
//            {
//                int nSvrs = pMsg->hearder.data_length / sizeof(CROSS_SVR_MSG::uni_payload::tag_CSM_Broadcast);
//                for (int i = 0; i < nSvrs; i++)
//                {
//                    QString strName((const char*)pMsg->payload.broadcastMsg[i].name);
//                    if (strName == m_pTerm->termName() || NULL != m_pTerm->svrNodeFromName(strName))
//                    {
//                        continue;
//                    }

//                    if (strName > m_pTerm->termName())
//                    {
//                        QHostAddress address((const char*)pMsg->payload.broadcastMsg[i].address_LAN);
//                        quint16 port = pMsg->payload.broadcastMsg[i].port_LAN;
//                        emit evt_Connect_to(address, port, false);
//                    }
//                    else
//                    {
//                        emit evt_Message(this, tr("Name %1 <= %2, omitted.").arg(strName).arg(m_pTerm->termName()));
//                    }
//                }
//                break;
//            }
//            case 0x03:
//                if (m_currentMessageSize == m_currentBlock.size())
//                {
//                    QByteArray arraySend((const char*)pMsg + tagHeaderSize,
//                                         m_currentMessageSize - tagHeaderSize);

//                    if (deal_user_data(arraySend))
//                    {
//                        m_currentBlock = QByteArray();
//                    }
//                }
//                else
//                {
//                    if (deal_user_data(m_currentBlock))
//                    {
//                        m_currentBlock = QByteArray();
//                    }
//                }
//                break;
//            default:
//                emit evt_Message(this, tr("Info: Unknown Msg Type got."));
//                emit evt_Close_client(socket());
//                break;
//		};

		return 0;
	}

    bool zp_ClusterNode::deal_user_data(QByteArray data)
	{
        emit evt_RemoteData_recieved(termName(), data);
		return true;
	}

    void zp_ClusterNode::checkHeartBeating()
	{
		QDateTime dtm = QDateTime::currentDateTime();
        qint64 usc = m_last_Report.secsTo(dtm);
        int nThrdHold = m_pTerm->heartBeatingThrdHold();
        if (usc < nThrdHold)
        {
            return;
        }
        emit evt_Message(this, tr("Client ") + QString("%1").arg((quint64)this) + tr(" is dead, kick out."));
        emit evt_Close_client(socket());
	}

    void zp_ClusterNode::sendHelloPackage()
	{
        int nMsgLen = sizeof(CROSS_SVR_MSG::tag_header) + sizeof(CROSS_SVR_MSG::uni_payload::tag_CSM_BasicInfo);
        QByteArray array(nMsgLen, 0);
        CROSS_SVR_MSG* pMsg = (CROSS_SVR_MSG*)(array.data());
        pMsg->hearder.Mark = 0x1234;
        pMsg->hearder.message_type = 0x01;
        pMsg->hearder.data_length = sizeof(CROSS_SVR_MSG::uni_payload::tag_CSM_BasicInfo);

        strncpy((char*)(pMsg->payload.basicInfo.name),
                m_pTerm->termName().toStdString().c_str(),
                sizeof(pMsg->payload.basicInfo.name) - 1);

        strncpy((char*)(pMsg->payload.basicInfo.address_LAN),
                m_pTerm->LANAddr().toString().toStdString().c_str(),
                sizeof(pMsg->payload.basicInfo.address_LAN) - 1);

        pMsg->payload.basicInfo.port_LAN = m_pTerm->LANPort();

        strncpy((char*)(pMsg->payload.basicInfo.address_Pub),
                m_pTerm->PublishAddr().toString().toStdString().c_str(),
                sizeof(pMsg->payload.basicInfo.address_Pub) - 1);

        pMsg->payload.basicInfo.port_Pub = m_pTerm->PublishPort();

        emit evt_SendDataToClient(socket(), array);
	}
}

#ifndef ZP_CLUSTERNODE_H
#define ZP_CLUSTERNODE_H

#include <QHostAddress>
#include <QDateTime>
#include "../pipeline/zp_pltaskbase.h"
#include "cross_svr_messages.h"
#include "http_packet_format.h"
#include "../http-parser/http_parser.h"

namespace ZP_Cluster
{
	class zp_ClusterTerm;
	/**
	 * @brief This class stand for a remote server.
     * when local server establish a connection between itself and remote server,
	 * a zp_ClusterNode object will be created.
	 */
	class zp_ClusterNode : public ZPTaskEngine::zp_plTaskBase
	{
		Q_OBJECT

	public:
        explicit zp_ClusterNode(zp_ClusterTerm *pTerm,
                                QObject *pSocket,
                                QObject *parent = nullptr);

		int run() override;
		//push new binary data into queue
        int push_new_data(const QByteArray &array);
        void checkHeartBeating();
        bool bTermSet;

	protected:
		//!deal at most m_nMessageBlockSize messages per deal_message();
		static const int m_nMessageBlockSize = 8;
        //!deal one message, affect m_currentReadOffset, m_currentMessageSize, m_currentHeader
		//!return bytes Used.
        int filter_message(const QByteArray &block, int offset);
		//!in Trans-Layer, it does nothing.
		int deal_current_message_block();
		//!virtual functions, dealing with the user-defined operations.
        virtual bool deal_user_data(QByteArray);

        int parse_http_packet(const QByteArray &block, int offset);

	public:
		QString termName();

		QHostAddress addrLAN();
        int portLAN();
		QHostAddress addrPub();
        int portPub();
        QObject *socket();

		//!Messages
        void sendHelloPackage();
        void sendOptionsPackage();
        quint32 clientNums() const;

        HTTP_PACKET_FORMAT *requestPacket() const;
        HTTP_PACKET_FORMAT *responsePacket() const;

	protected:
        zp_ClusterTerm *m_pTerm;
		//Client socket handle of this connection
        QObject *m_pSocket;

		//the data members.
        QString m_strTermName;	//the Terminal's name

		QHostAddress m_addrLAN;	//The LAN address for other terms to connect to
		int m_nPortLAN;			//The LAN port for other terms to connect to

		QHostAddress m_addrPub;	//The Publish address for clients to connect to
		int m_nPortPub;			//The Publish port for clients to connect to

		//Data Process
		//The raw data queue and its mutex
        QList<QByteArray> m_list_rawData;
		QMutex m_mutex_rawData;

        //The current Read Offset, from m_list_rawData's beginning
        //队列的首部元素已经处理的偏移。比如首部的 Block 有2341字节，
        //处理了1099字节，本指令已经结束，则此值为1099
		int m_currentReadOffset;

		//Current Message Offset, according to m_currentHeader
        //当前接收的信息的大小。比如 100MB 的信息，接受了 23MB，
        //这个值就是 23MB
		int m_currentMessageSize;

        //Current un-procssed message block. for large blocks,
        //this array will be re-setted as soon as some part of data has been dealed,
        //eg, send a 200MB block, the 200MB data will be splitted into pieces
        //当前的缓存。这个缓存会不断的递交处理，负责处理的程序可以根据情况适时清空它。
        //对短指令，不清空也是可以的。
		QByteArray m_currentBlock;

        //当前的信息头部，这个值记录了当前结构体的首部信息
		CROSS_SVR_MSG::tag_header m_currentHeader;

        HTTP_PACKET_FORMAT *m_requestPacket;
        HTTP_PACKET_FORMAT *m_responsePacket;

		QDateTime m_last_Report;
		quint32 m_nRemoteClientNums;

	signals:
        void evt_SendDataToClient(QObject *objClient, QByteArray array);
        void evt_Close_client(QObject *objClient);
        void evt_Connect_to(const QHostAddress &address, quint16 nPort, bool bSSLConn);
        void evt_Message(QObject *pSource, QString);

		//Notify Messages
        void evt_NewSvrConnected(QString svrName);
		//some data arrival
        void evt_RemoteData_recieved(QString /*svrName*/, QByteArray /*arrray*/);

    private:
        //One http_parser object is used per TCP connection
        http_parser m_parser;
        http_parser_settings m_settings;

        void parser_init();

        static int on_message_begin_callback(http_parser *parser);
        static int on_url_callback(http_parser *parser, const char *buf, size_t len);
        static int on_status_callback(http_parser *parser, const char *buf, size_t len);
        static int on_header_field_callback(http_parser *parser, const char *buf, size_t len);
        static int on_header_value_callback(http_parser *parser, const char *buf, size_t len);
        static int on_headers_complete_callback(http_parser *parser);
        static int on_body_callback(http_parser *parser, const char *buf, size_t len);
        static int on_message_complete_callback(http_parser *parser);
	};
}
#endif // ZP_CLUSTERNODE_H

#ifndef ST_CLIENTNODE_BASETRANS_H
#define ST_CLIENTNODE_BASETRANS_H

#include <QObject>
#include <QList>
#include <QDateTime>
#include "st_message.h"
#include "../pipeline/zp_pltaskbase.h"

namespace ExampleServer
{
	class st_client_table;
    //客户端基础传输类
	class st_clientNode_baseTrans : public ZPTaskEngine::zp_plTaskBase
	{
		Q_OBJECT
		friend class st_clien_table;

	public:
        explicit st_clientNode_baseTrans(st_client_table *pClientTable,
                                         QObject *pClientSock,
                                         QObject *parent = nullptr);

		//!deal at most m_nMessageBlockSize messages per deal_message();
		static const int m_nMessageBlockSize = 8;
		//!The main functional method, will run in thread pool
		int run() override;

		//push new binary data into queue
        int push_new_data(const QByteArray& array);

		quint32 uuid() const;
        QObject *socket() const;
		bool uuidValid() const;
		qint32 bytesLeft() const;

        void checkHeartBeating();
		bool bTermSet;

	protected:
        //!deal one message, affect m_currentReadOffset, m_currentMessageSize, m_currentHeader
		//!return bytes Used.
        int filter_message(const QByteArray &block, int offset);
		//!in Trans-Layer, it does nothing.
		virtual int deal_current_message_block();
		//judge whether id is valid.
        bool isValidUserId(quint32 id) const;

		//The current Read Offset, from m_list_rawData's beginning
		int m_currentReadOffset;
		//Current Message Offset, according to m_currentHeader
		int m_currentMessageSize;
		//Current un-procssed message block.for large blocks,
        //this array will be re-setted as soon as some part of data has been dealed,
        //eg, send a 200MB block, the 200MB data will be splitted into pieces
		QByteArray m_currentBlock;
		//current Header
        EXAMPLE_TRANS_MSG m_currentHeader;
		//The raw data queue and its mutex
        QList<QByteArray> m_list_rawData;
		QMutex m_mutex_rawData;

		//UUID of this equipment
		bool m_bUUIDRecieved;
        //Client ID
        quint64 m_uuid;
		//Client socket handle of this connection
        QObject *m_pClientSock;
        st_client_table *m_pClientTable;
		QDateTime m_last_Report;

	signals:
        void evt_SendDataToClient(QObject *objClient, QByteArray array);
        void evt_Close_client(QObject *objClient);
        void evt_Message(QObject *pSource, QString);
	};
}

#endif // ST_CLIENTNODE_BASETRANS_H

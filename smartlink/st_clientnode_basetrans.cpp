#include "st_clientnode_basetrans.h"
#include "st_client_table.h"
#include <assert.h>

namespace ExampleServer
{
    st_clientNode_baseTrans::st_clientNode_baseTrans(st_client_table* pClientTable,
                                                     QObject* pClientSock,
                                                     QObject* parent) :
		zp_plTaskBase(parent)
	{
		m_bUUIDRecieved = false;
		m_currentReadOffset = 0;
		m_currentMessageSize = 0;
		m_pClientSock = pClientSock;
        m_uuid = 0xffffffff;//无效的
		m_pClientTable = pClientTable;
		bTermSet = false;
		m_last_Report = QDateTime::currentDateTime();
        memset(&m_currentHeader, 0, sizeof(EXAMPLE_TRANS_MSG));
	}

	quint32 st_clientNode_baseTrans::uuid() const
	{
		return m_uuid;
	}

    QObject* st_clientNode_baseTrans::socket() const
    {
		return m_pClientSock;
	}

	bool st_clientNode_baseTrans::uuidValid() const
	{
		return m_bUUIDRecieved;
	}

	qint32 st_clientNode_baseTrans::bytesLeft() const
	{
        return m_currentHeader.data_length + sizeof(EXAMPLE_TRANS_MSG) - 1 - m_currentMessageSize;
	}

	//judge whether id is valid.
    bool st_clientNode_baseTrans::isValidUserId(quint32 id) const
    {
        return id >= static_cast<quint32>(0x00000002) && id <= static_cast<quint32>(0xFFFFFFFE);
	}

	//The main functional method, will run in thread pool
	int st_clientNode_baseTrans::run()
	{
        if (bTermSet)
		{
			return 0;
		}
        int nCurSize = -1;
		int nMessage = m_nMessageBlockSize;
        while (--nMessage >= 0 && nCurSize != 0)
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
                m_currentReadOffset = filter_message(block, m_currentReadOffset);
                int blockSize = block.size();
                if (m_currentReadOffset >= blockSize)
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
                //pop empty cabs
                if (!m_list_rawData.empty())
                {
                    m_list_rawData.pop_front();
                }
                m_mutex_rawData.unlock();
			}
			m_mutex_rawData.lock();
			nCurSize = m_list_rawData.size();
			m_mutex_rawData.unlock();
		}

		m_mutex_rawData.lock();
		nCurSize = m_list_rawData.size();
		m_mutex_rawData.unlock();

        return nCurSize == 0 ? 0 : -1;
	}

    //push new binary data array into queue
    int st_clientNode_baseTrans::push_new_data(const QByteArray& array)
	{
		int nRes;
		m_mutex_rawData.lock();
        m_list_rawData.push_back(array);
		nRes = m_list_rawData.size();
		m_mutex_rawData.unlock();
		m_last_Report = QDateTime::currentDateTime();
		return nRes;
	}

    //!deal one message, affect m_currentReadOffset, m_currentMessageSize, m_currentHeader
	//!return bytes Used.
    int st_clientNode_baseTrans::filter_message(const QByteArray& block, int offset)
	{
        const int blockLength = block.length();
        const char* dataptr = block.constData();
        while (offset < blockLength)
		{            
            //先确保信息的头标志被完整接收
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

            switch (m_currentHeader.Mark)
            {
                case 0xBEBE:
                {
                    quint32 heartbeatSize = sizeof(EXAMPLE_HEARTBEATING);
                    if ((quint32)m_currentMessageSize < heartbeatSize && offset < blockLength)
                    {
                        int nCpy = offset - blockLength;
                        if ((quint32)nCpy > heartbeatSize - m_currentMessageSize)
                        {
                            nCpy = heartbeatSize - m_currentMessageSize;
                        }
                        m_currentBlock.push_back(QByteArray(dataptr + offset, nCpy));
                        m_currentMessageSize += nCpy;
                        offset += nCpy;
                    }
                    //Header not completed.
                    if ((quint32)m_currentMessageSize < heartbeatSize)
                    {
                        break;
                    }
                    //Send back
                    emit evt_SendDataToClient(socket(), m_currentBlock);
                    //Try to Get UUID Immediately
                    if (!m_bUUIDRecieved)
                    {
                        EXAMPLE_HEARTBEATING *pHbMsg = (EXAMPLE_HEARTBEATING *)m_currentBlock.constData();
                        if (isValidUserId(pHbMsg->source_id))
                        {
                            m_bUUIDRecieved = true;
                            m_uuid = pHbMsg->source_id;
                            //regist client node to hash-table;
                            m_pClientTable->registClientUUID(this);
                        }
                    }

                    //This Message is Over. Start a new one.
                    m_currentMessageSize = 0;
                    m_currentBlock = QByteArray();
                    break;
                }
                case 0x55AA:
                {
                    //Only Mark and data_length
                    quint32 transMsgSize = sizeof(EXAMPLE_TRANS_MSG) - 1;
                    if (offset < blockLength)
                    {
                        int nCopy = blockLength - offset;
                        if ((quint32)nCopy > transMsgSize - m_currentMessageSize)
                        {
                            nCopy = transMsgSize - m_currentMessageSize;
                        }
                        m_currentBlock.push_back(QByteArray(dataptr + offset, nCopy));
                        m_currentMessageSize += nCopy;
                        offset += nCopy;
                    }
                    if ((quint32)m_currentMessageSize != transMsgSize)
                    {
                        break;
                    }
                    //Header just completed.
                    const char* headerptr = m_currentBlock.constData();
                    memcpy((void *)&m_currentHeader, headerptr, transMsgSize);

                    //continue reading if there is data left behind
                    if (blockLength > offset)
                    {
                        qint32 bitLeft = m_currentHeader.data_length;
                        if (bitLeft > 0)
                        {
                            int nCopy = blockLength - offset;
                            if (nCopy > bitLeft)
                            {
                                nCopy = bitLeft;
                            }
                            m_currentBlock.push_back(QByteArray(dataptr + offset, nCopy));
                            m_currentMessageSize += nCopy;
                            bitLeft -= nCopy;
                            offset += nCopy;
                        }
                        //deal block, may be send data as soon as possible;
                        deal_current_message_block();
                        if (bitLeft > 0)
                        {
                            break;
                        }
                        //This Message is Over. Start a new one.
                        m_currentMessageSize = 0;
                        m_currentBlock = QByteArray();
                    }
                    break;
                }
                case 0xA5A5:
                {
                    //Only Mark and data_length
                    quint32 transMsgSize = sizeof(EXAMPLE_TRANS_MSG) - 9;
                    if (offset < blockLength)
                    {
                        int nCopy = blockLength - offset;
                        if ((quint32)nCopy > transMsgSize - m_currentMessageSize)
                        {
                            nCopy = transMsgSize - m_currentMessageSize;
                        }
                        m_currentBlock.push_back(QByteArray(dataptr + offset, nCopy));
                        m_currentMessageSize += nCopy;
                        offset += nCopy;
                    }

                    if ((quint32)m_currentMessageSize != transMsgSize)
                    {
                        break;
                    }

                    //Header just completed.
                    const char *headerptr = m_currentBlock.constData();
                    memcpy((void *)&m_currentHeader, headerptr, transMsgSize);
                    m_currentHeader.source_id = 0xffffffff;

                    //continue reading if there is data left behind
                    if (offset < blockLength)
                    {
                        //add check and end_mark
                        qint32 bitLeft = m_currentHeader.data_length + 4;
                        if (bitLeft > 0)
                        {
                            int nCopy = blockLength - offset;
                            if (nCopy > bitLeft)
                            {
                                nCopy = bitLeft;
                            }
                            m_currentBlock.push_back(QByteArray(dataptr + offset, nCopy));
                            m_currentMessageSize += nCopy;
                            bitLeft -= nCopy;
                            offset += nCopy;
                        }
                        //deal block, may be send data as soon as possible;
                        deal_current_message_block();
                        if (bitLeft > 0)
                        {
                            break;
                        }
                        //This Message is Over. Start a new one.
                        m_currentMessageSize = 0;
                        m_currentBlock = QByteArray();
                    }
                    break;
                }
                default:
                {
                    const char* ptrCurData = m_currentBlock.constData();
                    emit evt_Message(this, tr("Client Send a unknown start Header %1 %2. Close client immediately.")
                                           .arg(static_cast<int>(ptrCurData[0])).arg(static_cast<int>(ptrCurData[1])));
                    m_currentMessageSize = 0;
                    m_currentBlock = QByteArray();
                    emit evt_Close_client(socket());
                    break;
                }
            }
        } // end while block offset < blockLength

		return offset;
	}

	//in Trans-Level, do nothing.
	int st_clientNode_baseTrans::deal_current_message_block()
	{
		//First, get uuid as soon as possible
        if (!m_bUUIDRecieved)
		{
            if (isValidUserId(m_currentHeader.source_id))
			{
				m_bUUIDRecieved = true;
                m_uuid = m_currentHeader.source_id;
                //regist client node to hash-table;
                m_pClientTable->registClientUUID(this);
			}
            else if (m_currentHeader.source_id == 0xffffffff)
			{
				//New clients
			}
			else //Invalid
			{
                emit evt_Message(this, tr("Client ID is invalid! Close client immediatly."));
				m_currentBlock = QByteArray();
                emit evt_Close_client(this->socket());
			}
		}
		else
		{
            if (!(isValidUserId(m_currentHeader.source_id) || m_currentHeader.source_id == 0xffffffff))
			{
                emit evt_Message(this, tr("Client ID is invalid! Close client immediatly."));
				m_currentBlock = QByteArray();
                emit evt_Close_client(this->socket());
			}
            if (isValidUserId(m_currentHeader.source_id) && m_uuid != m_currentHeader.source_id)
			{
                emit evt_Message(this, tr("Client ID Changed in Runtime! Close client immediatly."));
				m_currentBlock = QByteArray();
                emit evt_Close_client(this->socket());
			}
		}

		return 0;
	}

    void st_clientNode_baseTrans::checkHeartBeating()
	{
		QDateTime dtm = QDateTime::currentDateTime();
		qint64 usc = this->m_last_Report.secsTo(dtm);
		if (usc < m_pClientTable->heartBeatingThrd())
		{
			return;
		}
		emit evt_Message(this, tr("Client ") + QString("%1").arg((unsigned int)(quint64)this) + tr(" is dead, kick out."));
		emit evt_Close_client(this->socket());
	}
}

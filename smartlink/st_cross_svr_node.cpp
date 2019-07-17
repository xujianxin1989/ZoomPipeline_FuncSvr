#include "st_cross_svr_node.h"
#include "../cluster/zp_clusterterm.h"
#include "st_client_table.h"
#include "st_message.h"

namespace ExampleServer
{
    st_cross_svr_node::st_cross_svr_node(ZP_Cluster::zp_ClusterTerm* pTerm, QObject* pSocket, QObject* parent) :
		ZP_Cluster::zp_ClusterNode(pTerm, pSocket, parent),
		m_pClientTable(nullptr)
	{
		m_curStMegSize = 0;
		m_destin_uuid = 0xffffffff;
	}

    void st_cross_svr_node::setClientTable(st_client_table* clientTable)
    {
        m_pClientTable = clientTable;
    }

	int st_cross_svr_node::st_bytesLeft()
	{
        return m_st_Header.messageLen + sizeof(EXAMPLE_CROSSSVR_MSG::tag_msgHearder) - m_curStMegSize;
	}

	bool st_cross_svr_node::deal_user_data(QByteArray array)
	{
        const char* pData = array.constData();
        int nBlockSize = array.size();
		int nOffset = 0;
        quint32 tagMsgHearderSize = sizeof(EXAMPLE_CROSSSVR_MSG::tag_msgHearder);
        while (nOffset < nBlockSize)
        {
            if(m_curStMegSize < tagMsgHearderSize && nOffset < nBlockSize)
			{
				int nHeaderReadLen = nBlockSize - nOffset;
                if ((quint32)nHeaderReadLen > tagMsgHearderSize - m_curStMegSize)
                {
                    nHeaderReadLen = tagMsgHearderSize - m_curStMegSize;
                }
                m_curStBlock.push_back(QByteArray(pData + nOffset, nHeaderReadLen));
                m_curStMegSize += nHeaderReadLen;
				nOffset += nHeaderReadLen;
			}

            if (m_curStMegSize < tagMsgHearderSize)
            {
                return true;
            }
            if (m_curStMegSize == tagMsgHearderSize)
			{
                memcpy(&m_st_Header, m_curStBlock.constData(), tagMsgHearderSize);
				if (m_st_Header.Mark != 0x4567)
				{
                    m_curStMegSize = 0;
                    m_curStBlock.clear();
					return true;
				}
            }

            if (nOffset < nBlockSize &&
                m_curStMegSize < m_st_Header.messageLen + tagMsgHearderSize)
			{
				int nHeaderReadLen = nBlockSize - nOffset;
                if ((quint32)nHeaderReadLen > m_st_Header.messageLen + tagMsgHearderSize - m_curStMegSize)
                {
                    nHeaderReadLen = m_st_Header.messageLen + tagMsgHearderSize - m_curStMegSize;
                }
                m_curStBlock.push_back(QByteArray(pData + nOffset, nHeaderReadLen));
                m_curStMegSize += nHeaderReadLen;
                nOffset += nHeaderReadLen;
			}
			bool needDel = deal_msg();
            if (0 == st_bytesLeft() || needDel)
			{
                m_curStMegSize = 0;
                m_curStBlock.clear();
				m_destin_uuid = 0xffffffff;
			}
		}

        return ZP_Cluster::zp_ClusterNode::deal_user_data(array);
	}

    bool st_cross_svr_node::deal_msg()
	{
        bool delCurBlock = false;
        switch (m_st_Header.messageType)
		{
            case 0x01: //client node log in
            {
                if (st_bytesLeft() > 0)
                {
                    return delCurBlock;
                }
                EXAMPLE_CROSSSVR_MSG* pMsg = (EXAMPLE_CROSSSVR_MSG*)m_curStBlock.constData();
                int nUUIDs = pMsg->header.messageLen / sizeof(quint32);
                m_pClientTable->cross_svr_add_uuids(this->termName(), pMsg->payload.uuids, nUUIDs);
                break;
            }
            case 0x02: //client node exit
            {
                if (st_bytesLeft() > 0)
                {
                    return delCurBlock;
                }
                EXAMPLE_CROSSSVR_MSG* pMsg = (EXAMPLE_CROSSSVR_MSG*)m_curStBlock.constData();
                int nUUIDs = pMsg->header.messageLen / sizeof(quint32);
                m_pClientTable->cross_svr_del_uuids(this->termName(), pMsg->payload.uuids, nUUIDs);
                break;
            }
            case 0x03: //data transfer
            {
                quint32 tagMsgHearderSize = sizeof(EXAMPLE_CROSSSVR_MSG::tag_msgHearder);
                if (m_destin_uuid == 0xffffffff)
                {
                    if (m_curStMegSize >= tagMsgHearderSize + sizeof(EXAMPLE_TRANS_MSG) - 1)
                    {
                        EXAMPLE_CROSSSVR_MSG* pMsg = (EXAMPLE_CROSSSVR_MSG*)m_curStBlock.constData();
                        EXAMPLE_TRANS_MSG* pSmMsg = (EXAMPLE_TRANS_MSG*)pMsg->payload.data;
                        m_destin_uuid = pSmMsg->destin_id;
                    }
                }
                if (m_destin_uuid == 0xffffffff)
                {
                    return false;
                }

                //Send to Client
                //emit evt_SendToNode(m_destin_uuid, m_curStBlock);
                //Transfer
                //bool res = false;
                if (m_curStMegSize == (quint32)m_curStBlock.size())
                {
                    EXAMPLE_CROSSSVR_MSG* pMsg = (EXAMPLE_CROSSSVR_MSG*)m_curStBlock.constData();
                    EXAMPLE_TRANS_MSG* pSmMsg = (EXAMPLE_TRANS_MSG*)pMsg->payload.data;
                    QByteArray blocks((const char*)pSmMsg, m_curStMegSize - tagMsgHearderSize);
                    /*res = */m_pClientTable->sendToNode(m_destin_uuid, blocks);
                }
                else
                {
                    /*res = */m_pClientTable->sendToNode(m_destin_uuid, m_curStBlock);
                }
                delCurBlock = true;
                break;
            }
            default:
                break;
		}

        return delCurBlock;
	}
}

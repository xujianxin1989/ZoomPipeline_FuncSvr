#include "st_clientnode_applayer.h"
#include "st_client_table.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

namespace ExampleServer
{
    st_clientNodeAppLayer::st_clientNodeAppLayer(st_client_table* pClientTable, QObject* pClientSock, QObject* parent) :
		st_clientNode_baseTrans(pClientTable, pClientSock, parent)
	{
        m_bLoggedIn = false;
        memset(&m_current_app_header, 0, sizeof(EXAMPLE_MSG_APP));
	}

	bool st_clientNodeAppLayer::loadRelations()
	{
        QSqlDatabase db = m_pClientTable->dbRes()->databse(m_pClientTable->database_UserAcct());
        if (db.isValid() && db.isOpen())
		{
			QString sql = "select friend_id from relations where user_id = ?;";
			QSqlQuery query(db);
			query.prepare(sql);
			query.addBindValue((quint32)m_uuid);
            if (!query.exec())
			{
                emit evt_Message(this, tr("try to get relations Failed! ") + query.lastError().text());
				db.close();
				return false;
			}

			m_matched_nodes.clear();
			while (query.next())
			{
				quint32 val = query.value(0).toUInt();
				m_matched_nodes.insert(val);
			}

			return true;
		}
		//Server db is currently not accessable, wait.
		emit evt_Message(this, "Server Not Accessable Now.");

		return false;
	}

	bool st_clientNodeAppLayer::saveRelations()
	{
        QSqlDatabase db = m_pClientTable->dbRes()->databse(m_pClientTable->database_UserAcct());
        if (db.isValid() && db.isOpen())
		{
			QSqlQuery query(db);
			QString sql = "delete from relations where user_id = ?;";

			query.prepare(sql);
			query.addBindValue((quint32)m_uuid);
            if (!query.exec())
			{
                emit evt_Message(this, tr("try to del old relations Failed! ") + query.lastError().text());
				db.close();
				return false;
			}

			sql = "insert into relations (user_id,friend_id) values (?,?);";
			//the forigen key can automatic avoid non-existing values.
            foreach(quint32 nodeid, m_matched_nodes)
			{
				QSqlQuery query(db);
                if (!isValidUserId(nodeid))
                {
                    continue;
                }
				query.prepare(sql);
				query.addBindValue((quint32)m_uuid);
				query.addBindValue(nodeid);

                if (!query.exec())
				{
                    emit evt_Message(this, tr("try to insert new relations Failed! ") + query.lastError().text());
					db.close();
					return false;
				}
			}

			return true;
		}
		//Server db is currently not accessable, wait.
		emit evt_Message(this, "Server Not Accessable Now.");

		return false;
	}

	//!deal current message
	int st_clientNodeAppLayer::deal_current_message_block()
    {
        if (0x00000000 == m_currentHeader.destin_id)
        {
            QStringList list = m_pClientTable->clusterTerm()->svrNames();
            foreach (QString svrName, list)
            {
                m_pClientTable->cross_web_send_data(svrName, m_currentBlock);
            }
        }
        else if (0x00000001 == m_currentHeader.destin_id)
		{
            if (!m_bLoggedIn || !m_bUUIDRecieved)
			{
				//To-Server Messages does not wait for message-block completes
                if (!deal_ToServer_Handshakes())
				{
					m_currentBlock = QByteArray();
                    emit evt_Message(this, tr("To-server Message Failed."));
                    emit evt_Close_client(this->socket());
				}
			}
			else
			{
                if (isValidUserId(m_currentHeader.source_id))
				{
                    //Deal Client->Server Msgs
                    if (!deal_Node2Svr_Msgs())
					{
						m_currentBlock = QByteArray();
                        emit evt_Message(this, tr("Client To Server Message Failed."));
                        emit evt_Close_client(this->socket());
					}
				}
                else if (0xFFFFFFFF == m_currentHeader.source_id)
				{
					m_currentBlock = QByteArray();
                    emit evt_Message(this, tr("warning, UUID 0xFFFFFFFF.ignore"));
				}
				else
				{
					m_currentBlock = QByteArray();
                    emit evt_Message(this, tr("Bad UUID %1. Client Kicked out").arg(m_currentHeader.source_id));
                    emit evt_Close_client(this->socket());
				}
			}
		}
		else
		{
            //find destin client using hash.
            st_clientNode_baseTrans* destin_node = m_pClientTable->clientNodeFromUUID(m_currentHeader.destin_id);
            if (destin_node)
            {
                emit evt_SendDataToClient(destin_node->socket(), m_currentBlock);
                m_currentBlock = QByteArray();
            }
            else
			{
				//need server-to-server channels to re-post this message.
				QString svr = m_pClientTable->cross_svr_find_uuid(m_currentHeader.destin_id);
                if (svr.isEmpty())
                {
                    emit evt_Message(this, tr("Destin ID ") + QString("%1").arg(m_currentHeader.destin_id) + tr(" is not currently logged in."));
                }
				else
                {
                    m_pClientTable->cross_svr_send_data(svr, m_currentBlock);
                }
			}
		}

        if (0 == bytesLeft())
        {
            m_current_app_header.header.MsgType = 0x00;
        }
        return st_clientNode_baseTrans::deal_current_message_block();
	}

	//deal to-server handshakes
    bool st_clientNodeAppLayer::deal_ToServer_Handshakes()
	{
		bool res;
        quint32 transMsgSize = sizeof(EXAMPLE_TRANS_MSG) - 1;
        quint32 tagAppLayerHeaderSize = sizeof(EXAMPLE_MSG_APP::tag_app_layer_header);
        if (m_currentHeader.data_length < tagAppLayerHeaderSize)
        {
            return false;
        }
        if ((quint32)m_currentMessageSize < transMsgSize + tagAppLayerHeaderSize)
		{
            //header is not complete, return
			return true;
		}
		//Catch the header
        if (m_current_app_header.header.MsgType == 0x00)
        {
            memcpy((void *)&m_current_app_header,
                   m_currentBlock.constData() + transMsgSize,
                   tagAppLayerHeaderSize);
        }
		switch (m_current_app_header.header.MsgType)
		{
            case 0x0001:
                if (bytesLeft() > 0)
                {
                    //message is not complete, return
                    return true;
                }
                if ((quint32)m_currentMessageSize > transMsgSize +
                        tagAppLayerHeaderSize + sizeof(stMsg_ClientLoginReq) + 66)
                {
                    emit evt_Message(this, tr("Broken Message stMsg_ClientLoginReq, size not correct."));
                    res = false;
                }
                else
                {
                    res = this->loginClient();
                }
                break;
            default:
                emit evt_Message(this, tr("Message type not supported."));
                res = false;
                break;
		}

		m_currentBlock.clear();
		return res;
	}

	//Deal Box2Svr Msgs
    bool st_clientNodeAppLayer::deal_Node2Svr_Msgs()
	{
		bool res;
        quint32 transMsgSize = sizeof(EXAMPLE_TRANS_MSG) - 1;
        quint32 tagAppLayerHeaderSize = sizeof(EXAMPLE_MSG_APP::tag_app_layer_header);
        if (m_currentHeader.data_length < tagAppLayerHeaderSize)
        {
            return false;
        }
        if ((quint32)m_currentMessageSize < transMsgSize + tagAppLayerHeaderSize)
		{
			//header is not complete, return
			return true;
		}
		//Catch the header
        if (m_current_app_header.header.MsgType == 0x00)
        {
            memcpy((void *)&this->m_current_app_header,
                   (unsigned char *)this->m_currentBlock.constData() + transMsgSize,
                   tagAppLayerHeaderSize);
        }
		//do
		switch (m_current_app_header.header.MsgType)
		{
            case 0x1003:
                if (bytesLeft() > 0)
                {
                    return true;
                }
                if ((quint32)m_currentMessageSize < transMsgSize + tagAppLayerHeaderSize +
                        sizeof(stMsg_UploadUserListReq) - sizeof(quint32))
                {
                    emit evt_Message(this, tr("Broken Message stMsg_UploadUserListReq, size not correct."));
                    res = false;
                }
                else
                {
                    res = this->box2Svr_UploadUserTable();
                }
                break;
            case 0x1004:
                if (bytesLeft() > 0)
                {
                    //message is not complete, return
                    return true;
                }
                if ((quint32)m_currentMessageSize != transMsgSize + tagAppLayerHeaderSize
                        /*+ sizeof(stMsg_DownloadUserListReq)*/)
                {
                    emit evt_Message(this, tr("Broken Message stMsg_DownloadUserListReq, size not correct."));
                    res = false;
                }
                else
                {
                    res = this->box2Svr_DownloadUserTable();
                }
                break;
            case 0x1002:
                if (bytesLeft() > 0)
                {
                    // message is not complete, return
                    return true;
                }
                if ((quint32)m_currentMessageSize != transMsgSize + tagAppLayerHeaderSize +
                        sizeof(stMsg_ClientLogoutReq))
                {
                    emit evt_Message(this, tr("Broken Message stMsg_ClientLogoutReq, size not correct."));
                    res = false;
                }
                else
                {
                    res = this->logoutClient();
                }
                break;

            default:
                emit evt_Message(this, tr("Message type not supported."));
                res = false;
                break;
		}

		m_currentBlock.clear();
		return res;
	}
}

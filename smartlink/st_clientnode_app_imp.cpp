#include "st_clientnode_applayer.h"
#include "st_client_table.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSettings>
#include <QSqlError>
#include <QSet>

namespace ExampleServer
{
    bool st_clientNodeAppLayer::loginClient()
	{
        quint32 transMsgSize = sizeof(EXAMPLE_TRANS_MSG) - 1;
        quint32 tagAppLayerHeaderSize = sizeof(EXAMPLE_MSG_APP::tag_app_layer_header);
        const EXAMPLE_MSG_APP* pAppLayer = (const EXAMPLE_MSG_APP*)(m_currentBlock.constData() + transMsgSize);
        int nAppLen = m_currentBlock.length() - transMsgSize - tagAppLayerHeaderSize - sizeof(quint32);
        QString strPasswd;
		quint32 UserID = pAppLayer->MsgUnion.msg_ClientLoginReq.user_id;

		int nSwim = 0;
        while (nSwim < 65 && nSwim < nAppLen && pAppLayer->MsgUnion.msg_ClientLoginReq.Passwd[nSwim] != 0)
        {
            strPasswd += pAppLayer->MsgUnion.msg_ClientLoginReq.Passwd[nSwim++];
        }

		//form Msgs
        quint16 nMsgLen = tagAppLayerHeaderSize + sizeof(stMsg_ClientLoginRsp);
        QByteArray array(transMsgSize + nMsgLen, 0);
        char* ptr = array.data();
        EXAMPLE_TRANS_MSG* pMsg = (EXAMPLE_TRANS_MSG*)ptr;
        EXAMPLE_MSG_APP* pApp = (EXAMPLE_MSG_APP*)(((unsigned char*)(ptr)) + transMsgSize);
		pMsg->Mark = 0x55AA;
        pMsg->source_id = (quint32)((quint64)(m_currentHeader.destin_id) & 0xffffffff);
        pMsg->destin_id = (quint32)((quint64)(m_currentHeader.source_id) & 0xffffffff);
		pMsg->data_length = nMsgLen;
		pApp->header.MsgType = 0x7FFE;
		stMsg_ClientLoginRsp & reply = pApp->MsgUnion.msg_ClientLoginRsp;

		//Check the database, find current equipment info
        QSqlDatabase db = m_pClientTable->dbRes()->databse(m_pClientTable->database_UserAcct());
		reply.DoneCode = 3;

        if (db.isValid() && db.isOpen())
		{
            QSqlQuery query(db);
            QString sql = "select user_id, password from users where user_id = ? and password = ?;";
			query.prepare(sql);
			query.addBindValue(UserID);
			query.addBindValue(strPasswd);

            if (query.exec())
			{
				if (query.next())
				{
					bool bOk = false;
                    quint32 nCurrid = query.value(1).toUInt(&bOk);
                    if (bOk)
					{
                        if (this->isValidUserId(nCurrid))
						{
							reply.DoneCode = 0;
                            reply.UserID = nCurrid;
							m_bLoggedIn = true;
							m_bUUIDRecieved = true;
                            m_uuid = nCurrid;
                            m_pClientTable->registClientUUID(this);
                            if (!loadRelations())
							{
								reply.DoneCode = 3;
							}

							//Cluster-Balance.
                            if (m_pClientTable->needRedirect(reply.Address_Redirect, &reply.port_Redirect))
							{
								reply.DoneCode = 1;
							}
						}
                        //else
                        //strcpy(reply.TextInfo, "UserID Is Invalid.Accunt locked by svr");
					}
					//else
                    //strcpy(reply.TextInfo, "UserID Is Invalid.Accunt locked by svr");
				}
				else
				{
                    //No such device
					//strcpy(reply.TextInfo,"No such user or password.");
				}
			}
			else
			{
                emit evt_Message(this, tr("Database Access Error :") + query.lastError().text());
				db.close();
			}
		}
		else
		{
			//Server db is currently not accessable, wait.
            //strcpy(reply.TextInfo, "Server Not Accessable Now.");
		}

		//Send back
        emit evt_SendDataToClient(this->socket(), array);
        return reply.DoneCode == 3 ? false : true;
	}

    bool st_clientNodeAppLayer::box2Svr_UploadUserTable()
	{
        quint32 transMsgSize = sizeof(EXAMPLE_TRANS_MSG) - 1;
        quint32 tagAppLayerHeaderSize = sizeof(EXAMPLE_MSG_APP::tag_app_layer_header);
        const EXAMPLE_MSG_APP* pAppLayer = (const EXAMPLE_MSG_APP*)(m_currentBlock.constData() + transMsgSize);

        if ((quint32)m_currentMessageSize != transMsgSize + tagAppLayerHeaderSize +
                sizeof(stMsg_UploadUserListReq) - sizeof(quint32) +
                sizeof(quint32) * (pAppLayer->MsgUnion.msg_UploadUserListReq.UserNum))
		{
            emit evt_Message(this, tr("Broken Message stMsg_UploadUserListReq, size not correct."));
			return false;
		}

		//form Msgs
        quint16 nMsgLen = tagAppLayerHeaderSize + sizeof(stMsg_UploadUserListRsp);
        QByteArray array(transMsgSize + nMsgLen, 0);
        char* ptr = array.data();
        EXAMPLE_TRANS_MSG* pMsg = (EXAMPLE_TRANS_MSG*)ptr;
        EXAMPLE_MSG_APP* pApp = (EXAMPLE_MSG_APP*)(((unsigned char*)(ptr)) + transMsgSize);
		pMsg->Mark = 0x55AA;
        pMsg->source_id = (quint32)((quint64)(m_currentHeader.destin_id) & 0xffffffff);
        pMsg->destin_id = (quint32)((quint64)(m_currentHeader.source_id) & 0xffffffff);
		pMsg->data_length = nMsgLen;
		pApp->header.MsgType = 0x7FFC;
		stMsg_UploadUserListRsp & reply = pApp->MsgUnion.msg_UploadUserListRsp;

		reply.DoneCode = 1;
		//strcpy(reply.TextInfo,"Unknown error");
        if (loadRelations())
		{
            for (quint32 i = 0; i < pAppLayer->MsgUnion.msg_UploadUserListReq.UserNum && i < 4096; i++)
			{
                if (isValidUserId(pAppLayer->MsgUnion.msg_UploadUserListReq.pUserIDList[i]) == false)
                {
                    continue;
                }
				m_matched_nodes.insert(pAppLayer->MsgUnion.msg_UploadUserListReq.pUserIDList[i]);
			}
            if (saveRelations())
			{
				reply.DoneCode = 0;
                //strcpy(reply.TextInfo, "Succeeded.");
			}
		}
		else
		{
			//Server db is currently not accessable, wait.
            //strcpy(reply.TextInfo, "Failed to load current relations from db.");
		}

		//Send back
        emit evt_SendDataToClient(this->socket(), array);
        return reply.DoneCode == 0 ? true : false;
	}

    bool st_clientNodeAppLayer::box2Svr_DownloadUserTable()
	{
		bool res = true;
		//form Msgs
        quint16 nMsgLen = sizeof(EXAMPLE_MSG_APP::tag_app_layer_header) + sizeof(stMsg_DownloadUserListRsp) - sizeof(quint32);
        int nSize = 0;
        if (loadRelations())
		{
            nSize = m_matched_nodes.size();
            nMsgLen += nSize * sizeof(quint32);
		}
		else
        {
            res = false;
        }

		//form Msgs
        quint32 transMsgSize = sizeof(EXAMPLE_TRANS_MSG) - 1;
        QByteArray array(transMsgSize + nMsgLen, 0);
        char* ptr = array.data();
        EXAMPLE_TRANS_MSG* pMsg = (EXAMPLE_TRANS_MSG*)ptr;
        EXAMPLE_MSG_APP* pApp = (EXAMPLE_MSG_APP*)(((unsigned char*)(ptr)) + transMsgSize);
		pMsg->Mark = 0x55AA;
        pMsg->source_id = (quint32)((quint64)(m_currentHeader.destin_id) & 0xffffffff);
        pMsg->destin_id = (quint32)((quint64)(m_currentHeader.source_id) & 0xffffffff);
		pMsg->data_length = nMsgLen;
		pApp->header.MsgType = 0x7FFB;
		stMsg_DownloadUserListRsp & reply = pApp->MsgUnion.msg_DownloadUserListRsp;

        reply.DoneCode = res == true ? 0 : 1;

		int ii = 0;
		foreach (quint32 it, m_matched_nodes)
		{
            if (ii < nSize && ii < 32768)
			{
				reply.pUserIDList[ii] = it;
                reply.UserNum = (quint16)ii + 1;
			}
			++ii;
		}

		//Send back
        emit evt_SendDataToClient(this->socket(), array);
        return reply.DoneCode == 0 ? true : false;
	}

    bool st_clientNodeAppLayer::logoutClient()
	{
		bool res = true;
		//form Msgs
        quint16 nMsgLen = sizeof(EXAMPLE_MSG_APP::tag_app_layer_header) + sizeof(stMsg_ClientLogoutRsp);
		//int nSz = 0;
		//form Msgs
        quint32 transMsgSize = sizeof(EXAMPLE_TRANS_MSG) - 1;
        QByteArray array(transMsgSize + nMsgLen, 0);
        char* ptr = array.data();
        EXAMPLE_TRANS_MSG* pMsg = (EXAMPLE_TRANS_MSG*)ptr;
        EXAMPLE_MSG_APP* pApp = (EXAMPLE_MSG_APP*)((unsigned char*)(ptr) + transMsgSize);
		pMsg->Mark = 0x55AA;
        pMsg->source_id = (quint32)((quint64)(m_currentHeader.destin_id) & 0xffffffff);
        pMsg->destin_id = (quint32)((quint64)(m_currentHeader.source_id) & 0xffffffff);
		pMsg->data_length = nMsgLen;
		pApp->header.MsgType = 0x7FFD;
		stMsg_ClientLogoutRsp & reply = pApp->MsgUnion.msg_ClientLogoutRsp;
        reply.DoneCode = res == true ? 0 : 1;

		//Send back
        emit evt_SendDataToClient(this->socket(), array);
        return reply.DoneCode == 0 ? true : false;
	}
}

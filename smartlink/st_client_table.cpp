#include "st_client_table.h"
#include "st_clientnode_applayer.h"
#include <assert.h>
#include "st_cross_svr_node.h"
#include "st_cross_svr_msg.h"
#include <functional>
#include <QList>
#include <QStringList>

namespace ExampleServer
{
	using namespace std::placeholders;
    st_client_table::st_client_table(ZPNetwork::zp_net_Engine* netEngine,
                                     ZPTaskEngine::zp_pipeline* taskEngine,
                                     ZPDatabase::DatabaseResource* pDb,
                                     ZP_Cluster::zp_ClusterTerm* pCluster,
                                     QObject* parent) :
		QObject(parent),
		m_pThreadEngine(netEngine),
		m_pTaskEngine(taskEngine),
		m_pDatabaseRes(pDb),
		m_pCluster(pCluster),
		m_nBalanceMax(1024)
	{
		m_nHeartBeatingDeadThrd = 180;

        //连接新用户来到信号，将分配新的用户节点、登记哈希表(对一般的Socket)、广播集群信息
        connect(m_pThreadEngine, &ZPNetwork::zp_net_Engine::evt_NewClientConnected, this, &st_client_table::on_evt_NewClientConnected, Qt::QueuedConnection);
        //连接新用户已保护信号，将分配新的用户节点、登记哈希表(对SSL Socket)
        connect(m_pThreadEngine, &ZPNetwork::zp_net_Engine::evt_ClientEncrypted, this, &st_client_table::on_evt_ClientEncrypted, Qt::QueuedConnection);
        //连接新用户断开信号，将清除哈希表、广播集群信息
        connect(m_pThreadEngine, &ZPNetwork::zp_net_Engine::evt_ClientDisconnected, this, &st_client_table::on_evt_ClientDisconnected, Qt::QueuedConnection);
        //连接用户数据到来信号，将直接向各个客户端的处理队列中push，以待线程池解译客户端消息
        connect(m_pThreadEngine, &ZPNetwork::zp_net_Engine::evt_Data_recieved, this, &st_client_table::on_evt_Data_recieved, Qt::QueuedConnection);
        //连接用户数据成功发送信号，目前只用于统计流量
        connect(m_pThreadEngine, &ZPNetwork::zp_net_Engine::evt_Data_transferred, this, &st_client_table::on_evt_Data_transferred, Qt::QueuedConnection);

        //连接新集群节点接入信号，将向该节点发送HELLO包，并交换持有的客户端情况
        connect(m_pCluster, &ZP_Cluster::zp_ClusterTerm::evt_NewSvrConnected, this, &st_client_table::on_evt_NewSvrConnected, Qt::QueuedConnection);
        //连接集群节点断开信号，将清除属于该节点的客户端全局哈希
        connect(m_pCluster, &ZP_Cluster::zp_ClusterTerm::evt_NewSvrDisconnected, this, &st_client_table::on_evt_NewSvrDisconnected, Qt::QueuedConnection);
        //集群节点数据接收，将直接向各个集群节点的处理队列中push，以待线程池解译集群消息
        connect(m_pCluster, &ZP_Cluster::zp_ClusterTerm::evt_RemoteData_recieved, this, &st_client_table::on_evt_RemoteData_recieved, Qt::QueuedConnection);
        //集群节点数据发送，只用于流量统计
        connect(m_pCluster, &ZP_Cluster::zp_ClusterTerm::evt_RemoteData_transferred, this, &st_client_table::on_evt_RemoteData_transferred, Qt::QueuedConnection);
        m_pCluster->setNodeFactory(std::bind(&st_client_table::cross_svr_node_factory,
                                             this,
                                             _1,
                                             _2,
                                             _3)
                                  );
	}

    void st_client_table::setBalanceMax(int nMax)
	{
        m_nBalanceMax = nMax;
	}

	int st_client_table::balanceMax()
	{
		return m_nBalanceMax;
	}

	int st_client_table::heartBeatingThrd()
	{
		return m_nHeartBeatingDeadThrd;
	}

	void st_client_table::setHeartBeatingThrd(int h)
	{
		m_nHeartBeatingDeadThrd = h;
	}

	//Database and disk resources
    QString st_client_table::database_UserAcct()
	{
		return m_strDBName_useraccount;
	}

    void st_client_table::setDatabase_UserAcct(QString s)
	{
		m_strDBName_useraccount = s;
	}

    QString st_client_table::database_Event()
	{
		return m_strDBName_event;
	}

    void st_client_table::setDatabase_Event(QString s)
	{
		m_strDBName_event = s;
	}

	QString st_client_table::largeFileFolder()
	{
		return m_largeFileFolder;
	}

    void st_client_table::setLargeFileFolder(QString s)
	{
		m_largeFileFolder = s;
	}

    ZPDatabase::DatabaseResource* st_client_table::dbRes()
	{
        return m_pDatabaseRes;
    }

    ZP_Cluster::zp_ClusterTerm *st_client_table::clusterTerm()
    {
        return m_pCluster;
    }

	st_client_table::~st_client_table()
	{

	}

    void st_client_table::kickDeadClients()
	{
		m_hash_mutex.lock();
        for (std::unordered_map<QObject*, st_clientNode_baseTrans*>::iterator p = m_hash_sock2node.begin();
             p != m_hash_sock2node.end(); p++)
		{
            (*p).second->checkHeartBeating();
		}
		m_hash_mutex.unlock();
	}

    bool st_client_table::registClientUUID(st_clientNode_baseTrans* clientNode)
	{
        if (!clientNode->uuidValid())
        {
            return false;
        }

		m_hash_mutex.lock();
        m_hash_uuid2node[clientNode->uuid()] = clientNode;
		m_hash_mutex.unlock();
        broadcast_client_uuid(clientNode->uuid(), true);
		return true;
	}

    st_clientNode_baseTrans* st_client_table::clientNodeFromUUID(quint32 uuid)
	{
		m_hash_mutex.lock();
        if (m_hash_uuid2node.find(uuid) != m_hash_uuid2node.end())
		{
			m_hash_mutex.unlock();
			return m_hash_uuid2node[uuid];
		}
		m_hash_mutex.unlock();
		return NULL;
	}

    st_clientNode_baseTrans* st_client_table::clientNodeFromSocket(QObject* socket)
	{
		m_hash_mutex.lock();
        if (m_hash_sock2node.find(socket) != m_hash_sock2node.end())
		{
			m_hash_mutex.unlock();
            return m_hash_sock2node[socket];
		}
		m_hash_mutex.unlock();
		return NULL;
	}

	//this event indicates new client encrypted.
    void st_client_table::on_evt_ClientEncrypted(QObject* clientHandle)
	{
        st_clientNode_baseTrans *pClientNode;
		m_hash_mutex.lock();
        bool bHashContains = m_hash_sock2node.find(clientHandle) != m_hash_sock2node.end() ? true : false;
        if (!bHashContains)
		{
            st_clientNode_baseTrans* pNode = new st_clientNodeAppLayer(this, clientHandle, 0);
			//using queued connection of send and revieve;
            //把客户端发送数据的信号与网络模块连接起来，如客户端A发出该信号，含有客户端B的标示，将由网络模块响应，并最终发给客户端B
            connect(pNode, &st_clientNode_baseTrans::evt_SendDataToClient, m_pThreadEngine, &ZPNetwork::zp_net_Engine::sendDataToClient, Qt::QueuedConnection);
            //第二组连接了客户端踢出信号，比如客户端A想踢出客户端B，则把B的标示泵出，由网络模块响应并踢出B
            connect(pNode, &st_clientNode_baseTrans::evt_Close_client, m_pThreadEngine, &ZPNetwork::zp_net_Engine::kickClients, Qt::QueuedConnection);
            //第三组连接了消息显示信号
            connect(pNode, &st_clientNode_baseTrans::evt_Message, this, &st_client_table::evt_Message, Qt::QueuedConnection);
            m_hash_sock2node[clientHandle] = pNode;
            bHashContains = true;
            pClientNode = pNode;
		}
		else
		{
            pClientNode = m_hash_sock2node[clientHandle];
		}
		m_hash_mutex.unlock();
        assert(bHashContains != false && pClientNode != 0);
	}

	//this event indicates new client connected.
    //在各个新客户端接入的时候，创建属于该客户端的节点对象(st_clientNode_baseTrans)，并建立信号连接
    void st_client_table::on_evt_NewClientConnected(QObject* clientHandle)
    {
        st_clientNode_baseTrans *pClientNode;
		m_hash_mutex.lock();
        bool bHashContains = m_hash_sock2node.find(clientHandle) != m_hash_sock2node.end() ? true : false;
        if (!bHashContains)
		{
            st_clientNode_baseTrans* pNode = new st_clientNodeAppLayer(this, clientHandle, 0);
            //using queued connection of send and receive;
            //把客户端发送数据的信号与网络模块连接起来，如客户端A发出该信号，含有客户端B的标示，将由网络模块响应，并最终发给客户端B
            connect(pNode, &st_clientNode_baseTrans::evt_SendDataToClient, m_pThreadEngine, &ZPNetwork::zp_net_Engine::sendDataToClient, Qt::QueuedConnection);
            //第二组连接了客户端踢出信号，比如客户端A想踢出客户端B，则把B的标示泵出，由网络模块响应并踢出B
            connect(pNode, &st_clientNode_baseTrans::evt_Close_client, m_pThreadEngine, &ZPNetwork::zp_net_Engine::kickClients, Qt::QueuedConnection);
            //第三组连接了消息显示信号
            connect(pNode, &st_clientNode_baseTrans::evt_Message, this, &st_client_table::evt_Message, Qt::QueuedConnection);

            m_hash_sock2node[clientHandle] = pNode;
            bHashContains = true;
            pClientNode = pNode;
		}
		else
		{
            pClientNode = m_hash_sock2node[clientHandle];
		}
		m_hash_mutex.unlock();
        assert(bHashContains != false && pClientNode != 0);
	}

	//this event indicates a client disconnected.
    void st_client_table::on_evt_ClientDisconnected(QObject* clientHandle)
	{
        st_clientNode_baseTrans* pClientNode = 0;
		m_hash_mutex.lock();
        bool bHashContains = (m_hash_sock2node.find(clientHandle) != m_hash_sock2node.end()) ? true : false;
        if (bHashContains)
        {
            pClientNode = m_hash_sock2node[clientHandle];
        }
		if (pClientNode)
		{
			m_hash_sock2node.erase(clientHandle);
			if (pClientNode->uuidValid())
			{
				//This is important. some time m_hash_sock2node and m_hash_uuid2node, same uuid has different socket.
                if (m_hash_uuid2node.find(pClientNode->uuid()) != m_hash_uuid2node.end())
                {
                    if (m_hash_uuid2node[pClientNode->uuid()] == pClientNode)
					{
                        m_hash_uuid2node.erase(pClientNode->uuid());
                        broadcast_client_uuid(pClientNode->uuid(), false);
					}
                }
			}

			pClientNode->bTermSet = true;
            disconnect(pClientNode, &st_clientNode_baseTrans::evt_SendDataToClient, m_pThreadEngine, &ZPNetwork::zp_net_Engine::sendDataToClient);
            disconnect(pClientNode, &st_clientNode_baseTrans::evt_Close_client, m_pThreadEngine, &ZPNetwork::zp_net_Engine::kickClients);
            disconnect(pClientNode, &st_clientNode_baseTrans::evt_Message, this, &st_client_table::evt_Message);

			m_nodeToBeDel.push_back(pClientNode);
		}
		m_hash_mutex.unlock();

		//Try to delete objects
        QList<st_clientNode_baseTrans*> toBedel;
        foreach(st_clientNode_baseTrans* pdelObj, m_nodeToBeDel)
		{
            if (0 == pdelObj->ref())
            {
                toBedel.push_back(pdelObj);
            }
		}
        foreach(st_clientNode_baseTrans* pdelObj, toBedel)
		{
            m_nodeToBeDel.removeAll(pdelObj);
            pdelObj->deleteLater();
		}
	}

	//some data arrival
    void st_client_table::on_evt_Data_recieved(QObject* clientHandle, QByteArray datablock)
	{
		//Push Clients to nodes if it is not exist
        st_clientNode_baseTrans *pClientNode;
		m_hash_mutex.lock();
        bool bHashContains = m_hash_sock2node.find(clientHandle) != m_hash_sock2node.end() ? true : false;
        if (!bHashContains)
		{
            st_clientNode_baseTrans* pNode = new st_clientNodeAppLayer(this, clientHandle, 0);
			//using queued connection of send and revieve;
            //把客户端发送数据的信号与网络模块连接起来，如客户端A发出该信号，含有客户端B的标示，将由网络模块响应，并最终发给客户端B
            connect(pNode, &st_clientNode_baseTrans::evt_SendDataToClient, m_pThreadEngine, &ZPNetwork::zp_net_Engine::sendDataToClient, Qt::QueuedConnection);
            //第二组连接了客户端踢出信号，比如客户端A想踢出客户端B，则把B的标示泵出，由网络模块响应并踢出B
            connect(pNode, &st_clientNode_baseTrans::evt_Close_client, m_pThreadEngine, &ZPNetwork::zp_net_Engine::kickClients, Qt::QueuedConnection);
            //第三组连接了消息显示信号
            connect(pNode, &st_clientNode_baseTrans::evt_Message, this, &st_client_table::evt_Message, Qt::QueuedConnection);

            m_hash_sock2node[clientHandle] = pNode;
            bHashContains = true;
            pClientNode = pNode;
		}
		else
		{
            pClientNode = m_hash_sock2node[clientHandle];
		}
        assert(bHashContains != false && pClientNode != nullptr);

        int nblocks = pClientNode->push_new_data(datablock);
        if (nblocks <= 1)
        {
            m_pTaskEngine->pushTask(pClientNode);
        }
		m_hash_mutex.unlock();
	}

	//a block of data has been successfuly sent
    void st_client_table::on_evt_Data_transferred(QObject* /*clientHandle*/, qint64 /*bytes sent*/)
	{

	}

	//this event indicates new svr successfully hand-shaked.
    void st_client_table::on_evt_NewSvrConnected(QString svrName)
	{
        //Send all client UUIDs to new svr
		m_hash_mutex.lock();
		QList<quint32> uuids;
        for(std::unordered_map<quint32, st_clientNode_baseTrans*>::iterator p = m_hash_uuid2node.begin();
            p != m_hash_uuid2node.end(); ++p)
        {
            uuids.push_back((*p).first);
        }
		int nNodeSz = uuids.size();
        if (nNodeSz > 0)
		{
            int nMsgLen = sizeof(EXAMPLE_CROSSSVR_MSG::tag_msgHearder) + nNodeSz * sizeof(quint32);
            QByteArray array(nMsgLen, 0);
            EXAMPLE_CROSSSVR_MSG* pMsg = (EXAMPLE_CROSSSVR_MSG *)array.data();
			pMsg->header.Mark = 0x4567;
            pMsg->header.version = 1;
            pMsg->header.messageType = 0x01;
			pMsg->header.messageLen = nNodeSz * sizeof(quint32);
			int ct = -1;
            foreach (quint32 uuid, uuids)
            {
                pMsg->payload.uuids[++ct] = uuid;
            }
            m_pCluster->sendDataToRemoteServer(svrName, array);
		}
		m_hash_mutex.unlock();
        emit evt_Message(this, tr("Send Initial UUIDs to Remote Svr:") + svrName);
	}

	void st_client_table::broadcast_client_uuid(quint32 uuid, bool bActive)
	{
        QStringList svrs = m_pCluster->svrNames();
        if (svrs.empty())
        {
            return;
        }
        int nMsgLen = sizeof(EXAMPLE_CROSSSVR_MSG::tag_msgHearder) + sizeof(quint32);
        QByteArray array(nMsgLen, 0);
        EXAMPLE_CROSSSVR_MSG* pMsg = (EXAMPLE_CROSSSVR_MSG*)array.data();
        pMsg->header.Mark = 0x4567;
        pMsg->header.version = 1;
        pMsg->header.messageLen = sizeof(quint32);
        pMsg->header.messageType = bActive ? 0x01 : 0x02;
        pMsg->payload.uuids[0] = uuid;
        foreach (QString svrName, svrs)
        {
            m_pCluster->sendDataToRemoteServer(svrName, array);
        }
	}

    bool st_client_table::sendToNode(quint32 uuid, QByteArray array)
	{
        bool bRes = false;
		m_hash_mutex.lock();
        if (m_hash_uuid2node.find(uuid) != m_hash_uuid2node.end())
		{
            st_clientNode_baseTrans* pAppLayer = qobject_cast<st_clientNode_baseTrans*>(m_hash_uuid2node[uuid]);
			if (pAppLayer)
			{
                m_pThreadEngine->sendDataToClient(pAppLayer->socket(), array);
                bRes = true;
			}
		}
		m_hash_mutex.unlock();
        //emit evt_Message(this, tr("Recieved remote user-data to uuid:%1, DATA:%2").arg(uuid).arg(QString("HEX") + QString(array.toHex())));
        return bRes;
	}

	//this event indicates a client disconnected.
    void st_client_table::on_evt_NewSvrDisconnected(QString svrName)
	{
		//remove all client-maps belongs to this server.
        cross_svr_del_uuids(svrName, NULL, 0);
        emit evt_Message(this, "Svr DisConnected. " + svrName);
	}

	//some data arrival
    void st_client_table::on_evt_RemoteData_recieved(QString svrName, QByteArray array)
	{
        emit evt_Message(this, tr("Recieved %1 bytes Msg from ").arg(array.length()) + svrName);
	}

	//a block of data has been successfuly sent
    void st_client_table::on_evt_RemoteData_transferred(QObject* /*svrName*/, qint64 /*bytes sent*/)
	{

	}

    ZP_Cluster::zp_ClusterNode* st_client_table::cross_svr_node_factory(ZP_Cluster::zp_ClusterTerm* pTerm,
                                                                        QObject* pSocket,
                                                                        QObject* parent)
	{
        st_cross_svr_node* pNode = new st_cross_svr_node(pTerm, pSocket, parent);
		pNode->setClientTable(this);
        //connect(pNode, &st_cross_svr_node::evt_SendToNode, this, &st_client_table::sendToNode, Qt::QueuedConnection);
		return pNode;
	}

	//reg new uuids in m_hash_remoteClient2SvrName
    void st_client_table::cross_svr_add_uuids(QString svrName, quint32* pUUIDs, int nUUIDs)
	{
		m_mutex_cross_svr_map.lock();
        for (int i = 0; i < nUUIDs; i++)
        {
            m_hash_remoteClient2SvrName[pUUIDs[i]] = svrName;
        }
		m_mutex_cross_svr_map.unlock();
        emit evt_Message(this, tr("Recieved remote %1 client uuid(s) from svr ").arg(nUUIDs) + svrName);
	}

    //del uuids in m_hash_remoteClient2SvrName, pUUIDs = 0 means del all uuids belong to svrName
    void st_client_table::cross_svr_del_uuids(QString svrName, quint32* pUUIDs, int nUUIDs)
	{
		m_mutex_cross_svr_map.lock();
        if (pUUIDs == NULL)
		{
			QList<quint32> keys;
            for(std::unordered_map<quint32, QString>::iterator p =
				m_hash_remoteClient2SvrName.begin();
                p != m_hash_remoteClient2SvrName.end(); p++)
			{
                if ((*p).second == svrName)
                {
                    keys.push_back((*p).first);
                }
			}
			foreach (quint32 key, keys)
			{
				m_hash_remoteClient2SvrName.erase(key);
			}
		}
		else
		{
            for (int i = 0; i < nUUIDs; i++)
            {
                m_hash_remoteClient2SvrName.erase(pUUIDs[i]);
            }
		}
		m_mutex_cross_svr_map.unlock();
        emit evt_Message(this, tr("Removed remote %1 client uuid(s) from svr ").arg(nUUIDs) + svrName);
	}

    void st_client_table::cross_svr_send_data(QString svrName, QByteArray ary)
	{
		int nMsgLen = sizeof(EXAMPLE_CROSSSVR_MSG::tag_msgHearder);
        QByteArray array(nMsgLen, 0);
        EXAMPLE_CROSSSVR_MSG* pMsg = (EXAMPLE_CROSSSVR_MSG*)array.data();
		pMsg->header.Mark = 0x4567;
		pMsg->header.version = 1;
        pMsg->header.messageLen = ary.size();
        pMsg->header.messageType = 0x03;
        array.append(ary);
        m_pCluster->sendDataToRemoteServer(svrName, array);
	}

    QString st_client_table::cross_svr_find_uuid(quint32 uuid)
	{
		QString svr = "";
		m_mutex_cross_svr_map.lock();
        if (m_hash_remoteClient2SvrName.find(uuid) != m_hash_remoteClient2SvrName.end())
        {
            svr = m_hash_remoteClient2SvrName[uuid];
        }
		m_mutex_cross_svr_map.unlock();
        return svr;
    }

    void st_client_table::cross_web_send_data(QString svrName, QByteArray ary)
    {
        m_pCluster->sendDataToWebService(svrName, ary);
    }

    bool st_client_table::needRedirect(quint8 bufAddresses[/*64*/], quint16* pnPort)
	{
        quint32 clientNums = m_pCluster->clientNums();
        if (clientNums > (quint32)m_nBalanceMax)
        {
            return false;
        }
        QString strTermName = m_pCluster->minPayloadServer(bufAddresses, pnPort);
        if (strTermName == m_pCluster->termName())
        {
            return false;
        }
		return true;
	}
}


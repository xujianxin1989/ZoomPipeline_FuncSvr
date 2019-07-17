#include "zp_clusterterm.h"
#include "zp_clusternode.h"
#include <assert.h>
#include <QStringList>

namespace ZP_Cluster
{
	using namespace std::placeholders;
    zp_ClusterTerm::zp_ClusterTerm(QString termName, QObject* parent) :
		QObject(parent),
		m_nClientNums(0),
		m_strTermName(termName)
	{
		m_pClusterEng = new ZPTaskEngine::zp_pipeline(this);
        m_pClusterNet = new ZPNetwork::zp_net_Engine(8192, this);
        connect(m_pClusterNet, &ZPNetwork::zp_net_Engine::evt_Message, this, &zp_ClusterTerm::evt_Message, Qt::QueuedConnection);
        connect(m_pClusterNet, &ZPNetwork::zp_net_Engine::evt_SocketError, this, &zp_ClusterTerm::evt_SocketError, Qt::QueuedConnection);
        connect(m_pClusterNet, &ZPNetwork::zp_net_Engine::evt_Data_recieved, this, &zp_ClusterTerm::on_evt_Data_recieved, Qt::QueuedConnection);
        connect(m_pClusterNet, &ZPNetwork::zp_net_Engine::evt_Data_transferred, this, &zp_ClusterTerm::on_evt_Data_transferred, Qt::QueuedConnection);
        connect(m_pClusterNet, &ZPNetwork::zp_net_Engine::evt_NewClientConnected, this, &zp_ClusterTerm::on_evt_NewClientConnected, Qt::QueuedConnection);
        connect(m_pClusterNet, &ZPNetwork::zp_net_Engine::evt_ClientDisconnected, this, &zp_ClusterTerm::on_evt_ClientDisconnected, Qt::QueuedConnection);
        //connect(m_pClusterNet, &ZPNetwork::zp_net_Engine::evt_ClientEncrypted, this, &zp_ClusterTerm::on_evt_ClientEncrypted, Qt::QueuedConnection);

        m_nPortLAN = 0;
        m_nPortPub = 0;
		m_nHeartBeatingTime = 20;
        m_factory = std::bind(&zp_ClusterTerm::default_factory, this, _1, _2, _3);
	}

    void zp_ClusterTerm::setClientNums(quint32 num)
	{
        m_nClientNums = num;
	}

	quint32 zp_ClusterTerm::clientNums() const
	{
		return m_nClientNums;
	}

	/**
	 * @brief The factory enables user-defined sub-classes inherits from zp_ClusterNode
     * Using setNodeFactory, set your own allocate method.
     * @fn zp_ClusterTerm::default_factory the default factory function. just return zp_ClusterTerm*
	 * @param pTerm Term object
     * @param pSocket Socket Object
	 * @param parent Parent
     * @return zp_ClusterNode*
	 */
    zp_ClusterNode* zp_ClusterTerm::default_factory(zp_ClusterTerm* pTerm,
                                                    QObject* pSocket,
                                                    QObject* parent)
	{
        return new zp_ClusterNode(pTerm, pSocket, parent);
	}

	/**
     * @brief Using setNodeFactory, set your own allocate method.
	 *
     * @fn zp_ClusterTerm::setNodeFactory
	 * @param factory The functor
	 */
    void zp_ClusterTerm::setNodeFactory(std::function<zp_ClusterNode* (zp_ClusterTerm* /*pTerm*/, QObject* /*pSocket*/, QObject* /*parent*/)>factory)
	{
        m_factory = factory;
	}

    int zp_ClusterTerm::LANPort() const
    {
		return m_nPortLAN;
	}

	QHostAddress zp_ClusterTerm::LANAddr() const
	{
		return m_addrLAN;
	}

	QHostAddress zp_ClusterTerm::setLANAddr(QHostAddress addr)
	{
        return m_addrLAN = addr;
	}

	int zp_ClusterTerm::setLANPort(int port)
	{
		return m_nPortLAN = port;
	}

    int zp_ClusterTerm::PublishPort() const
    {
		return m_nPortPub;
	}

	QHostAddress zp_ClusterTerm::PublishAddr() const
	{
		return m_addrPub;
	}

	QHostAddress zp_ClusterTerm::setPublishAddr(QHostAddress addr)
	{
		return m_addrPub = addr;
	}

	int zp_ClusterTerm::setPublishPort(int port)
	{
		return m_nPortPub = port;
	}

    ZPNetwork::zp_net_Engine* zp_ClusterTerm::netEng() const
    {
		return m_pClusterNet;
	}

    ZPTaskEngine::zp_pipeline* zp_ClusterTerm::taskEng() const
    {
		return m_pClusterEng;
	}

    QString zp_ClusterTerm::setTermName(QString termName)
	{
        return m_strTermName = termName;
	}

    QString zp_ClusterTerm::termName() const
    {
		return m_strTermName;
	}

	int zp_ClusterTerm::heartBeatingThrdHold() const
	{
		return m_nHeartBeatingTime;
	}

	void zp_ClusterTerm::setHeartBeatingThrd(const int n)
	{
		m_nHeartBeatingTime = n;
	}

    void zp_ClusterTerm::startListen(const QHostAddress& address, int nPort)
	{
        m_pClusterNet->addListeningAddress(m_strTermName, address, nPort, false);
	}

    bool zp_ClusterTerm::joinCluster(const QHostAddress& address, int nPort, bool bSSL)
	{
        return m_pClusterNet->connectTo(address, nPort, bSSL);
	}

	bool zp_ClusterTerm::canExit() const
	{
        return m_pClusterEng->canClose() && m_pClusterNet->canExit();
	}

    QStringList zp_ClusterTerm::svrNames()
	{
        QStringList list;
		m_hash_mutex.lock();
        QList<QString> keys = m_hash_name2node.keys();
        foreach (QString key, keys)
		{
            list.push_back(m_hash_name2node[key]->termName());
		}
		m_hash_mutex.unlock();
        return list;
	}

    QHostAddress zp_ClusterTerm::svrLANAddr(QString svrName)
	{
		QHostAddress addr;
		m_hash_mutex.lock();
        if (m_hash_name2node.contains(svrName))
        {
            addr = m_hash_name2node[svrName]->addrLAN();
        }
		m_hash_mutex.unlock();
		return addr;
	}

    int zp_ClusterTerm::svrLANPort(QString svrName)
	{
		int port = 0;
		m_hash_mutex.lock();
        if (m_hash_name2node.contains(svrName))
        {
            port = m_hash_name2node[svrName]->portLAN();
        }
		m_hash_mutex.unlock();
		return port;
	}

    QHostAddress zp_ClusterTerm::svrPubAddr(QString svrName)
	{
		QHostAddress addr;
		m_hash_mutex.lock();
        if (m_hash_name2node.contains(svrName))
        {
            addr = m_hash_name2node[svrName]->addrPub();
        }
		m_hash_mutex.unlock();
		return addr;
	}

    int zp_ClusterTerm::svrPubPort(QString svrName)
	{
		int port = 0;
		m_hash_mutex.lock();
        if (m_hash_name2node.contains(svrName))
        {
            port = m_hash_name2node[svrName]->portPub();
        }
		m_hash_mutex.unlock();
		return port;
	}

    quint32 zp_ClusterTerm::remoteClientNums(QString svrName)
	{
		quint32 res = 0;
		m_hash_mutex.lock();
        if (m_hash_name2node.contains(svrName))
        {
            res = m_hash_name2node[svrName]->clientNums();
        }
		m_hash_mutex.unlock();
		return res;
	}

    bool zp_ClusterTerm::registNewServer(zp_ClusterNode* pNode)
	{
        //Before regist, term name must be recieved.
        if (pNode->termName().isEmpty())
		{
            emit evt_Message(pNode, tr("Term name is empty!") + pNode->termName());
			return false;
		}
		m_hash_mutex.lock();
        if (m_hash_name2node.contains(pNode->termName()))
		{
			m_hash_mutex.unlock();
			return false;
		}
        m_hash_name2node[pNode->termName()] = pNode;
		m_hash_mutex.unlock();
		return true;
	}

    zp_ClusterNode* zp_ClusterTerm::svrNodeFromName(QString svrName)
	{
		m_hash_mutex.lock();
        if (m_hash_name2node.contains(svrName))
		{
			m_hash_mutex.unlock();
            return m_hash_name2node[svrName];
		}
		m_hash_mutex.unlock();
		return nullptr;
	}

    zp_ClusterNode* zp_ClusterTerm::svrNodeFromSocket(QObject* socket)
	{
		m_hash_mutex.lock();
        if (m_hash_sock2node.contains(socket))
		{
			m_hash_mutex.unlock();
            return m_hash_sock2node[socket];
		}
		m_hash_mutex.unlock();
		return nullptr;
	}

	//this event indicates new client connected.
    void zp_ClusterTerm::on_evt_NewClientConnected(QObject* clientHandle)
	{
        bool bHashContains;
        zp_ClusterNode *pClientNode;
		m_hash_mutex.lock();
        bHashContains = m_hash_sock2node.contains(clientHandle);
        if (bHashContains)
        {
            pClientNode = m_hash_sock2node[clientHandle];
        }
        else
		{
            zp_ClusterNode* pNode = m_factory(this, clientHandle, nullptr);
			//using queued connection of send and revieve;
            connect(pNode, &zp_ClusterNode::evt_SendDataToClient, m_pClusterNet, &ZPNetwork::zp_net_Engine::sendDataToClient, Qt::QueuedConnection);
            connect(pNode, &zp_ClusterNode::evt_Close_client, m_pClusterNet, &ZPNetwork::zp_net_Engine::kickClients, Qt::QueuedConnection);
            connect(pNode, &zp_ClusterNode::evt_Connect_to, m_pClusterNet, &ZPNetwork::zp_net_Engine::connectTo, Qt::QueuedConnection);
            connect(pNode, &zp_ClusterNode::evt_RemoteData_recieved, this, &zp_ClusterTerm::evt_RemoteData_recieved, Qt::QueuedConnection);
            connect(pNode, &zp_ClusterNode::evt_NewSvrConnected, this, &zp_ClusterTerm::evt_NewSvrConnected, Qt::QueuedConnection);
            connect(pNode, &zp_ClusterNode::evt_Message, this, &zp_ClusterTerm::evt_Message, Qt::QueuedConnection);
            m_hash_sock2node[clientHandle] = pNode;
            bHashContains = true;
            pClientNode = pNode;
		}
		m_hash_mutex.unlock();
        assert(bHashContains != false && pClientNode != nullptr);
        //Send Hello Package(exchange basicinfo)
        //pClientNode->sendHelloPackage();
        pClientNode->sendOptionsPackage();
	}

	//this event indicates new client encrypted.
    void zp_ClusterTerm::on_evt_ClientEncrypted(QObject* clientHandle)
	{
        bool bHashContains;
        zp_ClusterNode *pClientNode;
		m_hash_mutex.lock();
        bHashContains = m_hash_sock2node.contains(clientHandle);
        if (bHashContains)
        {
            pClientNode = m_hash_sock2node[clientHandle];
        }
        else
		{
            zp_ClusterNode* pNode = m_factory(this, clientHandle, nullptr);
			//using queued connection of send and revieve;
            connect(pNode, &zp_ClusterNode::evt_SendDataToClient, m_pClusterNet, &ZPNetwork::zp_net_Engine::sendDataToClient, Qt::QueuedConnection);
            connect(pNode, &zp_ClusterNode::evt_Close_client, m_pClusterNet, &ZPNetwork::zp_net_Engine::kickClients, Qt::QueuedConnection);
            connect(pNode, &zp_ClusterNode::evt_Message, this, &zp_ClusterTerm::evt_Message, Qt::QueuedConnection);
            connect(pNode, &zp_ClusterNode::evt_Connect_to, m_pClusterNet, &ZPNetwork::zp_net_Engine::connectTo, Qt::QueuedConnection);
            connect(pNode, &zp_ClusterNode::evt_NewSvrConnected, this, &zp_ClusterTerm::evt_NewSvrConnected, Qt::QueuedConnection);
            connect(pNode, &zp_ClusterNode::evt_RemoteData_recieved, this, &zp_ClusterTerm::evt_RemoteData_recieved, Qt::QueuedConnection);
            m_hash_sock2node[clientHandle] = pNode;
            bHashContains = true;
            pClientNode = pNode;
		}
		m_hash_mutex.unlock();
        assert(bHashContains != false && pClientNode != nullptr);
        //Send Hello Package(exchange basicinfo)
        pClientNode->sendHelloPackage();
	}

	//this event indicates a client disconnected.
    void zp_ClusterTerm::on_evt_ClientDisconnected(QObject* clientHandle)
	{
        bool bHashContains;
        zp_ClusterNode *pClientNode = nullptr;
		m_hash_mutex.lock();
        bHashContains = m_hash_sock2node.contains(clientHandle);
        if (bHashContains)
        {
            pClientNode = m_hash_sock2node[clientHandle];
        }
		if (pClientNode)
		{
            QString termName = pClientNode->termName();
            disconnect(pClientNode, &zp_ClusterNode::evt_SendDataToClient, m_pClusterNet, &ZPNetwork::zp_net_Engine::sendDataToClient);
            disconnect(pClientNode, &zp_ClusterNode::evt_Close_client, m_pClusterNet, &ZPNetwork::zp_net_Engine::kickClients);
            disconnect(pClientNode, &zp_ClusterNode::evt_Message, this, &zp_ClusterTerm::evt_Message);
            disconnect(pClientNode, &zp_ClusterNode::evt_Connect_to, m_pClusterNet, &ZPNetwork::zp_net_Engine::connectTo);
            disconnect(pClientNode, &zp_ClusterNode::evt_NewSvrConnected, this, &zp_ClusterTerm::evt_NewSvrConnected);
            disconnect(pClientNode, &zp_ClusterNode::evt_RemoteData_recieved, this, &zp_ClusterTerm::evt_RemoteData_recieved);
            m_hash_sock2node.remove(clientHandle);
            if (!termName.isEmpty())
			{
                //This is important. some time m_hash_name2node and m_hash_sock2node, same uuid has different socket.
                if (m_hash_name2node.contains(termName))
                {
                    if (m_hash_name2node[termName] == pClientNode)
                    {
                        m_hash_name2node.remove(termName);
                    }
                }
			}

			pClientNode->bTermSet = true;
			m_nodeToBeDel.push_back(pClientNode);
            if (!termName.isEmpty())
            {
                emit evt_NewSvrDisconnected(termName);
            }
		}
		m_hash_mutex.unlock();

		//Try to delete objects
        QList<zp_ClusterNode*> toBedel;
        foreach(zp_ClusterNode* pdelObj, m_nodeToBeDel)
		{
            if (0 == pdelObj->ref())
            {
                toBedel.push_back(pdelObj);
            }
			else
			{
                //qDebug() << QString("%1(ref %2) Waiting in del queue.").arg((uint)pdelObj).arg(pdelObj->ref());
			}
		}
        foreach(zp_ClusterNode* pdelObj, toBedel)
		{
            m_nodeToBeDel.removeAll(pdelObj);
            pdelObj->deleteLater();
		}
	}

	//some data arrival
    void zp_ClusterTerm::on_evt_Data_recieved(QObject* clientHandle, QByteArray datablock)
	{
		//Push Clients to nodes if it is not exist
        bool bHashContains;
        zp_ClusterNode *pClientNode;
		m_hash_mutex.lock();
        bHashContains = m_hash_sock2node.contains(clientHandle);
        if (bHashContains)
        {
            pClientNode = m_hash_sock2node[clientHandle];
        }
        else
		{
            zp_ClusterNode *pNode = m_factory(this, clientHandle, nullptr);
			//using queued connection of send and revieve;
            connect(pNode, &zp_ClusterNode::evt_SendDataToClient, m_pClusterNet, &ZPNetwork::zp_net_Engine::sendDataToClient, Qt::QueuedConnection);
            connect(pNode, &zp_ClusterNode::evt_Close_client, m_pClusterNet, &ZPNetwork::zp_net_Engine::kickClients, Qt::QueuedConnection);
            connect(pNode, &zp_ClusterNode::evt_Connect_to, m_pClusterNet, &ZPNetwork::zp_net_Engine::connectTo, Qt::QueuedConnection);
            connect(pNode, &zp_ClusterNode::evt_Message, this, &zp_ClusterTerm::evt_Message, Qt::QueuedConnection);
            connect(pNode, &zp_ClusterNode::evt_NewSvrConnected, this, &zp_ClusterTerm::evt_NewSvrConnected, Qt::QueuedConnection);
            connect(pNode, &zp_ClusterNode::evt_RemoteData_recieved, this, &zp_ClusterTerm::evt_RemoteData_recieved, Qt::QueuedConnection);
            m_hash_sock2node[clientHandle] = pNode;
            bHashContains = true;
            pClientNode = pNode;
		}
        assert(bHashContains != false && pClientNode != nullptr);
        int nBlocks = pClientNode->push_new_data(datablock);
        if (nBlocks <= 1)
        {
            m_pClusterEng->pushTask(pClientNode);
        }
		m_hash_mutex.unlock();
	}

    void zp_ClusterTerm::kickDeadClients()
	{
		m_hash_mutex.lock();
        for (QMap<QObject*, zp_ClusterNode*>::iterator it = m_hash_sock2node.begin(); it != m_hash_sock2node.end(); ++it)
		{
            it.value()->checkHeartBeating();
		}
		m_hash_mutex.unlock();
	}

	//a block of data has been successfuly sent
    void zp_ClusterTerm::on_evt_Data_transferred(QObject* clientHandle, qint64 bytesSent)
	{
        emit evt_RemoteData_transferred(clientHandle, bytesSent);
	}

    void zp_ClusterTerm::broadcastServers()
	{
        if (m_strTermName.isEmpty())
        {
            return;
        }
		m_hash_mutex.lock();
        QList<QString> keys = m_hash_name2node.keys();
        int size = keys.size();
		//Msgs
        int nMsgLen = sizeof(CROSS_SVR_MSG::tag_header) + sizeof(CROSS_SVR_MSG::uni_payload::tag_CSM_Broadcast) * (size + 1);
        QByteArray array(nMsgLen, 0);
        CROSS_SVR_MSG* pMsg = (CROSS_SVR_MSG *)(array.data());
		pMsg->hearder.Mark = 0x1234;
        pMsg->hearder.data_length = sizeof(CROSS_SVR_MSG::uni_payload::tag_CSM_Broadcast) * (size + 1);
        pMsg->hearder.message_type = 0x02;
        int index = 0;
        foreach (QString key, keys)
		{
            zp_ClusterNode* pNode = m_hash_name2node[key];

            strncpy((char*)pMsg->payload.broadcastMsg[index].name,
					pNode->termName().toStdString().c_str(),
                    sizeof(pMsg->payload.broadcastMsg[index].name) - 1);

            strncpy((char*)pMsg->payload.broadcastMsg[index].address_LAN,
                    pNode->addrLAN().toString().toStdString().c_str(),
                    sizeof(pMsg->payload.broadcastMsg[index].address_LAN) - 1);

            pMsg->payload.broadcastMsg[index].port_LAN = pNode->portLAN();

            strncpy((char*)pMsg->payload.broadcastMsg[index].address_Pub,
					pNode->addrPub().toString().toStdString().c_str(),
                    sizeof(pMsg->payload.broadcastMsg[index].address_Pub) - 1);

            pMsg->payload.broadcastMsg[index].port_Pub = pNode->portPub();

            ++index;
		}
		m_hash_mutex.unlock();
        strncpy((char*)pMsg->payload.broadcastMsg[index].name,
                this->termName().toStdString().c_str(),
                sizeof(pMsg->payload.broadcastMsg[index].name) - 1);

        strncpy((char*)pMsg->payload.broadcastMsg[index].address_LAN,
				this->LANAddr().toString().toStdString().c_str(),
                sizeof(pMsg->payload.broadcastMsg[index].address_LAN) - 1);

        pMsg->payload.broadcastMsg[index].port_LAN = this->LANPort();

        strncpy((char*)pMsg->payload.broadcastMsg[index].address_Pub,
				this->PublishAddr().toString().toStdString().c_str(),
                sizeof(pMsg->payload.broadcastMsg[index].address_Pub) - 1);

        pMsg->payload.broadcastMsg[index].port_Pub = this->PublishPort();

		m_hash_mutex.lock();
        QList<QString> sockkeys = m_hash_name2node.keys();
		//Msgs
        foreach (QString key, sockkeys)
		{
            netEng()->sendDataToClient(m_hash_name2node[key]->socket(), array);
		}
		m_hash_mutex.unlock();
	}

    void zp_ClusterTerm::sendHeartBeatings()
	{
		int nMsgLen = sizeof(CROSS_SVR_MSG::tag_header) + sizeof(CROSS_SVR_MSG::uni_payload::tag_CSM_heartBeating);
        QByteArray array(nMsgLen, 0);
        CROSS_SVR_MSG* pMsg = (CROSS_SVR_MSG*)(array.data());
		pMsg->hearder.Mark = 0x1234;
		pMsg->hearder.data_length = sizeof(CROSS_SVR_MSG::uni_payload::tag_CSM_heartBeating);
        pMsg->hearder.message_type = 0x00;
        pMsg->payload.heartBeating.nClients = m_nClientNums;
        //Broadcast Data;
		m_hash_mutex.lock();
        QList<QString> keys = m_hash_name2node.keys();
		//Msgs
        foreach (QString key, keys)
		{
            netEng()->sendDataToClient(m_hash_name2node[key]->socket(), array);
		}
		m_hash_mutex.unlock();
	}

    void zp_ClusterTerm::sendDataToRemoteServer(QString svrName, QByteArray sourceArray)
	{
        int nMsgLen = sizeof(CROSS_SVR_MSG::tag_header) + sourceArray.size();
        QByteArray array(nMsgLen, 0);
        CROSS_SVR_MSG* pMsg = (CROSS_SVR_MSG*)(array.data());
        pMsg->hearder.Mark = 0x1234;
        pMsg->hearder.message_type = 0x03;
        pMsg->hearder.data_length = sourceArray.size();
        memcpy(pMsg->payload.data, sourceArray.constData(), sourceArray.size());
		m_hash_mutex.lock();
        if (m_hash_name2node.contains(svrName))
        {
            netEng()->sendDataToClient(m_hash_name2node[svrName]->socket(), array);
        }
        m_hash_mutex.unlock();
    }

    void zp_ClusterTerm::sendDataToWebService(QString svrName, QByteArray sourceArray)
    {
        QString strRequest = QString("POST /WebService.asmx/ReceiveData HTTP/1.1\r\n"
                                     "Host: localhost\r\n"
                                     "User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:50.0) Gecko/20100101 Firefox/50.0\r\n"
                                     "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
                                     "Accept-Language: zh-CN,zh;q=0.8,en-US;q=0.5,en;q=0.3\r\n"
                                     "Accept-Encoding: gzip, deflate\r\n"
                                     "Connection: keep-alive\r\n"
                                     "Content-Type: application/x-www-form-urlencoded\r\n"
                                     "Content-Length: 30\r\n\r\n") +
                             QString("detectorId=%1%2&").arg(sourceArray.at(6), 2, 16, QChar('0')).arg(sourceArray.at(5), 2, 16, QChar('0')) +
                             QString("detectorType=SMARTSENSOR&") +
                             QString("detCode=%1&").arg(sourceArray.at(18)) +
                             QString("dateTime=%1-%2-%3 %4:%5:00&").arg(2000 + sourceArray.at(9), 4, 10, QChar('0'))
                                                                   .arg(sourceArray.at(10), 2, 10, QChar('0'))
                                                                   .arg(sourceArray.at(11), 2, 10, QChar('0'))
                                                                   .arg(sourceArray.at(12), 2, 10, QChar('0'))
                                                                   .arg(sourceArray.at(13), 2, 10, QChar('0')) +
                             QString("volume=%1&").arg(sourceArray.at(24) * 256 + sourceArray.at(23)) +
                             QString("speed=%1&").arg(sourceArray.at(19)) +
                             "timeOpy=&" +
                             QString("headTime=%1&").arg(sourceArray.at(22) * 256 + sourceArray.at(21)) +
                             "headDistance=&" +
                             QString("saturation=%1&").arg(sourceArray.at(20)) +
                             "density=&" +
                             "avgLength=&" +
                             "volume1=&" +
                             QString("volume2=%1&").arg(sourceArray.at(26) * 256 + sourceArray.at(25)) +
                             QString("volume3=%1&").arg(sourceArray.at(28) * 256 + sourceArray.at(27)) +
                             QString("volume4=%1&").arg(sourceArray.at(30) * 256 + sourceArray.at(29)) +
                             QString("volume5=%1&").arg(sourceArray.at(32) * 256 + sourceArray.at(31)) +
                             "interval=1";

        QByteArray array = strRequest.toLatin1();
        m_hash_mutex.lock();
        if (m_hash_name2node.contains(svrName))
        {
            netEng()->sendDataToClient(m_hash_name2node[svrName]->socket(), array);
        }
        m_hash_mutex.unlock();
    }

    QString zp_ClusterTerm::minPayloadServer(quint8 bufAddresses[/*64*/], quint16* pnPort)
	{
        QString serverName = this->termName();
        strncpy((char*)bufAddresses, this->PublishAddr().toString().toStdString().c_str(), 64);
		*pnPort = this->PublishPort();
		m_hash_mutex.lock();
        QList<QString> keys = m_hash_name2node.keys();
        if (0 == keys.size())
		{
			m_hash_mutex.unlock();
			return serverName;
		}

		//Msgs
		int nMinVal = this->m_nClientNums;
        zp_ClusterNode *pMinNode = nullptr;
        foreach (QString key, keys)
		{
            zp_ClusterNode* pNode = m_hash_name2node[key];
            if (pNode->clientNums() < (quint32)nMinVal)
			{
				nMinVal = pNode->clientNums();
				pMinNode = pNode;
			}
		}
		if (pMinNode)
		{
            strncpy((char*)bufAddresses, pMinNode->addrPub().toString().toStdString().c_str(), 64);
			*pnPort = pMinNode->portPub();
			serverName = pMinNode->termName();
		}
		m_hash_mutex.unlock();
		return serverName;
	}
}

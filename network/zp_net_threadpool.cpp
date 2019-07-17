#include "zp_net_threadpool.h"
#include <QCoreApplication>
#include <QStringList>

namespace ZPNetwork
{
	/**
	 * @brief Construct a net thread pool object.
	 *
	 * @fn zp_net_Engine
	 * @param nPayLoad The data to be sent in buffer will be cutted into pieces, each pieces equals to nPayLoad bytes
	 * @param parent a Qt-style parent pointer. this object will be auto-deleted when parent is about to be destoryed.
	 */
    zp_net_Engine::zp_net_Engine(int nPayLoad, QObject* parent) : QObject(parent)
	{
        m_nPayLoad = nPayLoad;
        //The signals and slots will be reged.
        if (!QMetaType::isRegistered(QMetaType::type("qintptr")))
        {
            qRegisterMetaType<qintptr>("qintptr");
        }
        if (!QMetaType::isRegistered(QMetaType::type("QAbstractSocket::SocketError")))
        {
            qRegisterMetaType<QAbstractSocket::SocketError>("QAbstractSocket::SocketError");
        }
        if (!QMetaType::isRegistered(QMetaType::type("QHostAddress")))
        {
            qRegisterMetaType<QHostAddress>("QHostAddress");
        }
	}

	/**
	 * @brief return all of these listener names.
	 *
     * @fn zp_net_Engine::listenerNames
	 * @return QStringList listener names.
	 */
    QStringList zp_net_Engine::listenerNames() const
    {
		return m_map_netListenThreads.keys();
	}

	/**
	 * @brief Return how many threads is now actived for TCP transfer.
	 *
     * @fn zp_net_Engine::transThreadNum
	 * @return int 0 means no thread is active.
	 */
    int zp_net_Engine::transThreadNum() const
    {
		return m_vec_NetTransThreads.size();
	}

	/**
	 * @brief Return how many threads is now actived for SSL/Plain TCP transfer.
	 *
     * @fn zp_net_Engine::transThreadNum
	 * @param bSSL true means SSL Connection, false is Plain Connection
	 * @return int The transfer thread count for SSL connections.
	 */
    int zp_net_Engine::transThreadNum(bool bSSL)
	{
		//m_mutex_trans.lock();
        int size = m_vec_NetTransThreads.size();
		int nCount = 0;
        for (int i = 0; i < size; i++)
		{
            if (m_vec_NetTransThreads[i]->SSLConnection() == bSSL)
            {
                nCount++;
            }
		}
		//m_mutex_trans.unlock();
		return nCount;
	}

	/**
	 * @brief This function returns how many clients managed by idxThread is now online.
	 *
	 * @fn zp_net_Engine::totalClients
     * @param idxThread The thread index between 0 and transThreadNum() - 1
	 * @return int Clients which are currently online
	 */
	int zp_net_Engine::totalClients(int idxThread)
	{
        int size = m_vec_NetTransThreads.size();
        if (idxThread >= 0 && idxThread < size)
        {
            return m_vec_NetTransThreads[idxThread]->currentClients();
        }
        return 0;
	}

	/**
     * @brief Begin a listening socket at special address and port.
     * The socket will be activated as soon as possible
	 * The Wait-and-Accept approaches are all managed by Listening thread, instead of main-GUI thread.
     *
	 * @fn zp_net_Engine::AddListeningAddress
     * @param id The name of this listening address:port. You can later using removeListeningAddress(id) to close it.
	 * @param address Listening address. IPV6 is now not tested.
	 * @param nPort Listening Port. in Linux, this value should larger than 1024.
     * @param bSSLConn True means this address using OpenSSL. False means plain TCP
	 */
    void zp_net_Engine::addListeningAddress(QString id,
                                            const QHostAddress& address,
                                            quint16 nPort,
                                            bool bSSLConn /*= true*/)
	{
        if (m_map_netListenThreads.find(id) == m_map_netListenThreads.end())
		{
			//Start Thread
            QThread* pThread = new QThread(this);
            zp_netListenThread* pListenObject = new zp_netListenThread(id, address, nPort, bSSLConn);
			pThread->start();

			//m_mutex_listen.lock();
			m_map_netInternalListenThreads[id] = pThread;
			m_map_netListenThreads[id] = pListenObject;
			//m_mutex_listen.unlock();

			//Bind Object to New thread
            connect(this, &zp_net_Engine::startListen, pListenObject, &zp_netListenThread::startListen, Qt::QueuedConnection);
            connect(this, &zp_net_Engine::stopListen, pListenObject, &zp_netListenThread::stopListen, Qt::QueuedConnection);
            connect(pListenObject, &zp_netListenThread::evt_Message, this, &zp_net_Engine::evt_Message, Qt::QueuedConnection);
            connect(pListenObject, &zp_netListenThread::evt_ListenClosed, this, &zp_net_Engine::on_ListenClosed, Qt::QueuedConnection);
            connect(pListenObject, &zp_netListenThread::evt_NewClientArrived, this, &zp_net_Engine::on_New_Arrived_Client, Qt::QueuedConnection);
			pListenObject->moveToThread(pThread);

            //立刻开始监听
			emit startListen(id);
		}
		else
        {
            emit evt_Message(this, "Warning>" + QString(tr("This ID has been used.")));
        }
	}

	/**
     * @brief Remove a listening socket at special address and port.
     * The socket will be deactivated as soon as possible
	 *
     * @fn zp_net_Engine::removeListeningAddress
     * @param id Listening-Address name specified by addListeningAddress()
	 */
    void zp_net_Engine::removeListeningAddress(QString id)
	{
		//m_mutex_listen.lock();
        if (m_map_netListenThreads.find(id) != m_map_netListenThreads.end())
        {
            emit stopListen(id);
        }
		//m_mutex_listen.unlock();
	}

	/**
	 * @brief Cancel all addresses.
	 *
     * @fn zp_net_Engine::removeAllAddresses
	 */
    void zp_net_Engine::removeAllAddresses()
	{
		//m_mutex_listen.lock();
        foreach (QString id, m_map_netListenThreads.keys())
        {
            emit stopListen(id);
        }
		//m_mutex_listen.unlock();
	}

	/**
     * @brief This slot recieved incomming client socketDescriptor from listening threads.
     * In this function, zp_net_Engine will make a balance control, choosing a trans-thread
     * which currently holds least clients than others.
	 * @fn zp_net_Engine::on_New_Arrived_Client
	 * @param socketDescriptor the socket descriptor for incomming client.
	 */
	void zp_net_Engine::on_New_Arrived_Client(qintptr socketDescriptor)
	{
        //网络模块首先从当前可用的传输线程中确定最空闲的那个线程, 而后把套接字描述符转交给传输线程
        zp_netListenThread* pSource = qobject_cast<zp_netListenThread*>(sender());
		if (!pSource)
		{
            emit evt_Message(this, "Warning>" + QString(tr("Non-zp_netListenThread type detected.")));
			return;
		}

		int nMinPay = 0x7fffffff;
		int nMinIdx = -1;
        int size = m_vec_NetTransThreads.size();
        for (int i = 0; i < size && nMinPay != 0; i++)
		{
            if (!m_vec_NetTransThreads[i]->isActive() ||
                m_vec_NetTransThreads[i]->SSLConnection() != pSource->bSSLConn())
            {
                continue;
            }

            int nPat = m_vec_NetTransThreads[i]->currentClients();
            if (nPat < nMinPay)
			{
				nMinPay = nPat;
				nMinIdx = i;
			}
		}

		//remove old threads
        for (int i = 0; i < size; i++)
        {
            if (m_vec_NetTransThreads[i]->isActive())
            {
                continue;
            }
            transThreadDel(m_vec_NetTransThreads[i]);
        }
        if (nMinIdx >= 0 && nMinIdx < size)
        {
			//确定连接
            //evt_EstablishConnection 事件携带了由均衡策略确定的承接线程、
            //socketDescriptor 描述符这个事件广播给所有的传输线程对象
            emit evt_EstablishConnection(m_vec_NetTransThreads[nMinIdx], socketDescriptor);
        }
		else
		{
            emit evt_Message(this, "Warning>" + QString(tr("Need Trans Thread Object for clients.")));
            qCritical() << tr("Need Trans Thread Object for clients.");
		}
	}

	/**
	 * @brief This is a slot function. when a listening thread is terminated,
	 * the thread itself will be deleted here. Some cleanning works is also needed.
	 * @fn zp_net_Engine::on_ListenClosed
	 * @param id the terminated thread id.
	 */
    void zp_net_Engine::on_ListenClosed(QString id)
	{
		//m_mutex_listen.lock();
        if (m_map_netListenThreads.find(id) != m_map_netListenThreads.end())
		{
			//Clean objects;
            zp_netListenThread* pListenObj = m_map_netListenThreads[id];
            QThread* pThread = m_map_netInternalListenThreads[id];
			m_map_netListenThreads.remove(id);
			m_map_netInternalListenThreads.remove(id);
			//disconnect signals;
            disconnect(this, &zp_net_Engine::startListen, pListenObj, &zp_netListenThread::startListen);
            disconnect(this, &zp_net_Engine::stopListen, pListenObj, &zp_netListenThread::stopListen);
            disconnect(pListenObj, &zp_netListenThread::evt_Message, this, &zp_net_Engine::evt_Message);
            disconnect(pListenObj, &zp_netListenThread::evt_ListenClosed, this, &zp_net_Engine::on_ListenClosed);
            disconnect(pListenObj, &zp_netListenThread::evt_NewClientArrived, this, &zp_net_Engine::on_New_Arrived_Client);
			pListenObj->deleteLater();
			pThread->quit();
			pThread->wait();
			pThread->deleteLater();
		}
		//m_mutex_listen.unlock();
	}

	//Add n client-Trans Thread(s).
	/**
	 * @brief Add some threads to hold transfer between server and client.
	 * The send and recieve approaches are all managed by these threads, instead of main-GUI thread.
     * @fn zp_net_Engine::addClientTransThreads
	 * @param nThreads
	 * @param bSSL
	 */
    void zp_net_Engine::addClientTransThreads(int nThreads, bool bSSL)
	{
        if (nThreads <= 0 || nThreads >= 256)
        {
            return;
        }

        for (int i = 0; i < nThreads; i++)
        {
            zp_netTransThread* transThread = new zp_netTransThread(this, m_nPayLoad);
            transThread->setSSLConnection(bSSL);
            QThread* pThread = new QThread(this);
            //m_mutex_trans.lock();
            m_vec_netInternalTransThreads.push_back(pThread);
            m_vec_NetTransThreads.push_back(transThread);
            //m_mutex_trans.unlock();
            pThread->start();
            //Connect signals
            connect(transThread, &zp_netTransThread::evt_ClientDisconnected, this, &zp_net_Engine::evt_ClientDisconnected, Qt::QueuedConnection);
            connect(transThread, &zp_netTransThread::evt_Data_recieved, this, &zp_net_Engine::evt_Data_recieved, Qt::QueuedConnection);
            connect(transThread, &zp_netTransThread::evt_Data_transferred, this, &zp_net_Engine::evt_Data_transferred, Qt::QueuedConnection);
            connect(transThread, &zp_netTransThread::evt_NewClientConnected, this, &zp_net_Engine::evt_NewClientConnected, Qt::QueuedConnection);
            connect(transThread, &zp_netTransThread::evt_ClientEncrypted, this, &zp_net_Engine::evt_ClientEncrypted, Qt::QueuedConnection);
            connect(transThread, &zp_netTransThread::evt_SocketError, this, &zp_net_Engine::evt_SocketError, Qt::QueuedConnection);
            connect(transThread, &zp_netTransThread::evt_Message, this, &zp_net_Engine::evt_Message, Qt::QueuedConnection);
            connect(this, &zp_net_Engine::evt_EstablishConnection, transThread, &zp_netTransThread::incomingConnection, Qt::QueuedConnection);
            connect(this, &zp_net_Engine::evt_FireConnection, transThread, &zp_netTransThread::startConnection, Qt::QueuedConnection);
            connect(this, &zp_net_Engine::evt_SendDataToClient, transThread, &zp_netTransThread::sendDataToClient, Qt::QueuedConnection);
            connect(this, &zp_net_Engine::evt_KickAll, transThread, &zp_netTransThread::kickAllClients, Qt::QueuedConnection);
            connect(this, &zp_net_Engine::evt_DeactivteImmediately, transThread, &zp_netTransThread::deactivateImmediately, Qt::QueuedConnection);
            connect(this, &zp_net_Engine::evt_KickClient, transThread, &zp_netTransThread::kickClient, Qt::QueuedConnection);

            transThread->moveToThread(pThread);
        }
	}

	/**
	 * @brief This slot will be called, when an Trans-thread is about to close.
	 * Some cleanning works will be done ,all resources will be freeed.
     * @fn zp_net_Engine::transThreadDel
	 * @param pThreadObj The closing thread object.
	 * @return bool in normal situation, this slot is always return true.
	 */
    bool zp_net_Engine::transThreadDel(zp_netTransThread* pThreadObj)
	{
        if (!pThreadObj->canExit())
        {
            return false;
        }
        int size = m_vec_NetTransThreads.size();
        int index = -1;
        for (int i = 0; i < size && index < 0; i++)
		{
            if (m_vec_NetTransThreads[i] == pThreadObj)
            {
                index = i;
            }
		}
        if (index >= 0 && index < size)
		{
            zp_netTransThread* transThread =  m_vec_NetTransThreads[index];
            disconnect(transThread, &zp_netTransThread::evt_ClientDisconnected, this, &zp_net_Engine::evt_ClientDisconnected);
            disconnect(transThread, &zp_netTransThread::evt_Data_recieved, this, &zp_net_Engine::evt_Data_recieved);
            disconnect(transThread, &zp_netTransThread::evt_Data_transferred, this, &zp_net_Engine::evt_Data_transferred);
            disconnect(transThread, &zp_netTransThread::evt_NewClientConnected, this, &zp_net_Engine::evt_NewClientConnected);
            disconnect(transThread, &zp_netTransThread::evt_ClientEncrypted, this, &zp_net_Engine::evt_ClientEncrypted);
            disconnect(transThread, &zp_netTransThread::evt_SocketError, this, &zp_net_Engine::evt_SocketError);
            disconnect(transThread, &zp_netTransThread::evt_Message, this, &zp_net_Engine::evt_Message);
            disconnect(this, &zp_net_Engine::evt_EstablishConnection, transThread, &zp_netTransThread::incomingConnection);
            disconnect(this, &zp_net_Engine::evt_FireConnection, transThread, &zp_netTransThread::startConnection);
            disconnect(this, &zp_net_Engine::evt_SendDataToClient, transThread, &zp_netTransThread::sendDataToClient);
            disconnect(this, &zp_net_Engine::evt_KickAll, transThread, &zp_netTransThread::kickAllClients);
            disconnect(this, &zp_net_Engine::evt_DeactivteImmediately, transThread, &zp_netTransThread::deactivateImmediately);
            disconnect(this, &zp_net_Engine::evt_KickClient, transThread, &zp_netTransThread::kickClient);
            m_vec_netInternalTransThreads[index]->quit();
            m_vec_netInternalTransThreads[index]->wait();
            m_vec_NetTransThreads[index]->empty_RubbishCan();
            m_vec_netInternalTransThreads[index]->deleteLater();
            m_vec_NetTransThreads[index]->deleteLater();
            m_vec_netInternalTransThreads.remove(index);
            m_vec_NetTransThreads.remove(index);
		}

		return true;
	}

	/**
	 * @brief Kick all clients from server
	 * This Method is designed as an "Easy to compromise" method, which means this
	 * approach only "suggests" all these trans threads, that client should be kicked later.
     * @fn zp_net_Engine::kickAllClients
	 */
    void zp_net_Engine::kickAllClients()
	{
		//m_mutex_trans.lock();
        int size = m_vec_NetTransThreads.size();
        for (int i = 0; i < size; i++)
        {
            emit evt_KickAll(m_vec_NetTransThreads[i]);
        }
		//m_mutex_trans.unlock();
	}

	/**
	 * @brief :Deactive server immediately
	 *  This Method is designed as an "Mandatory" method, which means all clients will be sooner kicked out.
     * @fn zp_net_Engine::deactiveImmediately
	 */
    void zp_net_Engine::deactiveImmediately()
	{
		//m_mutex_trans.lock();
        int size = m_vec_NetTransThreads.size();
        for (int i = 0; i < size; i++)
        {
            emit evt_DeactivteImmediately(m_vec_NetTransThreads[i]);
        }
		//m_mutex_trans.unlock();
	}

	/**
	 * @brief Remove n Trans Thread(s)
	 * A thread marked as "removed" will be terminated after its last client socket exited.
     * @fn zp_net_Engine::removeClientTransThreads
	 * @param nThreads how many threads will be marked as "removed"
	 * @param bSSL true means SSL threads, false means Plain
	 */
    void zp_net_Engine::removeClientTransThreads(int nThreads, bool bSSL)
	{
		//m_mutex_trans.lock();
        int size = m_vec_NetTransThreads.size();
        if (nThreads < 0)
        {
            nThreads = size;
        }
		int nCount = 0;
        for (int i = 0; i < size && nCount < nThreads; i++)
		{
            if (m_vec_NetTransThreads[i]->SSLConnection() == bSSL)
			{
                m_vec_NetTransThreads[i]->deactivate();
				nCount ++;
			}
		}
        //Fire transThreadDel Immediately
        this->canExit();
		//m_mutex_trans.unlock();
	}

	/**
	 * @brief This slot send some data to a client.
	 * Because a client socket is actually belongs to a transfer-thread,
     * this event will be re-fired as evt_SendDataToClient, transfer-threads will
     * @fn zp_net_Engine::sendDataToClient
	 * @param objClient The destin client
     * @param array data to be sent
	 */
    void zp_net_Engine::sendDataToClient(QObject* objClient, QByteArray array)
	{
        emit evt_SendDataToClient(objClient, array);
	}

	/**
	 * @brief This slot kick client from server.
     * @fn zp_net_Engine::kickClients
	 * @param object Client to be kicked.
	 */
    void zp_net_Engine::kickClients(QObject* object)
	{
		emit evt_KickClient(object);
	}

	/**
	 * @brief Test whether this threadpool can be safely closed.
	 * This function will return false as long as some clients is still active.
     * @fn zp_net_Engine::canExit
	 * @return bool true means can close, false mean can not close.
	 */
    bool zp_net_Engine::canExit()
	{
        bool bRes = true;
		//m_mutex_trans.lock();
        int size = m_vec_NetTransThreads.size();
        for (int i = size - 1; i >= 0 && bRes; i--)
        {
            bRes = transThreadDel(m_vec_NetTransThreads[i]);
        }
		//m_mutex_trans.unlock();
		//m_mutex_listen.lock();
		if (m_map_netListenThreads.size())
        {
            bRes = false;
        }
		//m_mutex_listen.unlock();
        return bRes;
	}

	/**
	 * @brief Positive connect to a server.
     * In p2p connection, when the other side opens a listening address,
     * this object can directly connect to the remote side.
	 * @fn zp_net_Engine::connectTo
	 * @param address the address to connect to
	 * @param nPort port to connect to
	 * @param bSSLConn if true, SSL connections will be used
	 * @return bool
	 */
    bool zp_net_Engine::connectTo(const QHostAddress& address,
                                  quint16 nPort,
                                  bool bSSLConn)
	{
        bool bRes = false;
		//m_mutex_trans.lock();
        int size = m_vec_NetTransThreads.size();
		int nMinPay = 0x7fffffff;
		int nMinIdx = -1;

        for (int i = 0; i < size && nMinPay != 0; i++)
		{
            if (!m_vec_NetTransThreads[i]->isActive() ||
                m_vec_NetTransThreads[i]->SSLConnection() != bSSLConn)
            {
				continue;
            }

            int nPat = m_vec_NetTransThreads[i]->currentClients();
            if (nPat < nMinPay)
			{
				nMinPay = nPat;
				nMinIdx = i;
			}
		}
		//remove old threads
        for (int i = 0; i < size; i++)
        {
            if (!m_vec_NetTransThreads[i]->isActive())
            {
                transThreadDel(m_vec_NetTransThreads[i]);
            }
        }

        if (nMinIdx >= 0 && nMinIdx < size)
		{
            bRes = true;
            emit evt_FireConnection(m_vec_NetTransThreads[nMinIdx], address, nPort);
		}
		else
		{
            emit evt_Message(this, "Warning>" + QString(tr("Need Trans Thread Object for clients.")));
		}
		//m_mutex_trans.unlock();
        return bRes;
	}
}

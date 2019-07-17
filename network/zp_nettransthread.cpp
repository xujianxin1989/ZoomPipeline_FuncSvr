#include "zp_nettransthread.h"
#include "ssl_config.h"
#if (ZP_WANTSSL!=0)
#include <QSslSocket>
#endif
#include <assert.h>
#include <QDebug>
#include <QCoreApplication>
#include "zp_net_threadpool.h"
#include <QMutex>
#include <qtcpsocket.h>

QMutex g_mutex_sta;
quint64 g_bytesRecieved = 0;
quint64 g_bytesSent = 0;
quint64 g_secRecieved = 0;
quint64 g_secSent = 0;

namespace ZPNetwork
{
	int zp_netTransThread::RUBBISH_CAN_SIZE = 256;

    zp_netTransThread::zp_netTransThread(zp_net_Engine* pThreadPool, int nPayLoad, QObject* parent) :
        QObject(parent),
        m_pThreadPool(pThreadPool)
	{
        m_bActivated = true;
        m_bSSLConnection = true;
		m_nPayLoad = nPayLoad;
        assert(m_nPayLoad >= 256 && m_nPayLoad <= 16777216);
	}

    void zp_netTransThread::empty_RubbishCan()
	{
        m_mutex_rubbish_can.lock();
        foreach (QObject *pObj, m_rubbish_can)
        {
            pObj->deleteLater();
        }
        m_rubbish_can.clear();
        m_set_rubbish.clear();
        m_mutex_rubbish_can.unlock();
	}

	bool zp_netTransThread::isActive() const
	{
		return m_bActivated;
	}

	bool zp_netTransThread::SSLConnection() const
	{
        return m_bSSLConnection;
	}

    void zp_netTransThread::setSSLConnection(bool bssl)
	{
		m_bSSLConnection = bssl;
	}

    void zp_netTransThread::deactivate()
	{
		m_bActivated = false;
	}

	/**
	 * @brief return a list contains all of online-clients objects.
	 *
	 * @return QList<QObject *> Online clients
	 */
    QList<QObject*> zp_netTransThread::clientsList()
	{
        QList<QObject *> list;
		m_mutex_protect.lock();
        list = m_clientList.values();
		m_mutex_protect.unlock();
        return list;
	}

	/**
	 * @brief return howmany clients holded by this zp_netTransThread
	 *
	 * @return int the clients count
	 */
    int zp_netTransThread::currentClients()
	{
		int nRes;
		m_mutex_protect.lock();
		nRes = m_clientList.size();
		m_mutex_protect.unlock();
		return nRes;
	}

	/**
	 * @brief this slot recieves evt_DeactivateImmediately message,
     * if this message is sent to this object, a "kickAllClients"
	 * method will be called.
	 *
	 * @param ptr
	 */
    void zp_netTransThread::deactivateImmediately(zp_netTransThread *ptr)
	{
        if (ptr != this)
        {
            return;
        }
		m_bActivated = false;
        this->kickAllClients(ptr);
	}

	/**
	 * @brief set the payload of this object. data large than nPayload will be
	 * cutted into pieces, each piece is nPayload bytes. the last piece may be
	 * shorter than nPayload.
	 *
	 * @param nPayload bytes a signal block contains .
	 */
    void zp_netTransThread::setPayload(int nPayload)
	{
		m_nPayLoad = nPayload;
        assert(m_nPayLoad >= 256 && m_nPayLoad <= 16 * 1024 * 1024);
	}

	/**
	 * @brief the multithread object life-cycle is very complex, we hold a rabish_can,
	 * to prevent the misuse of deleted object in different threads.
	 * @param delObj The object to be deleted later
	 */
    void zp_netTransThread::push_to_rubbish_can(QObject *delObj)
	{
        m_mutex_rubbish_can.lock();
        if (!m_set_rubbish.contains(delObj))
		{
            m_rubbish_can.push_back(delObj);
            m_set_rubbish.insert(delObj);
		}
        if (RUBBISH_CAN_SIZE < 16)
        {
            RUBBISH_CAN_SIZE = 16;
        }
		if (RUBBISH_CAN_SIZE > 65536)
        {
            RUBBISH_CAN_SIZE = 65536;
        }
        if (m_rubbish_can.size() >= RUBBISH_CAN_SIZE)
        {
            qDebug() << "Delete old objects from rubbish can.";
        }
        while (m_rubbish_can.size() >= RUBBISH_CAN_SIZE)
		{
            m_rubbish_can.first()->deleteLater();
            m_set_rubbish.remove(m_rubbish_can.first());
            m_rubbish_can.pop_front();
		}
        m_mutex_rubbish_can.unlock();
	}

	/**
	 * @brief This slot dealing with multi-thread client socket accept.
	 * accepy works start from zp_netListenThread::m_tcpserver, end with this method.
	 * the socketDescriptor is delivered from zp_netListenThread(a Listening thread)
     * to zp_net_Engine(Normally in main-gui thread), and then zp_netTransThread.
	 *
     * @param threadId if threadId is not equal to this object, this message is just omitted.
	 * @param socketDescriptor socketDescriptor for incomming client.
	 */
    void zp_netTransThread::incomingConnection(QObject* threadId, qintptr socketDescriptor)
	{
        //创建的套接字直接属于特定的传输线程
        if (threadId != this)
        {
            return;
        }
        QTcpSocket *sock_client;
#if (ZP_WANTSSL!=0)
		if (m_bSSLConnection)
        {
            sock_client = new QSslSocket(this);
        }
		else
        {
            sock_client = new QTcpSocket(this);
        }
#else
        sock_client = new QTcpSocket(this);
#endif
        if (!sock_client)
        {
            return;
        }
        //Initial content
        if (!sock_client->setSocketDescriptor(socketDescriptor))
        {
            push_to_rubbish_can(sock_client);
            return;
        }
        connect(sock_client, &QTcpSocket::readyRead, this, &zp_netTransThread::new_data_recieved, Qt::QueuedConnection);
        connect(sock_client, &QTcpSocket::disconnected, this, &zp_netTransThread::client_closed, Qt::QueuedConnection);
        connect(sock_client, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)), Qt::QueuedConnection);
        connect(sock_client, &QTcpSocket::bytesWritten, this, &zp_netTransThread::some_data_sended, Qt::QueuedConnection);

        m_mutex_protect.lock();
        m_clientList.insert(sock_client);
        m_mutex_protect.unlock();
#if (ZP_WANTSSL!=0)
        if (m_bSSLConnection)
        {
            QSslSocket* psslsock = qobject_cast<QSslSocket*>(sock_client);
            assert(psslsock != NULL);
            QString strCerPath = QCoreApplication::applicationDirPath() + "/svr_cert.pem";
            QString strPkPath = QCoreApplication::applicationDirPath() + "/svr_privkey.pem";
            psslsock->setLocalCertificate(strCerPath);
            psslsock->setPrivateKey(strPkPath);
            connect(psslsock, &QSslSocket::encrypted, this, &zp_netTransThread::on_encrypted, Qt::QueuedConnection);
            psslsock->startServerEncryption();
        }
#endif
        emit evt_NewClientConnected(sock_client);
	}

	/**
	 * @brief This method will create a socket, and directly connect to destion address.
	 *
     * @param threadId if threadId is not equal to this object, this message is just omitted.
     * @param address address to which the socket should be connected.
	 * @param port port to which the socket should be connected.
	 */
    void zp_netTransThread::startConnection(QObject* threadId,
                                            const QHostAddress& address,
                                            quint16 port)
	{
        if (threadId != this)
        {
            return;
        }
        QTcpSocket *sock_client;
#if (ZP_WANTSSL!=0)
		if (m_bSSLConnection)
        {
            sock_client = new QSslSocket(this);
        }
		else
        {
            sock_client = new QTcpSocket(this);
        }
#else
        sock_client = new QTcpSocket(this);
#endif
        if (!sock_client)
        {
            return;
        }
#if (ZP_WANTSSL!=0)
        if (m_bSSLConnection)
        {
            QSslSocket* psslsock = qobject_cast<QSslSocket*>(sock_client);
            assert(psslsock != NULL);
            QString strCerPath = QCoreApplication::applicationDirPath() + "/ca_cert.pem";
            QList<QSslCertificate> listCas = QSslCertificate::fromPath(strCerPath);
            psslsock->setCaCertificates(listCas);

            connect(sock_client, &QTcpSocket::readyRead, this, &zp_netTransThread::new_data_recieved, Qt::QueuedConnection);
            connect(sock_client, &QTcpSocket::disconnected, this, &zp_netTransThread::client_closed, Qt::QueuedConnection);
            connect(sock_client, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)), Qt::QueuedConnection);
            connect(sock_client, &QTcpSocket::bytesWritten, this, &zp_netTransThread::some_data_sended, Qt::QueuedConnection);
            connect(sock_client, &QTcpSocket::connected, this, &zp_netTransThread::on_connected, Qt::QueuedConnection);
            connect(psslsock, &QSslSocket::encrypted, this, &zp_netTransThread::on_encrypted, Qt::QueuedConnection);

            m_mutex_protect.lock();
            m_clientList.insert(sock_client);
            m_mutex_protect.unlock();

            psslsock->connectToHostEncrypted(address.toString(), port);
        }
        else
        {
            connect(sock_client, &QTcpSocket::connected, this, &zp_netTransThread::on_connected, Qt::QueuedConnection);
            connect(sock_client, &QTcpSocket::disconnected, this, &zp_netTransThread::client_closed, Qt::QueuedConnection);
            connect(sock_client, &QTcpSocket::readyRead, this, &zp_netTransThread::new_data_recieved, Qt::QueuedConnection);
            connect(sock_client, &QTcpSocket::bytesWritten, this, &zp_netTransThread::some_data_sended, Qt::QueuedConnection);
            connect(sock_client, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)), Qt::QueuedConnection);

            m_mutex_protect.lock();
            m_clientList.insert(sock_client);
            m_mutex_protect.unlock();

            sock_client->connectToHost(address, port);
        }
#else
        connect(sock_client, &QTcpSocket::connected, this, &zp_netTransThread::on_connected, Qt::QueuedConnection);
        connect(sock_client, &QTcpSocket::disconnected, this, &zp_netTransThread::client_closed, Qt::QueuedConnection);
        connect(sock_client, &QTcpSocket::readyRead, this, &zp_netTransThread::new_data_recieved, Qt::QueuedConnection);
        connect(sock_client, &QTcpSocket::bytesWritten, this, &zp_netTransThread::some_data_sended, Qt::QueuedConnection);
        connect(sock_client, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)), Qt::QueuedConnection);

        m_mutex_protect.lock();
        m_clientList.insert(sock_client);
        m_mutex_protect.unlock();

        sock_client->connectToHost(address, port);
#endif
	}

	void zp_netTransThread::on_connected()
	{
        QTcpSocket *sock_client = qobject_cast<QTcpSocket *>(sender());
        if (!sock_client)
        {
            return;
        }
        emit evt_NewClientConnected(sock_client);
        emit evt_Message(sock_client, "Info>" +  QString(tr("Client connected.")));
	}

	void zp_netTransThread::on_encrypted()
	{
        QTcpSocket *pSocket = qobject_cast<QTcpSocket *>(sender());
        if (!pSocket)
        {
            return;
        }
        emit evt_ClientEncrypted(pSocket);
    }

    void zp_netTransThread::on_hostFound()
    {
        QTcpSocket* pSocket = qobject_cast<QTcpSocket*>(sender());
        if (!pSocket)
        {
            return;
        }
        emit evt_Message(pSocket, "Info>" + QString(tr("Host founded.")));
    }

	void zp_netTransThread::client_closed()
	{
        QTcpSocket* pSocket = qobject_cast<QTcpSocket*>(sender());
        if (!pSocket)
        {
            return;
        }
#if (ZP_WANTSSL!=0)
        if (m_bSSLConnection)
        {
            QSslSocket* psslsock = qobject_cast<QSslSocket*>(pSocket);
            if (psslsock)
            {
                disconnect(psslsock, &QSslSocket::encrypted, this, &zp_netTransThread::on_encrypted);
            }
        }
#endif
        disconnect(pSocket, &QTcpSocket::readyRead, this, &zp_netTransThread::new_data_recieved);
        disconnect(pSocket, &QTcpSocket::disconnected, this, &zp_netTransThread::client_closed);
        disconnect(pSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)));
        disconnect(pSocket, &QTcpSocket::bytesWritten, this, &zp_netTransThread::some_data_sended);
        disconnect(pSocket, &QTcpSocket::connected, this, &zp_netTransThread::on_connected);

        m_buffer_sending.erase(pSocket);
        m_buffer_sending_offset.erase(pSocket);

        m_mutex_protect.lock();
        m_clientList.remove(pSocket);
        m_mutex_protect.unlock();

        //pSocket->abort();
        emit evt_ClientDisconnected(pSocket);
        emit evt_Message(pSocket, "Info>" + QString(tr("Client Closed.")));
        push_to_rubbish_can(pSocket);
	}

	void zp_netTransThread::new_data_recieved()
	{
        QTcpSocket *pSocket = qobject_cast<QTcpSocket *>(sender());
        if (!pSocket)
        {
            return;
        }
        QByteArray array = pSocket->readAll();
        int size = array.size();
        g_mutex_sta.lock();
        g_bytesRecieved += size;
        g_secRecieved += size;
        g_mutex_sta.unlock();

        //当套接字收到数据后, 简单的触发 evt_Data_recieved 事件
        emit evt_Data_recieved(pSocket, array);
	}

	/**
	 * @brief this slot will be called when internal socket successfully
	 * sent some data. in this method, zp_netTransThread object will check
	 * the sending-queue, and send more data to buffer.
	 *
	 * @param wsended
	 */
    void zp_netTransThread::some_data_sended(qint64 wsended)
	{
		g_mutex_sta.lock();
        g_bytesSent += wsended;
		g_secSent += wsended;
		g_mutex_sta.unlock();

        QTcpSocket* pSocket = qobject_cast<QTcpSocket*>(sender());
        if (!pSocket)
        {
            return;
        }
        emit evt_Data_transferred(pSocket, wsended);

        QList<QByteArray>& list_sock_data = m_buffer_sending[pSocket];
        QList<qint64>& list_offset = m_buffer_sending_offset[pSocket];
        while (!list_sock_data.empty())
        {
            QByteArray& arraySending = *list_sock_data.begin();
            qint64& currentOffset = *list_offset.begin();

            qint64 totalBytes = arraySending.size();
            assert(totalBytes >= currentOffset);

            qint64 nBytesWritten = pSocket->write(arraySending.constData() + currentOffset,
                                                  qMin((int)(totalBytes - currentOffset), m_nPayLoad));
            currentOffset += nBytesWritten;
            if (currentOffset < totalBytes)
            {
                break;
            }
            list_sock_data.pop_front();
            list_offset.pop_front();
        }
	}

	void zp_netTransThread::displayError(QAbstractSocket::SocketError socketError)
	{
        QTcpSocket *pSocket = qobject_cast<QTcpSocket *>(sender());
        if (!pSocket)
        {
            return;
        }
        emit evt_SocketError(pSocket, socketError);
#if (ZP_WANTSSL!=0)
        if (m_bSSLConnection)
        {
            QSslSocket *psslsock = qobject_cast<QSslSocket *>(pSocket);
            if (psslsock)
            {
                disconnect(psslsock, &QSslSocket::encrypted, this, &zp_netTransThread::on_encrypted);
            }
        }
#endif
        disconnect(pSocket, &QTcpSocket::readyRead, this, &zp_netTransThread::new_data_recieved);
        disconnect(pSocket, &QTcpSocket::disconnected, this, &zp_netTransThread::client_closed);
        disconnect(pSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)));
        disconnect(pSocket, &QTcpSocket::bytesWritten, this, &zp_netTransThread::some_data_sended);
        disconnect(pSocket, &QTcpSocket::connected, this, &zp_netTransThread::on_connected);
        m_buffer_sending.erase(pSocket);
        m_buffer_sending_offset.erase(pSocket);

        m_mutex_protect.lock();
        m_clientList.remove(pSocket);
        m_mutex_protect.unlock();

        pSocket->abort();
        emit evt_ClientDisconnected(pSocket);
        emit evt_Message(pSocket, "Info>" + QString(tr("Client Error, Closed.")));
        push_to_rubbish_can(pSocket);
	}

    void zp_netTransThread::sendDataToClient(QObject *objClient, QByteArray array)
	{
		m_mutex_protect.lock();
        if (!m_clientList.contains(objClient))
		{
			m_mutex_protect.unlock();
			return;
		}
		m_mutex_protect.unlock();

        QTcpSocket *pSocket = qobject_cast<QTcpSocket *>(objClient);
        if (!pSocket)
        {
            return;
        }
        if (array.isEmpty())
        {
            return;
        }
        //检查队列是否为空, 为空的话, 将触发 QTcpSocket::write 方法并发出 m_nPayload(最多) 大小的数据块
        QList<QByteArray> &list_sock_data = m_buffer_sending[pSocket];
        QList<qint64> &list_offset = m_buffer_sending_offset[pSocket];
        if (list_sock_data.empty())
        {
            qint64 totalBytes = array.size();
            qint64 bytesWritten = pSocket->write(array.constData(), qMin((int)totalBytes, m_nPayLoad));
            if (bytesWritten >= totalBytes)
            {
                return;
            }

            list_sock_data.push_back(array);
            list_offset.push_back(bytesWritten);
			return;
        }
		list_sock_data.push_back(array);
		list_offset.push_back(0);
	}

    void zp_netTransThread::kickAllClients(zp_netTransThread *ptr)
	{
        if (this != ptr)
        {
            return;
        }
		m_mutex_protect.lock();
        QList<QObject *> clientList = m_clientList.values();
		m_mutex_protect.unlock();

        foreach(QObject *obj, clientList)
		{
            QTcpSocket *pSocket = qobject_cast<QTcpSocket *>(obj);
            if (pSocket)
			{
                pSocket->disconnectFromHost();
			}
		}
	}

    void zp_netTransThread::kickClient(QObject *objClient)
	{
		m_mutex_protect.lock();
        if (!m_clientList.contains(objClient))
		{
			m_mutex_protect.unlock();
			return;
		}
		m_mutex_protect.unlock();
        QTcpSocket *pSocket = qobject_cast<QTcpSocket *>(objClient);
        if (pSocket)
		{
            pSocket->disconnectFromHost();
        }
    }

    bool zp_netTransThread::canExit()
	{
        if (m_bActivated)
        {
            return false;
        }
        if (currentClients())
        {
            return false;
        }
		return true;
	}
}

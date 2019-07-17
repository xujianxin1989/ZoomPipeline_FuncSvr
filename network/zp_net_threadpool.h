/**
  * @brief This is the Zoom-Pipeline Network engine by goldenhawking, 2013,2014
  */
#ifndef ZP_NET_THREADPOOL_H
#define ZP_NET_THREADPOOL_H

#include <QObject>
#include <QThread>
#include <QMap>
#include <QVector>
#include <QMutex>
#include <QThread>
#include <QSslCertificate>
#include <QSslKey>
#include <QFile>
#include "zp_netlistenthread.h"
#include "zp_nettransthread.h"

namespace ZPNetwork
{
	/**
      * @brief zp_net_Engine provides a multi-thread listening\transfer
      * network framework. SSL and Plain sockets are both supported.
      * 派生自 QObject。模块的外部接口类，同时也是功能管理者。
      * 提供了设置监听器、配置线程池的功能。
	  */
	class zp_net_Engine : public QObject
	{
		Q_OBJECT

	public:
        explicit zp_net_Engine(int nPayLoad = 4096, QObject* parent = 0);

        //Begin a listening socket at special address and port. The socket will be activated as soon as possible
        void addListeningAddress(QString id, const QHostAddress& address, quint16 nPort, bool bSSLConn = true);
		//Remove a listening socket at special address and port.The socket will be deactivated as soon as possible
        void removeListeningAddress(QString id);
		//Remove all listening sockets
        void removeAllAddresses();

		//Trans Control
		//Add n client-Trans Thread(s).
        void addClientTransThreads(int nThreads, bool bSSL = true);
		//Remove n client-Trans Thread(s).a thread marked reomved will be terminated after its last client socket exited.
        void removeClientTransThreads(int nThreads, bool bSSL = true);
		//Kick All Clients
        void kickAllClients();
		//Deactive Immediately
        void deactiveImmediately();
		//when Program exit, wait for close;
        bool canExit();
		//The status
        QStringList listenerNames() const;
        int transThreadNum() const;
        int transThreadNum(bool bSSL);
		int totalClients(int idxThread);

	protected:
		int m_nPayLoad;
        //存储了各个端口的线程对象
        QMap<QString, zp_netListenThread*> m_map_netListenThreads;
        //存储了各个端口的线程
        QMap<QString, QThread*> m_map_netInternalListenThreads;
		//This map stores ClientTransThreadObjects
        QVector<zp_netTransThread*> m_vec_NetTransThreads;
		//Internal Threads to hold each m_map_NetTransThreads' message Queue
        QVector<QThread*> m_vec_netInternalTransThreads;
        bool transThreadDel(zp_netTransThread* pThreadObj);

	signals:
        void evt_Message(QObject* pSource, QString);
		//The socket error message
        void evt_SocketError(QObject* senderSock, QAbstractSocket::SocketError socketError);
		//this event indicates new client connected.
        void evt_NewClientConnected(QObject* /*clientHandle*/);
		//SSL Connections OK
        void evt_ClientEncrypted(QObject* client);
		//this event indicates a client disconnected.
        void evt_ClientDisconnected(QObject* /*clientHandle*/);
		//some data arrival
        void evt_Data_recieved(QObject* /*clientHandle*/, QByteArray /*datablock*/);
		//a block of data has been successfuly sent
        void evt_Data_transferred(QObject* /*clientHandle*/, qint64 /*bytes sent*/);

		//Internal Message for ctrl.------------------------------------
		//Listen Control
        void startListen(QString id);
        void stopListen(QString id);
        void evt_EstablishConnection(QObject* threadId, qintptr socketDescriptor);
        void evt_FireConnection(QObject* threadId, const QHostAddress& address, quint16 port);
        //Trans Control, for intenal thread usage
        void evt_SendDataToClient(QObject* objClient, QByteArray array);
        void evt_KickClient(QObject*);
        void evt_DeactivteImmediately(zp_netTransThread*);
        void evt_KickAll(zp_netTransThread*);

	protected slots:
		void on_New_Arrived_Client(qintptr socketDescriptor);
        void on_ListenClosed(QString id);

	public slots:
		//Call this function to send data to client
        void sendDataToClient(QObject* objClient, QByteArray array);
		//Close client Immediatele
        void kickClients(QObject* object);
		//Possive Connection Methods
        bool connectTo(const QHostAddress& address, quint16 nPort, bool bSSLConn);
	};
}
#endif // ZP_NET_THREADPOOL_H

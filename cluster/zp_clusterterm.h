#ifndef ZP_CLUSTERTERM_H
#define ZP_CLUSTERTERM_H

#include <QObject>
#include "../network/zp_net_threadpool.h"
#include "../pipeline/zp_pipeline.h"
#include <functional>

namespace ZP_Cluster
{
	class zp_ClusterNode;
	//!this class enable server processes can
	//!communicate with each other.
	class zp_ClusterTerm : public QObject
	{
		Q_OBJECT

	public:
		explicit zp_ClusterTerm(QString termName, QObject *parent = nullptr);

		/**
		 * The factory enables user-defined sub-classes inherits from zp_ClusterNode
		 * Using setNodeFactory, set your own allocate method.
		 */
		void setNodeFactory(std::function<zp_ClusterNode* (zp_ClusterTerm * /*pTerm*/,
			QObject * /*pSocket*/,
			QObject * /*parent*/)>factory);

		//cluster status
		ZPNetwork::zp_net_Engine *netEng() const;
		ZPTaskEngine::zp_pipeline *taskEng() const;
		bool canExit() const;

		//Server Group Mapping
		zp_ClusterNode *svrNodeFromName(QString svrName);
		zp_ClusterNode *svrNodeFromSocket(QObject *socket);
		QStringList svrNames();
		quint32 remoteClientNums(QString svrName);

		QHostAddress svrLANAddr(QString svrName);
		int svrLANPort(QString svrName);
		QHostAddress svrPubAddr(QString svrName);
		int svrPubPort(QString svrName);

		//Client Num set, for cross-svr balance
		void setClientNums(quint32 num);
		quint32 clientNums() const;
		//Propties
	public:
		//properties.
		QString setTermName(QString termName);
		QString termName() const;

		//LAN Address, for other servers
		QHostAddress LANAddr() const;
		int LANPort() const;
		QHostAddress setLANAddr(QHostAddress addr);
		int setLANPort(int port);

		//Publish Address, for clients.
		QHostAddress PublishAddr() const;
		int PublishPort() const;
		QHostAddress setPublishAddr(QHostAddress addr);
		int setPublishPort(int port);

		//Re-Direct Test.
		QString minPayloadServer(quint8 bufAddresses[/*64*/], quint16 *pnPort);

		int heartBeatingThrdHold() const;
		void setHeartBeatingThrd(const int n);

		bool registNewServer(zp_ClusterNode *pNode);
		void broadcastServers();
		void sendHeartBeatings();

	protected:
		std::function<zp_ClusterNode * (
			zp_ClusterTerm * /*pTerm*/,
			QObject * /*pSocket*/,
			QObject * /*parent*/)> m_factory;
		zp_ClusterNode *default_factory(
			zp_ClusterTerm * /*pTerm*/,
			QObject * /*pSocket*/,
			QObject * /*parent*/);

		int m_nHeartBeatingTime;

		quint32 m_nClientNums;  //the clients this server now connected.
		QString m_strTermName;  //the Terminal's name

		QHostAddress m_addrLAN; //The LAN address for other server-terms to connect to
		int m_nPortLAN;         //The LAN port for other server-terms to connect to
		QHostAddress m_addrPub;	//The Publish address for clients to connect to
		int m_nPortPub;         //The Publish port for clients to connect to

		ZPNetwork::zp_net_Engine* m_pClusterNet;
		ZPTaskEngine::zp_pipeline* m_pClusterEng;
		//This list hold dead nodes that still in task queue, avoiding crash
		QList<zp_ClusterNode*> m_nodeToBeDel;
		//important hashes. server name to socket, socket to server name
		QMutex m_hash_mutex;

		//名称到对象的映射
		QMap<QString, zp_ClusterNode *> m_hash_name2node;
		//套接字到对象的映射
		QMap<QObject*, zp_ClusterNode *> m_hash_sock2node;

	signals:
		void evt_Message(QObject *, QString);
		//The socket error message
		void evt_SocketError(QObject *senderSock, QAbstractSocket::SocketError socketError);
		//this event indicates new svr successfully hand-shaked.
		void evt_NewSvrConnected(QString svrName);
		//this event indicates a client disconnected.
		void evt_NewSvrDisconnected(QString /*svrName*/);
		//some data arrival
		void evt_RemoteData_recieved(QString /*svrName*/, QByteArray /*array*/);
		//a block of data has been successfuly sent
		void evt_RemoteData_transferred(QObject * /*svrName*/, qint64 /*bytes sent*/);

	protected slots:
		//this event indicates new client connected.
		void on_evt_NewClientConnected(QObject *clientHandle);
		//this event indicates new client encrypted.
		void on_evt_ClientEncrypted(QObject *clientHandle);
		//this event indicates a client disconnected.
		void on_evt_ClientDisconnected(QObject *clientHandle);
		//some data arrival
		void on_evt_Data_recieved(QObject *clientHandle, QByteArray datablock);
		//a block of data has been successfuly sent
		void on_evt_Data_transferred(QObject *clientHandle, qint64 bytesSent);

	public slots:
		//!Start listen, this term can be connected by newly joined terms in future.
		void startListen(const QHostAddress &address, int nPort);
		//!Join cluster, using existing term (address:nPort)
		//!as soon as connection established, more existing terms will be sent to this term,
		//!an p2p connection will start
		bool joinCluster(const QHostAddress &address, int nPort, bool bSSL = false);
		void kickDeadClients();

		//Send Data to Server
		void sendDataToRemoteServer(QString svrName, QByteArray sourceArray);
		//Send Data to WebService
		void sendDataToWebService(QString svrName, QByteArray sourceArray);
	};
}
#endif // ZP_CLUSTERTERM_H

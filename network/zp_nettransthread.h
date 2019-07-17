/**
 * @brief class zp_netTransThread by goldenhawking, 2013,2014
 */
#ifndef ZP_NETTRANSTHREAD_H
#define ZP_NETTRANSTHREAD_H

#include <QObject>
#include <QAbstractSocket>
#include <QMutex>
#include <unordered_map>
#include <QSet>
#include <QList>

namespace ZPNetwork
{
	class zp_net_Engine;
	/**
	 * @brief class zp_netTransThread is a QObject-derived object class.
	 * Objects of this type will be binded to a QThread, in which TCP transfer
	 * will work
	 */
	class zp_netTransThread : public QObject
	{
		Q_OBJECT

	public:
        explicit zp_netTransThread(zp_net_Engine* pThreadPool,
                                   int nPayLoad = 4096,
                                   QObject* parent = 0);

        //一个 zp_netTransThread 线程可以承担多个客户端的收发请求
        QList<QObject*> clientsList();
        int currentClients();
        void setPayload(int nPayload);
		bool isActive() const;

        bool canExit();
		bool SSLConnection() const;
        void setSSLConnection(bool bssl);

		//RubbishCan Functions
        void empty_RubbishCan();
		//Size of the RubbishCan
		static int RUBBISH_CAN_SIZE;

	private:
		bool m_bActivated;
		bool m_bSSLConnection;
        int m_nPayLoad;

		//sending buffer, hold byteArraies.
        std::unordered_map<QObject *, QList<QByteArray>> m_buffer_sending;
        std::unordered_map<QObject *, QList<qint64>> m_buffer_sending_offset;

		//The socket and the connection-direction, 0 is passive, 1 is postive.
		QSet<QObject *> m_clientList;

		QMutex m_mutex_protect;
        zp_net_Engine *m_pThreadPool;
		//Rabish Can
        QList<QObject *> m_rubbish_can;
        QSet<QObject *> m_set_rubbish;
        QMutex m_mutex_rubbish_can;

        void push_to_rubbish_can(QObject* delObj);

	public slots:
		//This slot dealing with multi-thread client socket accept.
        void incomingConnection(QObject *threadId, qintptr socketDescriptor);
		//This slot dealing with possive connecting-to method.
        void startConnection(QObject *threadId, const QHostAddress &address, quint16 port);
        //sending array to objClient. array will be pushed into m_buffer_sending
        void sendDataToClient(QObject *objClient, QByteArray array);
		//Set terminate mark, the thread will quit after last client quit.
        void deactivate();
		//terminate this thread immediately
        void deactivateImmediately(zp_netTransThread *);
        //Kick all clients.
        void kickAllClients(zp_netTransThread *ptr);
		//Kick client.
        void kickClient(QObject*);

	protected slots:
		//when client closed, this slot will be activated.
		void client_closed();
		void new_data_recieved();
		void some_data_sended(qint64);
		void displayError(QAbstractSocket::SocketError socketError);
		//Plain Connected
        void on_connected();
        void on_hostFound();
		//SSL Encrypted started
		void on_encrypted();

	signals:
        void evt_Message(QObject *pSource, QString);
        void evt_SocketError(QObject *senderSock, QAbstractSocket::SocketError socketError);
        void evt_NewClientConnected(QObject *client);
        void evt_ClientEncrypted(QObject *client);
        void evt_ClientDisconnected(QObject *client);
        void evt_Data_recieved(QObject *, QByteArray);
        void evt_Data_transferred(QObject *client, qint64);
	};
}
#endif // ZP_NETTRANSTHREAD_H

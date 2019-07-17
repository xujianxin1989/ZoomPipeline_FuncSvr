#ifndef ZP_TcpServer_H
#define ZP_TcpServer_H

#include <QTcpServer>

namespace ZPNetwork
{
	class ZP_TcpServer : public QTcpServer
	{
		Q_OBJECT

	public:
        ZP_TcpServer(QObject *parent);

	protected:
        //重载了ZP_TcpServer::incomingConnection，不在监听线程中进行Accept操作，
        //而是直接发出evt_NewClientArrived信号，把套接字描述符(socketdescriptor)泵出，
        //由zp_net_Engine类进行负荷均衡，选取当前负荷最小的传输线程(zp_netTransThread)接受该接入申请。
		void incomingConnection(qintptr socketDescriptor) override;

	signals:
		void evt_NewClientArrived(qintptr socketDescriptor);
	};
}

#endif // ZP_TcpServer_H

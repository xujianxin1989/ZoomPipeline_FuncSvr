#ifndef ST_CROSS_SVR_NODE_H
#define ST_CROSS_SVR_NODE_H

#include "../cluster/zp_clusternode.h"
#include "st_cross_svr_msg.h"

namespace ExampleServer
{
	class st_client_table;
	class st_cross_svr_node : public ZP_Cluster::zp_ClusterNode
	{
		Q_OBJECT

	public:
        st_cross_svr_node(ZP_Cluster::zp_ClusterTerm *pTerm,
                          QObject *pSocket,
                          QObject *parent);

        void setClientTable(st_client_table *clientTable);

    protected:
        int st_bytesLeft();
		//!virtual functions, dealing with the user-defined operations.
        virtual bool deal_user_data(QByteArray array);
		bool deal_msg();

	protected:
        st_client_table *m_pClientTable;
		//Current Message Offset, according to m_currentHeader
        quint32 m_curStMegSize;
        //Current un-procssed message block. for large blocks,
        //this array will be re-setted as soon as some part of data has been dealed,
        //eg, send a 200MB block, the 200MB data will be splitted into pieces
        QByteArray m_curStBlock;
        //current header
		EXAMPLE_CROSSSVR_MSG::tag_msgHearder m_st_Header;
        //destin uuid
		quint32 m_destin_uuid;

	signals:
        void evt_SendToNode(quint32 uuid, QByteArray array);
	};
}
#endif // ST_CROSS_SVR_NODE_H

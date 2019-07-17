#ifndef ST_CLIENT_TABLE_H
#define ST_CLIENT_TABLE_H

#include <QObject>
#include <unordered_map>
#include "../network/zp_net_threadpool.h"
#include "../pipeline/zp_pipeline.h"
#include "./st_message.h"
#include "../database/databaseresource.h"
#include "../cluster/zp_clusterterm.h"
#include "./st_cross_svr_node.h"

namespace ExampleServer
{
    class st_clientNode_baseTrans;

    //封装所有的服务功能。这个类在每个服务器进程中仅有一个实例
    class st_client_table : public QObject
    {
        Q_OBJECT

    public:
        explicit st_client_table(ZPNetwork::zp_net_Engine *netEngine,
                                 ZPTaskEngine::zp_pipeline *taskEngine,
                                 ZPDatabase::DatabaseResource *pDb,
                                 ZP_Cluster::zp_ClusterTerm *pCluster,
                                 QObject *parent = nullptr);
        ~st_client_table();

        bool registClientUUID(st_clientNode_baseTrans *clientNode);
        st_clientNode_baseTrans *clientNodeFromUUID(quint32 uuid);
        st_clientNode_baseTrans *clientNodeFromSocket(QObject *socket);

        //Heart beating and healthy
        void kickDeadClients();
        int heartBeatingThrd();
        void setHeartBeatingThrd(int h);

        //Database and disk resources
        QString database_UserAcct();
        void setDatabase_UserAcct(QString s);
        QString database_Event();
        void setDatabase_Event(QString s);
        QString largeFileFolder();
        void setLargeFileFolder(QString s);

        ZPDatabase::DatabaseResource *dbRes();
        ZP_Cluster::zp_ClusterTerm *clusterTerm();

        //reg new uuids in m_hash_remoteClient2SvrName
        void cross_svr_add_uuids(QString svrName, quint32 *pUUIDs, int nUUIDs);
        //del uuids in m_hash_remoteClient2SvrName, pUUIDs = 0 means del all uuids belong to svrName
        void cross_svr_del_uuids(QString svrName, quint32 *pUUIDs, int nUUIDs);
        //tell remote servers of uuid-change
        void broadcast_client_uuid(quint32 uuid, bool bActive);
        //trans user Data
        void cross_svr_send_data(QString svrName, QByteArray ary);
        //server name find
        QString cross_svr_find_uuid(quint32 uuid);
        void cross_web_send_data(QString svrName, QByteArray ary);

        //cluster global balance
        bool needRedirect(quint8 bufAddresses[/*64*/], quint16 *pnPort);

        void setBalanceMax(int nMax);
        int balanceMax();

    protected:
        //This list hold dead nodes that still in task queue,avoiding crash
        QList<st_clientNode_baseTrans*> m_nodeToBeDel;

        //Very important hashes. will be improved for cross-server transfer
        //本地客户端 Hash 成员
        QMutex m_hash_mutex;
        //UUID:用户的全局标示
        //从用户UUID到用户节点的映射
        std::unordered_map<quint32, st_clientNode_baseTrans*> m_hash_uuid2node;
        //从套接字到用户节点的映射
        std::unordered_map<QObject*, st_clientNode_baseTrans*> m_hash_sock2node;

        //Concurrent Network frame work
        ZPNetwork::zp_net_Engine *m_pThreadEngine;
        //The piple-line
        ZPTaskEngine::zp_pipeline *m_pTaskEngine;
        //The database pool
        ZPDatabase::DatabaseResource *m_pDatabaseRes;
        //The Server Cluster Group
        ZP_Cluster::zp_ClusterTerm *m_pCluster;

        //The max seconds before dead client be kicked out
        int m_nHeartBeatingDeadThrd;
        //Database Resource Names used by nodes
        QString m_strDBName_useraccount;
        QString m_strDBName_event;
        QString m_largeFileFolder;

        //Cluster max balance value, when clients exceeds this val, balance will happen.
        int m_nBalanceMax;

        //cluster Nodes Map
        //从客户端的 UUID 到 集群服务器名称的映射
        std::unordered_map<quint32, QString> m_hash_remoteClient2SvrName;
        QMutex m_mutex_cross_svr_map;
        //Cluster Node Factory
        ZP_Cluster::zp_ClusterNode* cross_svr_node_factory(ZP_Cluster::zp_ClusterTerm * /*pTerm*/,
                                                           QObject * /*pSocket*/,
                                                           QObject * /*parent*/);

    signals:
        void evt_Message(QObject *pSource, QString);

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
        void on_evt_Data_transferred(QObject * /*clientHandle*/, qint64 /*bytes sent*/);

        //this event indicates new svr successfully hand-shaked.
        void on_evt_NewSvrConnected(QString svrName);
        //this event indicates a client disconnected.
        void on_evt_NewSvrDisconnected(QString svrName);
        //some data arrival
        void on_evt_RemoteData_recieved(QString svrName, QByteArray array);
        //a block of data has been successfuly sent
        void on_evt_RemoteData_transferred(QObject * /*svrName*/, qint64 /*bytes sent*/);

    public slots:
        //send msg to uuid
        bool sendToNode(quint32 uuid, QByteArray array);
    };
}
#endif // ST_CLIENT_TABLE_H

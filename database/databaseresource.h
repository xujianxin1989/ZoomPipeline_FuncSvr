﻿#ifndef DATABASERESOURCE_H
#define DATABASERESOURCE_H

#include <QObject>
#include <QMap>
#include <QSqlDatabase>
#include <QString>
#include <QMutex>
#include <QThread>
#include <QSet>
#include <QDateTime>

namespace ZPDatabase
{
	/**
	 * @brief this class provide an database reource pool.In different thread, workers can get existing db connections
     * immediately without re-creation operations. The working thread does not maintain db connections, instead of
	 * maintaining, it just using db resources from DatabaseResource
	 * @class DatabaseResource databaseresource.h "ZoomPipeline_FuncSvr/database/databaseresource.h"
	 */
	class DatabaseResource : public QThread
	{
		Q_OBJECT
	public:
		struct tagConnectionPara{
			QString connName;
			QString type;
			QString  HostAddr;
			int port;
			QString  dbName;
			QString  User;
			QString  Pass;
			QString  ExtraOptions;
			QString  testSQL;
			bool status;
			QString lastError;
		};

		explicit DatabaseResource(QObject *parent = nullptr);

		//!Get an database connection belong to current thread.
		//!if database does not exist, it will be added using dbtype
        QSqlDatabase databse(QString strDBName, bool checkConn = false);

		//!add connection connName, return true if ok.
        bool addConnection(QString connName,
                           QString type,
                           QString HostAddr,
                           int port,
                           QString dbName,
                           QString User,
                           QString Pass,
                           QString ExtraOptions,
                           QString testSQL);
        void remove_connection(QString strDBName);
		void remove_connections();
        bool confirmConnection (QString connName);

        QMap <QString, tagConnectionPara> currentDatabaseConnections(){ return m_dbNames; }

		void run();

        void TerminateMe(){ bTerm = true; }

	protected:
		bool bTerm;
		QMutex m_mutex_reg;
        QMap <QString, tagConnectionPara> m_dbNames;					//Each Main connection Name, and their connection para
        QMap <QString, QSet<QString> > m_ThreadsDB;                     //Each Main connection Name, and theri thread-owned names
        QMap <QThread *, QMap<QString, QDateTime> > m_ThreadOwnedMainDBs;//Each Thread owns Main connection Names.
		void RemoveTreadsConnections(QString mainName);
	signals:
        void evt_Message(QObject *, QString);

	public slots:
		void on_finishedThread();
	};
};
#endif // DATABASERESOURCE_H

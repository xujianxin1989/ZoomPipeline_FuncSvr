﻿#include "zpmainframe.h"
#include "ui_zpmainframe.h"
#include "network/ssl_config.h"
#include <QtCore/QMetaEnum>
#include <QDateTime>
#include <QSettings>
#include <QMessageBox>
#include <QFileDialog>
#include <QSqlDatabase>
#include <QMap>
#include <QThread>
#if (ZP_WANTSSL!=0)
#include <QSslSocket>
#endif
#include <QSystemTrayIcon>
#include "smartlink/st_clientnode_basetrans.h"
#include "dialogaddressinput.h"
#include <QHostInfo>

using namespace ZPNetwork;
using namespace ZPTaskEngine;
using namespace ZP_Cluster;

extern quint64 g_bytesRecieved;
extern quint64 g_bytesSent;
extern quint64 g_secRecieved;
extern quint64 g_secSent;

ZPMainFrame::ZPMainFrame(QWidget* parent) :
    QMainWindow(parent),
    m_pLogger(nullptr),
    ui(new Ui::ZPMainFrame),
    m_iconTray(nullptr)
{
    m_currentConfigFile = QCoreApplication::applicationFilePath() + ".ini";
    ui->setupUi(this);

	//Create net engine
    m_netEngine = new zp_net_Engine(8192);
    connect(m_netEngine, &zp_net_Engine::evt_Message, this, &ZPMainFrame::on_evt_MessageNetwork);
    connect(m_netEngine, &zp_net_Engine::evt_SocketError, this, &ZPMainFrame::on_evt_SocketError);

	//Create TaskEngine
	m_taskEngine = new zp_pipeline(this);

	//Cluster is not created
    m_pClusterTerm = new ZP_Cluster::zp_ClusterTerm("Unknown", this);
    connect(m_pClusterTerm, &ZP_Cluster::zp_ClusterTerm::evt_Message, this, &ZPMainFrame::on_evt_Message_Cluster);
    connect(m_pClusterTerm, &ZP_Cluster::zp_ClusterTerm::evt_SocketError, this, &ZPMainFrame::on_evt_SocketError_Cluster);

	//Create databases
	m_pDatabases = new ZPDatabase::DatabaseResource(this);
    connect(m_pDatabases, &ZPDatabase::DatabaseResource::evt_Message, this, &ZPMainFrame::on_evt_Message_Database);
	m_pDatabases->start();

	//Create Smartlink client table
    m_clientTable = new ExampleServer::st_client_table(m_netEngine, m_taskEngine, m_pDatabases, m_pClusterTerm, this);
    connect(m_clientTable, &ExampleServer::st_client_table::evt_Message, this, &ZPMainFrame::on_evt_Message_Smartlink);

	m_nTimerId = startTimer(2000);
    m_nTimerCheck = startTimer(10000);
	initUI();
    loadSettings(m_currentConfigFile);
    m_trayMenu = new QMenu(this);
    m_trayMenu->addAction(ui->actionShow_Window);
    m_trayMenu->addAction(ui->actionExit);

    m_iconTray = new QSystemTrayIcon(QIcon(":/icons/Resources/Color Classic.png"), this);
    m_iconTray->setContextMenu(m_trayMenu);
    m_iconTray->show();
}

ZPMainFrame::~ZPMainFrame()
{
    m_netEngine->removeAllAddresses();
    m_netEngine->kickAllClients();
    m_netEngine->deactiveImmediately();
    m_taskEngine->removeThreads(-1);

	//term the confirm check
	m_pDatabases->TerminateMe();
	m_pDatabases->remove_connections();

    m_pClusterTerm->netEng()->removeAllAddresses();
    m_pClusterTerm->netEng()->kickAllClients();
    m_pClusterTerm->netEng()->deactiveImmediately();
	m_pClusterTerm->taskEng()->removeThreads(-1);

	int maxWait = 0;
    while (m_netEngine->canExit() == false || m_taskEngine->canClose() == false ||
           m_pDatabases->isRunning() == true || m_pClusterTerm->canExit() == false)
	{
		QCoreApplication::processEvents();
		QThread::currentThread()->msleep(200);
        maxWait++;
        if (maxWait >= 150)
		{
            if (QMessageBox::information(nullptr,
                                         tr("Confire Exit"),
										 tr("There are still some clients alive in the server. continue waiting?"),
                                         QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
            {
				maxWait = 0;
            }
			else
            {
                break;
            }
		}
	}
	m_netEngine->deleteLater();
	m_pDatabases->deleteLater();
	m_taskEngine->deleteLater();
	m_pClusterTerm->deleteLater();
	delete ui;
}

void ZPMainFrame::changeEvent(QEvent* event)
{
    QMainWindow::changeEvent(event);
    switch (event->type())
    {
        case QEvent::LanguageChange:
            ui->retranslateUi(this);
            break;
        case QEvent::WindowStateChange:
            if (this->isMinimized())
            {
                this->hide();
                this->m_iconTray->showMessage(tr("Server still running"),
                                              tr("If you want to terminate server, just using exit Toolbar button."));
            }
            break;
        default:
            break;
	}
}

void ZPMainFrame::setLogger(STMsgLogger::st_logger* pLogger)
{
    this->m_pLogger = pLogger;
}

void ZPMainFrame::initUI()
{
	//Message Shown model
	m_pMsgModelNetwork = new QStandardItemModel(this);
	ui->listView_msg->setModel(m_pMsgModelNetwork);
	m_pMsgModelCluster = new QStandardItemModel(this);
	ui->listView_msg_cluster->setModel(m_pMsgModelCluster);
	m_pMsgModelDatabase = new QStandardItemModel(this);
	ui->listView_msg_database->setModel(m_pMsgModelDatabase);
	m_pMsgModelSmartlink = new QStandardItemModel(this);
	ui->listView_msg_smartlink->setModel(m_pMsgModelSmartlink);

	//Network listeners setting model
    m_pListenerModel = new QStandardItemModel(0, 4, this);
    m_pListenerModel->setHeaderData(0, Qt::Horizontal, tr("Name"));
    m_pListenerModel->setHeaderData(1, Qt::Horizontal, tr("Addr"));
    m_pListenerModel->setHeaderData(2, Qt::Horizontal, tr("Port"));
    m_pListenerModel->setHeaderData(3, Qt::Horizontal, tr("SSL"));
	ui->tableView_listen->setModel(m_pListenerModel);

	//database
    m_pDbResModel = new QStandardItemModel(0, 8, this);
    m_pDbResModel->setHeaderData(0, Qt::Horizontal, tr("Name"));
    m_pDbResModel->setHeaderData(1, Qt::Horizontal, tr("Type"));
    m_pDbResModel->setHeaderData(2, Qt::Horizontal, tr("HostAddr"));
    m_pDbResModel->setHeaderData(3, Qt::Horizontal, tr("Port"));
    m_pDbResModel->setHeaderData(4, Qt::Horizontal, tr("Database"));
    m_pDbResModel->setHeaderData(5, Qt::Horizontal, tr("Username"));
    m_pDbResModel->setHeaderData(6, Qt::Horizontal, tr("Options"));
    m_pDbResModel->setHeaderData(7, Qt::Horizontal, tr("TestSQL"));
	ui->tableView_dbconn->setModel(m_pDbResModel);
	QStringList fdrivers = QSqlDatabase::drivers();
    QStandardItemModel* pCombo = new QStandardItemModel(this);
	foreach (QString str, fdrivers)
	{
		pCombo->appendRow(new QStandardItem(str));
	}
	ui->comboBox_db_type->setModel(pCombo);

    m_pModelCluster= new QStandardItemModel(0, 6, this);
    m_pModelCluster->setHeaderData(0, Qt::Horizontal, tr("Name"));
    m_pModelCluster->setHeaderData(1, Qt::Horizontal, tr("LAN_Address"));
    m_pModelCluster->setHeaderData(2, Qt::Horizontal, tr("LAN_Port"));
    m_pModelCluster->setHeaderData(3, Qt::Horizontal, tr("Clients"));
    m_pModelCluster->setHeaderData(4, Qt::Horizontal, tr("Pub_Address"));
    m_pModelCluster->setHeaderData(5, Qt::Horizontal, tr("Pub_Port"));
	ui->tableView_activeTerms->setModel(m_pModelCluster);
    m_statusLabel = new QLabel(this);
    this->statusBar()->addWidget(m_statusLabel);
}

void ZPMainFrame::on_evt_MessageNetwork(QObject *pSource, QString strMsg)
{
	//return ;
	QDateTime dtm = QDateTime::currentDateTime();
	QString msg = dtm.toString("yyyy-MM-dd HH:mm:ss.zzz") + " " + strMsg;
    quint64 address = (quint64)pSource;
    msg += tr("Source=0x%1").arg(address, 4, 16, QLatin1Char('0'));
    int nRows = m_pMsgModelNetwork->rowCount();
    m_pMsgModelNetwork->insertRow(0, new QStandardItem(msg));
    while (nRows-- > 4096)
    {
        m_pMsgModelNetwork->removeRow(m_pMsgModelNetwork->rowCount() - 1);
    }
}

void ZPMainFrame::on_evt_Message_Database(QObject *pSource, QString strMsg)
{
    //return;
	QDateTime dtm = QDateTime::currentDateTime();
	QString msg = dtm.toString("yyyy-MM-dd HH:mm:ss.zzz") + " " + strMsg;
    quint64 address = (quint64)pSource;
    msg += tr("Source=0x%1").arg(address, 4, 16, QLatin1Char('0'));
    int nRows = m_pMsgModelDatabase->rowCount();
    m_pMsgModelDatabase->insertRow(0, new QStandardItem(msg));
    while (nRows-- > 4096)
    {
        m_pMsgModelDatabase->removeRow(m_pMsgModelDatabase->rowCount() - 1);
    }
}

void ZPMainFrame::on_evt_Message_Smartlink(QObject *pSource, QString strMsg)
{
    //return;
	QDateTime dtm = QDateTime::currentDateTime();
	QString msg = dtm.toString("yyyy-MM-dd HH:mm:ss.zzz") + " " + strMsg;
    quint64 address = (quint64)pSource;
    msg += tr("Source=0x%1").arg(address, 4, 16, QLatin1Char('0'));

    int nRows = m_pMsgModelSmartlink->rowCount();
    m_pMsgModelSmartlink->insertRow(0, new QStandardItem(msg));
    while (nRows-- > 4096)
    {
        m_pMsgModelSmartlink->removeRow(m_pMsgModelSmartlink->rowCount() - 1);
    }
}

//The socket error message
void ZPMainFrame::on_evt_SocketError(QObject *senderSock, QAbstractSocket::SocketError socketError)
{
    //return;
	QDateTime dtm = QDateTime::currentDateTime();
    QMetaEnum metaEnum = QMetaEnum::fromType<QAbstractSocket::SocketError>();
    quint64 address = (quint64)senderSock;

    QString msg = dtm.toString("yyyy-MM-dd HH:mm:ss.zzz") + " " + QString("SockError 0x%1 with %2")
                  .arg(address, 4, 16, QLatin1Char('0')).arg(metaEnum.valueToKey(socketError));
    int nRows = m_pMsgModelNetwork->rowCount();
    m_pMsgModelNetwork->insertRow(0, new QStandardItem(msg));
    while (nRows-- > 4096)
    {
        m_pMsgModelNetwork->removeRow(m_pMsgModelNetwork->rowCount() - 1);
    }
}

void ZPMainFrame::on_evt_Message_Cluster(QObject* pSource, QString strMsg)
{
	//return ;
	QDateTime dtm = QDateTime::currentDateTime();
	QString msg = dtm.toString("yyyy-MM-dd HH:mm:ss.zzz") + " " + strMsg;
    quint64 address = (quint64)pSource;
    msg += tr("Source=0x%1").arg(address, 4, 16, QLatin1Char('0'));
	int nrows = m_pMsgModelCluster->rowCount();
    m_pMsgModelCluster->insertRow(0, new QStandardItem(msg));
	while (nrows-- > 4096)
    {
        m_pMsgModelCluster->removeRow(m_pMsgModelCluster->rowCount() - 1);
    }
}

//The socket error message
void ZPMainFrame::on_evt_SocketError_Cluster(QObject* senderSock, QAbstractSocket::SocketError socketError)
{
    //return;
	QDateTime dtm = QDateTime::currentDateTime();
    QMetaEnum metaEnum = QMetaEnum::fromType<QAbstractSocket::SocketError>();
    quint64 address = (quint64)senderSock;

    QString msg = dtm.toString("yyyy-MM-dd HH:mm:ss.zzz") + " " + QString("SockError 0x%1 with %2")
                  .arg(address, 4, 16, QLatin1Char('0')).arg(metaEnum.valueToKey(socketError));
	int nrows = m_pMsgModelCluster->rowCount();
    m_pMsgModelCluster->insertRow(0, new QStandardItem(msg));
	while (nrows-- > 4096)
    {
        m_pMsgModelCluster->removeRow(m_pMsgModelCluster->rowCount() - 1);
    }
}

void ZPMainFrame::timerEvent(QTimerEvent* event)
{
	static int lastRecieved = 0;
	static int lastSent = 0;

    if (event->timerId() == m_nTimerId)
	{
		killTimer(m_nTimerId);
		m_nTimerId = -1;
		//recording net status
		QString str_msg;
        QStringList lstListeners = m_netEngine->listenerNames();
		str_msg += tr("Current Listen Threads: %1\n").arg(lstListeners.size());
        for (int i = 0; i < lstListeners.size(); i++)
        {
            str_msg += tr("\tListen Threads %1: %2\n").arg(i + 1).arg(lstListeners.at(i));
        }
        int nClientThreads = m_netEngine->transThreadNum();

		str_msg += tr("Current Trans Threads: %1\n").arg(nClientThreads);
		int nTotalCLientsNums = 0;
        for (int i = 0; i < nClientThreads; i++)
		{
            str_msg += tr("\t%1:%2").arg(i + 1).arg(m_netEngine->totalClients(i));
            if ((i + 1) % 5 == 0)
            {
                str_msg += "\n";
            }
			nTotalCLientsNums += m_netEngine->totalClients(i);
		}

		//Set This message to Cluster Info.
		m_pClusterTerm->setClientNums(nTotalCLientsNums);

		str_msg += "\n";
		//recording task status
		str_msg += tr("Current Task Threads: %1\n").arg(m_taskEngine->threadsCount());
        str_msg += tr("Current Task Payload: %1\n").arg(m_taskEngine->payLoad());
		str_msg += tr("Current Task Idle Threads: %1\n").arg(m_taskEngine->idleThreads());

		QMap<QString,ZPDatabase::DatabaseResource::tagConnectionPara> map_conns
				= m_pDatabases->currentDatabaseConnections();
		str_msg += tr("Database Connections: %1\n").arg(map_conns.size());
        foreach (QString name, map_conns.keys())
		{
            ZPDatabase::DatabaseResource::tagConnectionPara& para = map_conns[name];
			str_msg += tr("\t%1 status = %2").arg(name).arg(para.status);
            if (para.status == false)
            {
                str_msg += ", Msg=" + para.lastError;
            }
			str_msg += "\n";
		}
		//Cluster----------------------------

		str_msg += tr("Cluster Paras:\n");
		str_msg += tr("\tTerminal %1 : %2\n\t\tLAN Address: %3:%4\n\t\t Publish: %5:%6\n\t\tbalance max clients=%7\n")
                .arg(m_pClusterTerm->termName())
                .arg(!m_pClusterTerm->netEng()->listenerNames().empty())
				.arg(m_pClusterTerm->LANAddr().toString())
				.arg(m_pClusterTerm->LANPort())
				.arg(m_pClusterTerm->PublishAddr().toString())
				.arg(m_pClusterTerm->PublishPort())
                .arg(m_clientTable->balanceMax());

        nClientThreads = m_pClusterTerm->netEng()->transThreadNum();
		str_msg += "\t"+tr("Trans Threads: %1\n").arg(nClientThreads);
        for (int i = 0; i < nClientThreads; i++)
		{
			str_msg += tr("\t%1:%2").arg(i+1).arg(m_pClusterTerm->netEng()->totalClients(i));
            if ((i + 1) % 5 == 0)
            {
                str_msg += "\n";
            }
		}
		str_msg += "\n";
		str_msg += tr("\tShift Threads: %1\n").arg(m_pClusterTerm->taskEng()->threadsCount());
        str_msg += tr("\tShift Payload: %1\n").arg(m_pClusterTerm->taskEng()->payLoad());
		str_msg += tr("\tShift Idle Threads: %1\n").arg(m_pClusterTerm->taskEng()->idleThreads());

		str_msg += tr("Smartlink Function Server Paras:\n");
        str_msg += tr("\tUser Account Database is : %1\n").arg(m_clientTable->database_UserAcct());
        str_msg += tr("\tEvent Database is : %1\n").arg(m_clientTable->database_Event());
		str_msg += tr("\tLocal folder for large file is : %1\n").arg(m_clientTable->largeFileFolder());
		str_msg += tr("\tHeart beating Threadhold is : %1\n").arg(m_clientTable->heartBeatingThrd());

		ui->plainTextEdit_status_net->setPlainText(str_msg);

		//The Cluster Info
        QStringList listCluster = m_pClusterTerm->svrNames();
        if (m_pModelCluster->rowCount() > 0)
        {
            m_pModelCluster->removeRows(0, m_pModelCluster->rowCount());
        }
		int nInserted = 0;
		m_pModelCluster->insertRow(nInserted);
        m_pModelCluster->setData(m_pModelCluster->index(nInserted, 0), m_pClusterTerm->termName());
        m_pModelCluster->setData(m_pModelCluster->index(nInserted, 1), m_pClusterTerm->LANAddr().toString());
        m_pModelCluster->setData(m_pModelCluster->index(nInserted, 2), m_pClusterTerm->LANPort());
        m_pModelCluster->setData(m_pModelCluster->index(nInserted, 3), m_pClusterTerm->clientNums());
        m_pModelCluster->setData(m_pModelCluster->index(nInserted, 4), m_pClusterTerm->PublishAddr().toString());
        m_pModelCluster->setData(m_pModelCluster->index(nInserted, 5), m_pClusterTerm->PublishPort());

		++nInserted;
        foreach (QString strNodeName, listCluster)
		{
			m_pModelCluster->insertRow(nInserted);
            m_pModelCluster->setData(m_pModelCluster->index(nInserted, 0), strNodeName);
            m_pModelCluster->setData(m_pModelCluster->index(nInserted, 1), m_pClusterTerm->svrLANAddr(strNodeName).toString());
            m_pModelCluster->setData(m_pModelCluster->index(nInserted, 2), m_pClusterTerm->svrLANPort(strNodeName));
            m_pModelCluster->setData(m_pModelCluster->index(nInserted, 3), m_pClusterTerm->remoteClientNums(strNodeName));
            m_pModelCluster->setData(m_pModelCluster->index(nInserted, 4), m_pClusterTerm->svrPubAddr(strNodeName).toString());
            m_pModelCluster->setData(m_pModelCluster->index(nInserted, 5), m_pClusterTerm->svrPubPort(strNodeName));
			++nInserted;
		}
        QString str = QDateTime::currentDateTime().toString(tr("yyyy年MM月dd日 HH:mm:ss"));
		str += tr(" Rec %1B(%3 kbps)  Sent %2B(%4 kbps)")
				.arg(g_bytesRecieved)
				.arg(g_bytesSent)
                .arg(g_secRecieved * 8 / 1024 / 2)
                .arg(g_secSent * 8 / 1024 / 2);

        if ((int)g_bytesRecieved != lastRecieved || (int)g_bytesSent != lastSent)
		{
			static int ppIc = 0;
			lastRecieved = g_bytesRecieved;
			lastSent = g_bytesSent;
            if (++ppIc % 2 == 0)
            {
                m_iconTray->setIcon(QIcon(":/icons/Resources/Color Classic Green.png"));
            }
			else
            {
                m_iconTray->setIcon(QIcon(":/icons/Resources/Color Classic, Performa 250, 275.png"));
            }
		}
		else
		{
            m_iconTray->setIcon(QIcon(":/icons/Resources/Color Classic.png"));
		}
		g_secRecieved = g_secSent = 0;
        m_statusLabel->setText(str);
		m_nTimerId = startTimer(2000);
	}
    else if (event->timerId() == m_nTimerCheck)
	{
		killTimer(m_nTimerCheck);
		m_nTimerCheck = -1;

        m_clientTable->kickDeadClients();
        m_pClusterTerm->sendHeartBeatings();
        m_pClusterTerm->kickDeadClients();

        if (ui->action_Start_Stop->isChecked())
		{
            //Cluster re-connection test
            QStringList listCluster = m_pClusterTerm->svrNames();
            if (listCluster.size() == 0 && m_dtmLastClusterJoin.secsTo(QDateTime::currentDateTime()) >= 120)
			{
                m_pClusterTerm->netEng()->kickAllClients();
                QSettings settings(m_currentConfigFile, QSettings::IniFormat);
                QString strAddr = settings.value("history/clusterAddr", "192.168.1.118").toString();
                QString strPort = settings.value("history/clusterPort", "25600").toString();
                //根据主机名，使用类 QHostInfo 来获取该主机的 IP 地址
                QString ipAddress;
                QHostInfo info = QHostInfo::fromName(strAddr);
                foreach (const QHostAddress& address, info.addresses())
                {
                    if (address.protocol() != QAbstractSocket::IPv4Protocol)
                    {
                        continue;
                    }
                    ipAddress = address.toString();
                    break;
                }
                m_pClusterTerm->joinCluster(QHostAddress(ipAddress), strPort.toInt());
				m_dtmLastClusterJoin = QDateTime::currentDateTime();
			}
		}
		m_nTimerCheck = startTimer(5000);
	}
}

void ZPMainFrame::on_action_Start_Stop_triggered(bool setordel)
{
    if (setordel)
	{
		forkServer(m_currentConfigFile);
		//Cluster Reconnection Dtm Protect
		m_dtmLastClusterJoin = QDateTime::currentDateTime();
		return;
	}
	m_netEngine->removeAllAddresses();
	m_netEngine->removeClientTransThreads(-1, true);
	m_netEngine->removeClientTransThreads(-1, false);
	m_taskEngine->removeThreads(-1);
	m_pDatabases->remove_connections();
	m_pClusterTerm->netEng()->removeListeningAddress("clusterTerm");
	m_pClusterTerm->netEng()->removeClientTransThreads(-1, false);
	m_pClusterTerm->taskEng()->removeThreads(-1);
}

void ZPMainFrame::forkServer(QString config_file)
{
    QSettings settings(config_file, QSettings::IniFormat);
    int nListeners = settings.value("settings/listeners", 0).toInt();
    if (nListeners < 0)
    {
        nListeners = 0;
    }
    if (nListeners >= 1024)
    {
        nListeners = 1024;
    }
	//read listeners from ini
    m_netEngine->removeAllAddresses();
    while (m_netEngine->listenerNames().size())
	{
		QThread::currentThread()->msleep(200);
		QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
	}
    for (int i = 0; i < nListeners; i++)
	{
        QString keyPrefix = QString("listener%1/").arg(i);
        QString listen_name = settings.value(keyPrefix + "name", QString("Listener%1").arg(i)).toString();
        QString address = settings.value(keyPrefix + "address", QString("127.0.0.1")).toString();
        QHostAddress listen_address(address);
        int nPort = settings.value(keyPrefix + "port", 23456 + i).toInt();
        bool bSSL = settings.value(keyPrefix + "ssl", i % 2 ? true : false).toBool();
        if (listen_address.isNull() || nPort < 1024 || nPort >= 32768)
        {
            continue;
        }
        m_netEngine->addListeningAddress(listen_name, listen_address, nPort, bSSL);
	}
    //服务器端最佳线程数量 = ((线程等待时间 + 线程cpu时间) / 线程cpu时间) * cpu数量
	//read thread config
    int nSSLThreads = settings.value("settings/nSSLThreads", 4).toInt();
    int nPlainThreads = settings.value("settings/nPlainThreads", 4).toInt();
    int nWorkingThreads = settings.value("settings/nWorkingThreads", 8).toInt();

    int nDeltaSSL = m_netEngine->transThreadNum(true) - nSSLThreads;
    if (nDeltaSSL > 0)
    {
        m_netEngine->removeClientTransThreads(nDeltaSSL, true);
    }
    else if (nDeltaSSL < 0)
    {
        m_netEngine->addClientTransThreads(-nDeltaSSL, true);
    }

    int nDeltaPlain = m_netEngine->transThreadNum(false) - nPlainThreads;
    if (nDeltaPlain > 0)
    {
        m_netEngine->removeClientTransThreads(nDeltaPlain, false);
    }
    else if (nDeltaPlain < 0)
    {
        m_netEngine->addClientTransThreads(-nDeltaPlain, false);
    }

	int nDeltaWorking = m_taskEngine->threadsCount() - nWorkingThreads;
    if (nDeltaWorking > 0)
    {
        m_taskEngine->removeThreads(nDeltaWorking);
    }
	else
    {
        m_taskEngine->addThreads(-nDeltaWorking);
    }

	//database connections
	m_pDatabases->remove_connections();
    int nDBConns = settings.value("settings/dbresources", 0).toInt();
    if (nDBConns >= 1024)
    {
        nDBConns = 1024;
    }
    for (int i = 0; i < nDBConns; i++)
	{
		QString keyPrefix = QString ("dbres%1/").arg(i);
        QString db_name = settings.value(keyPrefix + "name", "").toString();
        QString db_type = settings.value(keyPrefix + "type", "").toString();
        QString db_Address = settings.value(keyPrefix + "addr", "").toString();
        int nPort = settings.value(keyPrefix + "port", 0).toInt();
        QString db_Schema = settings.value(keyPrefix + "schema", "").toString();
        QString db_User = settings.value(keyPrefix + "user", "").toString();
        QString db_Pass = settings.value(keyPrefix + "pass", "").toString();
        QString db_Extra = settings.value(keyPrefix + "extra", "").toString();
        QString db_testSQL = settings.value(keyPrefix + "testSql", "").toString();
        if (db_name.length() < 1 || db_type.length() < 1)
        {
            continue;
        }
        bool bRet = m_pDatabases->addConnection(db_name, db_type, db_Address, nPort, db_Schema, db_User, db_Pass, db_Extra, db_testSQL);
        qDebug() << bRet;
	}


	//Smartlink settings
    int nHeartbeatingThreadhold = settings.value("Smartlink/HeartbeatingThreadhold", 180).toInt();
    if (nHeartbeatingThreadhold >= 60 && nHeartbeatingThreadhold <= 300)
    {
        m_clientTable->setHeartBeatingThrd(nHeartbeatingThreadhold);
    }

    QString strSLDB_useracc = settings.value("Smartlink/SLDB_useracc", "EMPTY").toString();
	m_clientTable->setDatabase_UserAcct(strSLDB_useracc);

    QString strSLDB_mainEvent = settings.value("Smartlink/SLDB_mainEvt", "EMPTY").toString();
	m_clientTable->setDatabase_Event(strSLDB_mainEvent);

    QString strSL_LargetFolder = settings.value("Smartlink/SL_LargetFolder", "NUL").toString();
	m_clientTable->setLargeFileFolder(strSL_LargetFolder);
    int nmaxBalance = settings.value("Smartlink/nmaxBalance", "1024").toInt();
	m_clientTable->setBalanceMax(nmaxBalance);
    //Clusters
    QString strClusterTermAddr = settings.value("Cluster/strClusterTermAddr", "0.0.0.0").toString();
    QString strClusterTermPort = settings.value("Cluster/strClusterTermPort", "25600").toString();
    QString strClusterPubName = settings.value("Cluster/strClusterPubName", "Term 001").toString();
    QString strClusterLANAddr = settings.value("Cluster/strClusterLANAddr", "127.0.0.1").toString();
    QString strClusterLANPort = settings.value("Cluster/strClusterLANPort", "25600").toString();
    QString strClusterPubAddr = settings.value("Cluster/strClusterPubAddr", "192.168.1.111").toString();
    QString strClusterPubPort = settings.value("Cluster/strClusterPubPort", "25600").toString();
    int nClusterTransThreads = settings.value("Cluster/nClusterTransThreads", "4").toInt();
    int nClusterWorkingThreads = settings.value("Cluster/nClusterWorkingThreads", "4").toInt();
    this->m_pClusterTerm->netEng()->removeAllAddresses();
    this->m_pClusterTerm->netEng()->removeClientTransThreads(-1, false);
	this->m_pClusterTerm->taskEng()->removeThreads(-1);
    while (m_pClusterTerm->netEng()->listenerNames().size())
	{
		QThread::currentThread()->msleep(200);
		QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
	}
    this->m_pClusterTerm->netEng()->addClientTransThreads(nClusterTransThreads, false);
	this->m_pClusterTerm->taskEng()->addThreads(nClusterWorkingThreads);
    this->m_pClusterTerm->setTermName(strClusterPubName);
	this->m_pClusterTerm->setLANAddr(QHostAddress(strClusterLANAddr));
	this->m_pClusterTerm->setLANPort(strClusterLANPort.toInt());
	this->m_pClusterTerm->setPublishAddr(QHostAddress(strClusterPubAddr));
	this->m_pClusterTerm->setPublishPort(strClusterPubPort.toInt());
    this->m_pClusterTerm->startListen(QHostAddress(strClusterTermAddr), strClusterTermPort.toInt());

    int nRabbishCanSize = settings.value("settings/rubbish_can_size", "32").toInt();
	ZPNetwork::zp_netTransThread::RUBBISH_CAN_SIZE = nRabbishCanSize;

    if (this->windowTitle().indexOf("(") == -1)
    {
        this->setWindowTitle(windowTitle() + "(" + strClusterPubName +")");
    }
}

void ZPMainFrame::on_action_About_triggered()
{
	QApplication::aboutQt();
}

void ZPMainFrame::loadSettings(QString config_file)
{
    QSettings settings(config_file, QSettings::IniFormat);
    int nListeners = settings.value("settings/listeners", 0).toInt();
    if (nListeners < 0)
    {
        nListeners = 0;
    }
    if (nListeners > 1024)
    {
        nListeners = 1024;
    }
    m_pListenerModel->removeRows(0, m_pListenerModel->rowCount());
	m_set_listenerNames.clear();
	//read listeners from ini
	int nInserted = 0;
    for (int i = 0; i < nListeners; i++)
	{
        QString keyPrefix = QString("listener%1/").arg(i);
        QString listen_name = settings.value(keyPrefix + "name", QString("Listener%1").arg(i)).toString();
        QString Address = settings.value(keyPrefix + "address", QString("127.0.0.1")).toString();
        QHostAddress listen_address(Address) ;

        int nPort = settings.value(keyPrefix + "port", 23456 + i).toInt();
        bool bSSL = settings.value(keyPrefix + "ssl", i % 2 ? true : false).toBool();
        if (listen_address.isNull() || nPort < 1024 || nPort >= 32768)
        {
            continue;
        }
		if (m_set_listenerNames.contains(listen_name))
        {
            continue;
        }
        //监听项
		m_set_listenerNames.insert(listen_name);
		m_pListenerModel->insertRow(nInserted);
        m_pListenerModel->setData(m_pListenerModel->index(nInserted, 0), listen_name);
        m_pListenerModel->setData(m_pListenerModel->index(nInserted, 1), listen_address.toString());
        m_pListenerModel->setData(m_pListenerModel->index(nInserted, 2), nPort);
        m_pListenerModel->setData(m_pListenerModel->index(nInserted, 3), bSSL);
		nInserted++;
	}
	//read thread config
    int nSSLThreads = settings.value("settings/nSSLThreads", 4).toInt();
    int nPlainThreads = settings.value("settings/nPlainThreads", 4).toInt();
    int nWorkingThreads = settings.value("settings/nWorkingThreads", 8).toInt();
	ui->dial_plain_trans_threads->setValue(nPlainThreads);
	ui->dial_ssl_trans_threads->setValue(nSSLThreads);
	ui->dial_task_working_threads->setValue(nWorkingThreads);

	//read db connections
	m_set_DbResNames.clear();
    m_pDbResModel->removeRows(0, m_pDbResModel->rowCount());
    int nDBConns = settings.value("settings/dbresources", 0).toInt();
    if (nDBConns >= 1024)
    {
        nDBConns = 1024;
    }
	nInserted = 0;
    for (int i = 0; i < nDBConns; i++)
	{
        QString keyPrefix = QString("dbres%1/").arg(i);
        QString db_name = settings.value(keyPrefix + "name", "").toString();
        QString db_type = settings.value(keyPrefix + "type", "").toString();
        QString db_Address = settings.value(keyPrefix + "addr", "").toString();
        int nPort = settings.value(keyPrefix + "port", 0).toInt();
        QString db_Schema = settings.value(keyPrefix + "schema", "").toString();
        QString db_User = settings.value(keyPrefix + "user", "").toString();
        QString db_Pass = settings.value(keyPrefix + "pass", "").toString();
        QString db_Extra = settings.value(keyPrefix + "extra", "").toString();
        QString db_testSQL = settings.value(keyPrefix + "testSql", "").toString();
        if (db_name.length() < 1 || m_set_DbResNames.contains(db_name))
        {
            continue;
        }
        if (db_type.length() < 1)
        {
            continue;
        }
		m_set_DbResNames[db_name] = db_Pass;
		m_pDbResModel->insertRow(nInserted);

        m_pDbResModel->setData(m_pDbResModel->index(nInserted, 0), db_name);
        m_pDbResModel->setData(m_pDbResModel->index(nInserted, 1), db_type);
        m_pDbResModel->setData(m_pDbResModel->index(nInserted, 2), db_Address);
        m_pDbResModel->setData(m_pDbResModel->index(nInserted, 3), nPort);
        m_pDbResModel->setData(m_pDbResModel->index(nInserted, 4), db_Schema);
        m_pDbResModel->setData(m_pDbResModel->index(nInserted, 5), db_User);
        m_pDbResModel->setData(m_pDbResModel->index(nInserted, 6), db_Extra);
        m_pDbResModel->setData(m_pDbResModel->index(nInserted, 7), db_testSQL);
		nInserted++;
	}

	//Smartlink settings
    int nHeartbeatingThreadhold = settings.value("Smartlink/HeartbeatingThreadhold", 180).toInt();
    if (nHeartbeatingThreadhold >= 60 && nHeartbeatingThreadhold <= 300)
    {
        ui->horizontalSlider_heartbeating->setValue(nHeartbeatingThreadhold);
    }
	else
    {
        ui->horizontalSlider_heartbeating->setValue(180);
    }

	ui->lcdNumber_heartbeating->display(ui->horizontalSlider_heartbeating->value());

    QString strSLDB_useracc = settings.value("Smartlink/SLDB_useracc", "EMPTY").toString();
	ui->lineEdit_SL_DB_Account->setText(strSLDB_useracc);

    QString strSLDB_mainEvent = settings.value("Smartlink/SLDB_mainEvt", "EMPTY").toString();
	ui->lineEdit_SL_DB_ME->setText(strSLDB_mainEvent);

    int nmaxBalance = settings.value("Smartlink/nmaxBalance", "1024").toInt();
	ui->spinBox_cluster_max_payload->setValue(nmaxBalance);

	//Cluster
    QString strClusterTermAddr = settings.value("Cluster/strClusterTermAddr", "0.0.0.0").toString();
	ui->lineEdit_cluster_term_addr->setText(strClusterTermAddr);
    QString strClusterTermPort = settings.value("Cluster/strClusterTermPort", "25600").toString();
	ui->lineEdit_cluster_term_port->setText(strClusterTermPort);
    QString strClusterPubName = settings.value("Cluster/strClusterPubName", "Term 001").toString();
	ui->lineEdit_cluster_pub_name->setText(strClusterPubName);
    QString strClusterLANAddr = settings.value("Cluster/strClusterLANAddr", "127.0.0.1").toString();
	ui->lineEdit_cluster_LAN_Addr->setText(strClusterLANAddr);
    QString strClusterLANPort = settings.value("Cluster/strClusterLANPort", "25600").toString();
	ui->lineEdit_cluster_LAN_Port->setText(strClusterLANPort);

    QString strClusterPubAddr = settings.value("Cluster/strClusterPubAddr", "192.168.1.111").toString();
	ui->lineEdit_cluster_pub_Addr->setText(strClusterPubAddr);
    QString strClusterPubPort = settings.value("Cluster/strClusterPubPort", "25600").toString();
	ui->lineEdit_cluster_pub_Port->setText(strClusterPubPort);

    int nClusterTransThreads = settings.value("Cluster/nClusterTransThreads", "4").toInt();
	ui->horizontalSlider_cluster_transThreads->setValue(nClusterTransThreads);
    int nClusterWorkingThreads = settings.value("Cluster/nClusterWorkingThreads", "4").toInt();
	ui->horizontalSlider_cluster_workingThread->setValue(nClusterWorkingThreads);
	//Rubbish Can (for Sockets)
    int nRabbishCanSize = settings.value("settings/rubbish_can_size", "32").toInt();
	ui->horizontalSlider_rubbishCan->setValue(nRabbishCanSize);
}


void ZPMainFrame::saveSettings(QString config_file)
{
	QSettings settings(config_file,QSettings::IniFormat);
	int nListeners = m_pListenerModel->rowCount();
	settings.setValue("settings/listeners",nListeners);
	//save listeners to ini
	int nRealsave = 0;
    for (int i = 0; i < nListeners; i++)
	{
		QString keyPrefix = QString ("listener%1/").arg(nRealsave);
        QString listen_name = m_pListenerModel->data(m_pListenerModel->index(i, 0)).toString();
        settings.setValue(keyPrefix + "name", listen_name);
        QHostAddress listen_address (m_pListenerModel->data(m_pListenerModel->index(i, 1)).toString()) ;
        settings.setValue(keyPrefix + "address", listen_address.toString());
        int nPort = m_pListenerModel->data(m_pListenerModel->index(i, 2)).toInt();
        settings.setValue(keyPrefix + "port", nPort);
        bool bSSL = m_pListenerModel->data(m_pListenerModel->index(i, 3)).toBool();
        settings.setValue(keyPrefix + "ssl", bSSL);
        if (listen_name.length() <= 0 || nPort < 1024 || nPort > 32767)
        {
            continue;
        }
		nRealsave++;
	}
    settings.setValue("settings/listeners", nRealsave);

	//save thread config
	int nSSLThreads =  ui->dial_plain_trans_threads->value();
    settings.setValue("settings/nPlainThreads", nSSLThreads);
	int nPlainThreads = ui->dial_ssl_trans_threads->value();
    settings.setValue("settings/nSSLThreads", nPlainThreads);

	int nWorkingThreads = ui->dial_task_working_threads->value();
    settings.setValue("settings/nWorkingThreads", nWorkingThreads);

	//Save Database Connections
	int nDBRess = m_pDbResModel->rowCount();
    settings.setValue("settings/dbresources", nDBRess);
    for (int i = 0; i < nDBRess; i++)
	{
		QString keyPrefix = QString ("dbres%1/").arg(i);
        QString db_name = m_pDbResModel->data(m_pDbResModel->index(i, 0)).toString();
        settings.setValue(keyPrefix + "name", db_name);
        QString db_type = m_pDbResModel->data(m_pDbResModel->index(i, 1)).toString() ;
        settings.setValue(keyPrefix + "type", db_type);
        QString db_Address = m_pDbResModel->data(m_pDbResModel->index(i, 2)).toString() ;
        settings.setValue(keyPrefix + "addr", db_Address);
        int nPort = m_pDbResModel->data(m_pDbResModel->index(i, 3)).toInt();
        settings.setValue(keyPrefix + "port", nPort);
        QString db_Schema = m_pDbResModel->data(m_pDbResModel->index(i, 4)).toString() ;
        settings.setValue(keyPrefix + "schema", db_Schema);
        QString db_User = m_pDbResModel->data(m_pDbResModel->index(i, 5)).toString() ;
        settings.setValue(keyPrefix + "user", db_User);
		QString db_Pass = m_set_DbResNames[db_name];
        settings.setValue(keyPrefix + "pass", db_Pass);
        QString db_Extra = m_pDbResModel->data(m_pDbResModel->index(i, 6)).toString() ;
        settings.setValue(keyPrefix + "extra", db_Extra);
        QString db_testSQL = m_pDbResModel->data(m_pDbResModel->index(i, 7)).toString() ;
        settings.setValue(keyPrefix + "testSql", db_testSQL);
	}

	//Smartlink settings
	int nHeartbeatingThreadhold = ui->horizontalSlider_heartbeating->value();
    settings.setValue("Smartlink/HeartbeatingThreadhold", nHeartbeatingThreadhold);
	QString strSLDB_useracc = ui->lineEdit_SL_DB_Account->text();
    settings.setValue("Smartlink/SLDB_useracc", strSLDB_useracc);
	QString strSLDB_mainEvent = ui->lineEdit_SL_DB_ME->text();
    settings.setValue("Smartlink/SLDB_mainEvt", strSLDB_mainEvent);
	int nmaxBalance = ui->spinBox_cluster_max_payload->value();
    settings.setValue("Smartlink/nmaxBalance", nmaxBalance);

	//Cluster
    settings.setValue("Cluster/strClusterTermAddr", ui->lineEdit_cluster_term_addr->text());
    settings.setValue("Cluster/strClusterTermPort", ui->lineEdit_cluster_term_port->text());
    settings.setValue("Cluster/strClusterPubName",  ui->lineEdit_cluster_pub_name->text());
    settings.setValue("Cluster/strClusterLANAddr", ui->lineEdit_cluster_LAN_Addr->text());
    settings.setValue("Cluster/strClusterLANPort", ui->lineEdit_cluster_LAN_Port->text());
    settings.setValue("Cluster/strClusterPubAddr", ui->lineEdit_cluster_pub_Addr->text());
    settings.setValue("Cluster/strClusterPubPort", ui->lineEdit_cluster_pub_Port->text());
    settings.setValue("Cluster/nClusterTransThreads", ui->horizontalSlider_cluster_transThreads->value());
	settings.setValue("Cluster/nClusterWorkingThreads", ui->horizontalSlider_cluster_workingThread->value());
	//Rubbish Can (for Sockets)
	int nRabbishCanSize =ui->horizontalSlider_rubbishCan->value();
    settings.setValue("settings/rubbish_can_size", nRabbishCanSize);
}

void ZPMainFrame::on_pushButton_addListener_clicked()
{
	QString name = ui->lineEdit_listenerName->text();
	QString Addr = ui->lineEdit_listenerAddr->text();
	QHostAddress address(Addr);
	if (address.isNull())
    {
        address = QHostAddress(QHostAddress::Any);
    }
	QString Port = ui->lineEdit_listenerPort->text();
	int nPort = Port.toInt();
	bool bSSL = ui->checkBox_listener_ssl->isChecked();
	if (m_set_listenerNames.contains(name))
	{
        QMessageBox::information(this, tr("Name Already Used."), tr("The listener name has been used."));
		return;
	}
    if (address.isNull() || nPort < 1024 || nPort > 32767)
	{
        QMessageBox::information(this, tr("Invalid Paraments."), tr("Address must be valid, Port between 1024 to 32767."));
		return;
	}
	int nRowCount = m_pListenerModel->rowCount();
	m_pListenerModel->insertRow(nRowCount);
    m_pListenerModel->setData(m_pListenerModel->index(nRowCount, 0), name);
    m_pListenerModel->setData(m_pListenerModel->index(nRowCount, 1), address.toString());
    m_pListenerModel->setData(m_pListenerModel->index(nRowCount, 2), nPort);
    m_pListenerModel->setData(m_pListenerModel->index(nRowCount, 3), bSSL);
	m_set_listenerNames.insert( name);
}

void ZPMainFrame::on_pushButton_delListener_clicked()
{
    QItemSelectionModel* ptr = ui->tableView_listen->selectionModel();
	QModelIndexList lst = ptr->selectedIndexes();
	QSet<int> nRows;
	foreach (QModelIndex item, lst)
    {
        nRows.insert(item.row());
    }
	int nct = 0;
	foreach (int row, nRows)
	{
		m_pListenerModel->removeRow(row - nct);
		nct++;
	}
}

void ZPMainFrame::on_pushButton_listerner_apply_clicked()
{
    saveSettings(m_currentConfigFile);
}

void ZPMainFrame::on_pushButton_threadsApply_clicked()
{
    saveSettings(m_currentConfigFile);
}

void ZPMainFrame::on_pushButton_cluster_apply_clicked()
{
    saveSettings(m_currentConfigFile);
}

void ZPMainFrame::on_pushButton_smartlink_save_clicked()
{
    saveSettings(m_currentConfigFile);
}

void ZPMainFrame::on_actionReload_config_file_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open Conf file"),
                                                    QCoreApplication::applicationDirPath(),
													tr("Ini files(*.ini)"));
    if (fileName.isEmpty())
    {
        return;
    }
    //SaveSettings(m_currentConffile);
    m_currentConfigFile = fileName;
    loadSettings(m_currentConfigFile);
    forkServer(m_currentConfigFile);
    //Cluster Reconnection Dtm Protect
    m_dtmLastClusterJoin = QDateTime::currentDateTime();
}

void ZPMainFrame::on_pushButton_db_add_clicked()
{
	QString name = ui->lineEdit_db_name->text();
    if (name.length() <= 0)
	{
        QMessageBox::information(this, tr("Name can't be empty."), tr("Database name can not be empty."));
		return;
	}
    if (m_set_DbResNames.contains(name))
	{
        QMessageBox::information(this, tr("Name already exist."), tr("Database name already exist."));
		return;
	}
	int nRow = m_pDbResModel->rowCount();
	m_pDbResModel->insertRow(nRow);
    m_pDbResModel->setData(m_pDbResModel->index(nRow, 0), ui->lineEdit_db_name->text());
    m_pDbResModel->setData(m_pDbResModel->index(nRow, 1), ui->comboBox_db_type->currentText());
    m_pDbResModel->setData(m_pDbResModel->index(nRow, 2), ui->lineEdit_db_addr->text());
    m_pDbResModel->setData(m_pDbResModel->index(nRow, 3), ui->lineEdit_db_port->text().toInt());
    m_pDbResModel->setData(m_pDbResModel->index(nRow, 4), ui->lineEdit_db_schema->text());
    m_pDbResModel->setData(m_pDbResModel->index(nRow, 5), ui->lineEdit_db_user->text());
	m_set_DbResNames[name] = ui->lineEdit_db_pass->text();
}

void ZPMainFrame::on_pushButton_db_del_clicked()
{
    QItemSelectionModel* ptr = ui->tableView_dbconn->selectionModel();
    QModelIndexList list = ptr->selectedIndexes();
	QSet<int> nRows;
    foreach (QModelIndex item, list)
    {
        nRows.insert(item.row());
    }
	int nct = 0;
	foreach (int row, nRows)
	{
        QString names = m_pDbResModel->data(m_pDbResModel->index(row, 0)).toString();
		m_set_DbResNames.remove(names);
		m_pDbResModel->removeRow(row - nct);
		nct++;
	}
}

void ZPMainFrame::on_pushButton_db_apply_clicked()
{
    saveSettings(m_currentConfigFile);
}

void ZPMainFrame::on_pushButton_join_clicked()
{
    QSettings settings(this->m_currentConfigFile, QSettings::IniFormat);
    QString strAddr = settings.value("history/clusterAddr", "192.168.1.118").toString();
    QString strPort = settings.value("history/clusterPort", "25600").toString();
	DialogAddressInput inputdlg(this);
    inputdlg.setAddr(strAddr, strPort);
    if (inputdlg.exec() != QDialog::Accepted)
    {
        return;
    }
    settings.setValue("history/clusterAddr", inputdlg.addr());
    settings.setValue("history/clusterPort", inputdlg.port());

    //根据主机名，使用类 QHostInfo 来获取该主机的 IP 地址
    QString ipAddress;
    QHostInfo info = QHostInfo::fromName(inputdlg.addr());
    foreach (const QHostAddress& address, info.addresses())
    {
        if (address.protocol() == QAbstractSocket::IPv4Protocol)
        {
            ipAddress = address.toString();
            break;
        }
    }

    m_pClusterTerm->joinCluster(QHostAddress(ipAddress), inputdlg.port().toInt());
}

void ZPMainFrame::loadSettingsAndForkServer(const QString& configFile)
{
    if (configFile.length() > 2)
    {
        this->m_currentConfigFile = configFile;
    }
    loadSettings(m_currentConfigFile);
    if (ui->action_Start_Stop->isChecked())
    {
        on_action_Start_Stop_triggered(false);
    }
	on_action_Start_Stop_triggered(true);
	ui->action_Start_Stop->setChecked(true);
    //Join the cluster immediatly, trying for 5 times, 2 seconds each.
    QSettings settings(this->m_currentConfigFile, QSettings::IniFormat);
    QString strAddr = settings.value("history/clusterAddr", "192.168.1.118").toString();
    QString strPort = settings.value("history/clusterPort", "25600").toString();
    //根据主机名，使用类 QHostInfo 来获取该主机的 IP 地址
    QString ipAddress;
    QHostInfo info = QHostInfo::fromName(strAddr);
    foreach (const QHostAddress& address, info.addresses())
    {
        if (address.protocol() == QAbstractSocket::IPv4Protocol)
        {
            ipAddress = address.toString();
            break;
        }
    }

    for (int i = 0; i < 5; ++i)
	{
        m_pClusterTerm->joinCluster(QHostAddress(ipAddress), strPort.toInt());
        for (int j = 0; j < 10; ++j)
		{
			QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
			QThread::currentThread()->msleep(200);
		}
	}
}

void ZPMainFrame::on_actionShow_Window_triggered()
{
	this->showNormal();
}

void  ZPMainFrame::closeEvent(QCloseEvent* event)
{
    if (this->isVisible())
	{
		this->hide();
        this->m_iconTray->showMessage(tr("Server still running"),
									  tr("If you want to terminate server, just using exit Toolbar button."));
        event->ignore();
	}
	else
	{
        event->accept();
        this->m_iconTray->showMessage(tr("Server is going to closed"),
									  tr("Waiting for all unfinished progress..."));
	}
}

void ZPMainFrame::on_actionExit_triggered()
{
	this->hide();
    this->m_iconTray->setContextMenu(nullptr);
	this->close();
}

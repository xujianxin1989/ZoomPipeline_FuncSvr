#ifndef ZPMAINFRAME_H
#define ZPMAINFRAME_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QLabel>
#include <QMap>
#include <QMenu>
#include <QSystemTrayIcon>
#include "network/zp_net_threadpool.h"
#include "pipeline/zp_pipeline.h"
#include "smartlink/st_client_table.h"
#include "database/databaseresource.h"
#include "cluster/zp_clusterterm.h"
#include "logger/st_logger.h"

namespace Ui
{
	class ZPMainFrame;
}

class ZPMainFrame : public QMainWindow
{
	Q_OBJECT

public:
    explicit ZPMainFrame(QWidget *parent = nullptr);
	~ZPMainFrame();

    void timerEvent(QTimerEvent *event) override;
	//Auto start support
    void loadSettingsAndForkServer(const QString &configFile);
    void setLogger(STMsgLogger::st_logger *pLogger);

protected:
    void changeEvent(QEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

    QStandardItemModel *m_pMsgModelNetwork;
    QStandardItemModel *m_pMsgModelCluster;
    QStandardItemModel *m_pMsgModelDatabase;
    QStandardItemModel *m_pMsgModelSmartlink;

	//Config File Name
	QString m_currentConfigFile;
	//Listeners settings
    QStandardItemModel *m_pListenerModel;
	QSet<QString> m_set_listenerNames;
	//Database settings
    QStandardItemModel *m_pDbResModel;
    QMap<QString, QString> m_set_DbResNames;

	//Dead check
	int m_nTimerCheck;
    QStandardItemModel *m_pModelCluster;
	//Logger
    STMsgLogger::st_logger *m_pLogger;

private:
    Ui::ZPMainFrame *ui;
    QSystemTrayIcon *m_iconTray;
    QMenu *m_trayMenu;
    QLabel *m_statusLabel;

    ZPNetwork::zp_net_Engine *m_netEngine;
    ZPTaskEngine::zp_pipeline *m_taskEngine;
    ExampleServer::st_client_table *m_clientTable;
    ZPDatabase::DatabaseResource* m_pDatabases;
    ZP_Cluster::zp_ClusterTerm* m_pClusterTerm;

	//Cluster Reconnection
	QDateTime m_dtmLastClusterJoin;
	int m_nTimerId;
	void initUI();
    void loadSettings(QString config_file);
    void saveSettings(QString config_file);
    void forkServer(QString config_file);

protected slots:
    void on_evt_MessageNetwork(QObject* pSource, QString);
    void on_evt_Message_Cluster(QObject* pSource, QString);
    void on_evt_Message_Database(QObject* pSource, QString);
    void on_evt_Message_Smartlink(QObject* pSource, QString);
    //The socket error message
    void on_evt_SocketError(QObject* senderSock, QAbstractSocket::SocketError socketError);
	//The socket error message
    void on_evt_SocketError_Cluster(QObject* senderSock, QAbstractSocket::SocketError socketError);

public slots:
	void on_action_Start_Stop_triggered(bool);
	void on_action_About_triggered();
	void on_actionReload_config_file_triggered();
	void on_actionShow_Window_triggered();
	void on_actionExit_triggered();
	void on_pushButton_addListener_clicked();
	void on_pushButton_delListener_clicked();
	void on_pushButton_listerner_apply_clicked();
	void on_pushButton_threadsApply_clicked();
	void on_pushButton_db_add_clicked();
	void on_pushButton_db_del_clicked();
	void on_pushButton_db_apply_clicked();
	void on_pushButton_cluster_apply_clicked();
	void on_pushButton_smartlink_save_clicked();
    void on_pushButton_join_clicked();
};

#endif // ZPMAINFRAME_H

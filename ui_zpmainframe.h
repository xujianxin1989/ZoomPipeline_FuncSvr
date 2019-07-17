/********************************************************************************
** Form generated from reading UI file 'zpmainframe.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ZPMAINFRAME_H
#define UI_ZPMAINFRAME_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDial>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ZPMainFrame
{
public:
    QAction *actionExit;
    QAction *action_Start_Stop;
    QAction *action_About;
    QAction *actionReload_config_file;
    QAction *actionShow_Window;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    QTabWidget *tabWidget;
    QWidget *tab_network;
    QVBoxLayout *verticalLayout;
    QPlainTextEdit *plainTextEdit_status_net;
    QWidget *tab_settings;
    QVBoxLayout *verticalLayout_2;
    QTabWidget *tabWidget_Smartlink;
    QWidget *tab_set_network;
    QVBoxLayout *verticalLayout_3;
    QTableView *tableView_listen;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_2;
    QLineEdit *lineEdit_listenerName;
    QLabel *label_3;
    QLineEdit *lineEdit_listenerAddr;
    QLabel *label_4;
    QLineEdit *lineEdit_listenerPort;
    QCheckBox *checkBox_listener_ssl;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton_addListener;
    QPushButton *pushButton_delListener;
    QPushButton *pushButton_listerner_apply;
    QWidget *tab_taskPools;
    QVBoxLayout *verticalLayout_9;
    QGridLayout *gridLayout;
    QLabel *label_5;
    QDial *dial_plain_trans_threads;
    QProgressBar *progressBar_plain_threads;
    QLabel *label_6;
    QDial *dial_ssl_trans_threads;
    QProgressBar *progressBar_ssl_threads;
    QLabel *label_7;
    QDial *dial_task_working_threads;
    QProgressBar *progressBar_working_threads;
    QHBoxLayout *horizontalLayout_20;
    QLabel *label_31;
    QSlider *horizontalSlider_rubbishCan;
    QLCDNumber *lcdNumber_rabbish;
    QSpacerItem *horizontalSpacer_13;
    QPushButton *pushButton_threadsApply;
    QWidget *tab_set_db;
    QVBoxLayout *verticalLayout_4;
    QTableView *tableView_dbconn;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_9;
    QLineEdit *lineEdit_db_name;
    QLabel *label_10;
    QComboBox *comboBox_db_type;
    QSpacerItem *horizontalSpacer_2;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_11;
    QLineEdit *lineEdit_db_addr;
    QLabel *label_12;
    QLineEdit *lineEdit_db_port;
    QLabel *label_13;
    QLineEdit *lineEdit_db_schema;
    QSpacerItem *horizontalSpacer_3;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_14;
    QLineEdit *lineEdit_db_user;
    QLabel *label_15;
    QLineEdit *lineEdit_db_pass;
    QSpacerItem *horizontalSpacer_4;
    QHBoxLayout *horizontalLayout_8;
    QSpacerItem *horizontalSpacer_5;
    QPushButton *pushButton_db_add;
    QPushButton *pushButton_db_del;
    QPushButton *pushButton_db_apply;
    QWidget *tab_cluster;
    QVBoxLayout *verticalLayout_6;
    QHBoxLayout *horizontalLayout_12;
    QLabel *label_23;
    QLineEdit *lineEdit_cluster_pub_name;
    QLabel *label_20;
    QLineEdit *lineEdit_cluster_term_addr;
    QLabel *label_19;
    QLineEdit *lineEdit_cluster_term_port;
    QSpacerItem *horizontalSpacer_10;
    QSpacerItem *horizontalSpacer_6;
    QHBoxLayout *horizontalLayout_16;
    QLabel *label_24;
    QSlider *horizontalSlider_cluster_transThreads;
    QLabel *label_25;
    QSlider *horizontalSlider_cluster_workingThread;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout_14;
    QTableView *tableView_activeTerms;
    QHBoxLayout *horizontalLayout_13;
    QLabel *label_21;
    QLineEdit *lineEdit_cluster_LAN_Addr;
    QLabel *label_22;
    QLineEdit *lineEdit_cluster_LAN_Port;
    QSpacerItem *horizontalSpacer_7;
    QPushButton *pushButton_cluster_apply;
    QHBoxLayout *horizontalLayout_17;
    QLabel *label_27;
    QLineEdit *lineEdit_cluster_pub_Addr;
    QLabel *label_26;
    QLineEdit *lineEdit_cluster_pub_Port;
    QSpacerItem *horizontalSpacer_9;
    QPushButton *pushButton_join;
    QWidget *tab;
    QVBoxLayout *verticalLayout_5;
    QHBoxLayout *horizontalLayout_9;
    QLabel *label;
    QSlider *horizontalSlider_heartbeating;
    QLCDNumber *lcdNumber_heartbeating;
    QLabel *label_8;
    QHBoxLayout *horizontalLayout_10;
    QLabel *label_16;
    QLineEdit *lineEdit_SL_DB_Account;
    QLabel *label_17;
    QLineEdit *lineEdit_SL_DB_ME;
    QHBoxLayout *horizontalLayout_11;
    QLabel *label_18;
    QSpinBox *spinBox_cluster_max_payload;
    QSpacerItem *horizontalSpacer_11;
    QSpacerItem *verticalSpacer_2;
    QHBoxLayout *horizontalLayout_15;
    QSpacerItem *horizontalSpacer_8;
    QPushButton *pushButton_smartlink_save;
    QWidget *tab_task;
    QHBoxLayout *horizontalLayout_18;
    QTabWidget *tabWidget_msgs;
    QWidget *tab_msg_nodes;
    QHBoxLayout *horizontalLayout_2;
    QListView *listView_msg;
    QWidget *tab_msg_db;
    QVBoxLayout *verticalLayout_7;
    QListView *listView_msg_database;
    QWidget *tab_msg_cluster;
    QHBoxLayout *horizontalLayout_19;
    QListView *listView_msg_cluster;
    QWidget *tab_msg_smartlink;
    QVBoxLayout *verticalLayout_8;
    QListView *listView_msg_smartlink;
    QMenuBar *menuBar;
    QMenu *menu_Control;
    QMenu *menu_Help;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *ZPMainFrame)
    {
        if (ZPMainFrame->objectName().isEmpty())
            ZPMainFrame->setObjectName(QStringLiteral("ZPMainFrame"));
        ZPMainFrame->resize(640, 500);
        ZPMainFrame->setToolTipDuration(-1);
        ZPMainFrame->setIconSize(QSize(16, 16));
        ZPMainFrame->setToolButtonStyle(Qt::ToolButtonIconOnly);
        ZPMainFrame->setAnimated(true);
        ZPMainFrame->setTabShape(QTabWidget::Rounded);
        actionExit = new QAction(ZPMainFrame);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        action_Start_Stop = new QAction(ZPMainFrame);
        action_Start_Stop->setObjectName(QStringLiteral("action_Start_Stop"));
        action_Start_Stop->setCheckable(true);
        QIcon icon;
        icon.addFile(QStringLiteral(":/icons/Resources/cn11.png"), QSize(), QIcon::Normal, QIcon::Off);
        icon.addFile(QStringLiteral(":/icons/Resources/cn6.png"), QSize(), QIcon::Normal, QIcon::On);
        action_Start_Stop->setIcon(icon);
        action_About = new QAction(ZPMainFrame);
        action_About->setObjectName(QStringLiteral("action_About"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/icons/Resources/4013.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_About->setIcon(icon1);
        actionReload_config_file = new QAction(ZPMainFrame);
        actionReload_config_file->setObjectName(QStringLiteral("actionReload_config_file"));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/icons/Resources/Color, Lime.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionReload_config_file->setIcon(icon2);
        actionShow_Window = new QAction(ZPMainFrame);
        actionShow_Window->setObjectName(QStringLiteral("actionShow_Window"));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/icons/Resources/Color, Bondi.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionShow_Window->setIcon(icon3);
        centralWidget = new QWidget(ZPMainFrame);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tab_network = new QWidget();
        tab_network->setObjectName(QStringLiteral("tab_network"));
        verticalLayout = new QVBoxLayout(tab_network);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        plainTextEdit_status_net = new QPlainTextEdit(tab_network);
        plainTextEdit_status_net->setObjectName(QStringLiteral("plainTextEdit_status_net"));

        verticalLayout->addWidget(plainTextEdit_status_net);

        QIcon icon4;
        icon4.addFile(QStringLiteral(":/icons/Resources/Alienware (27).png"), QSize(), QIcon::Normal, QIcon::Off);
        tabWidget->addTab(tab_network, icon4, QString());
        tab_settings = new QWidget();
        tab_settings->setObjectName(QStringLiteral("tab_settings"));
        verticalLayout_2 = new QVBoxLayout(tab_settings);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        tabWidget_Smartlink = new QTabWidget(tab_settings);
        tabWidget_Smartlink->setObjectName(QStringLiteral("tabWidget_Smartlink"));
        tabWidget_Smartlink->setTabPosition(QTabWidget::South);
        tabWidget_Smartlink->setTabShape(QTabWidget::Triangular);
        tab_set_network = new QWidget();
        tab_set_network->setObjectName(QStringLiteral("tab_set_network"));
        verticalLayout_3 = new QVBoxLayout(tab_set_network);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        tableView_listen = new QTableView(tab_set_network);
        tableView_listen->setObjectName(QStringLiteral("tableView_listen"));

        verticalLayout_3->addWidget(tableView_listen);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        label_2 = new QLabel(tab_set_network);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout_4->addWidget(label_2);

        lineEdit_listenerName = new QLineEdit(tab_set_network);
        lineEdit_listenerName->setObjectName(QStringLiteral("lineEdit_listenerName"));

        horizontalLayout_4->addWidget(lineEdit_listenerName);

        label_3 = new QLabel(tab_set_network);
        label_3->setObjectName(QStringLiteral("label_3"));

        horizontalLayout_4->addWidget(label_3);

        lineEdit_listenerAddr = new QLineEdit(tab_set_network);
        lineEdit_listenerAddr->setObjectName(QStringLiteral("lineEdit_listenerAddr"));

        horizontalLayout_4->addWidget(lineEdit_listenerAddr);

        label_4 = new QLabel(tab_set_network);
        label_4->setObjectName(QStringLiteral("label_4"));

        horizontalLayout_4->addWidget(label_4);

        lineEdit_listenerPort = new QLineEdit(tab_set_network);
        lineEdit_listenerPort->setObjectName(QStringLiteral("lineEdit_listenerPort"));

        horizontalLayout_4->addWidget(lineEdit_listenerPort);

        checkBox_listener_ssl = new QCheckBox(tab_set_network);
        checkBox_listener_ssl->setObjectName(QStringLiteral("checkBox_listener_ssl"));

        horizontalLayout_4->addWidget(checkBox_listener_ssl);


        verticalLayout_3->addLayout(horizontalLayout_4);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);

        pushButton_addListener = new QPushButton(tab_set_network);
        pushButton_addListener->setObjectName(QStringLiteral("pushButton_addListener"));

        horizontalLayout_3->addWidget(pushButton_addListener);

        pushButton_delListener = new QPushButton(tab_set_network);
        pushButton_delListener->setObjectName(QStringLiteral("pushButton_delListener"));

        horizontalLayout_3->addWidget(pushButton_delListener);

        pushButton_listerner_apply = new QPushButton(tab_set_network);
        pushButton_listerner_apply->setObjectName(QStringLiteral("pushButton_listerner_apply"));
        QIcon icon5;
        icon5.addFile(QStringLiteral(":/icons/Resources/3floppy_mount-2.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_listerner_apply->setIcon(icon5);

        horizontalLayout_3->addWidget(pushButton_listerner_apply);


        verticalLayout_3->addLayout(horizontalLayout_3);

        QIcon icon6;
        icon6.addFile(QStringLiteral(":/icons/Resources/4003.png"), QSize(), QIcon::Normal, QIcon::Off);
        tabWidget_Smartlink->addTab(tab_set_network, icon6, QString());
        tab_taskPools = new QWidget();
        tab_taskPools->setObjectName(QStringLiteral("tab_taskPools"));
        verticalLayout_9 = new QVBoxLayout(tab_taskPools);
        verticalLayout_9->setSpacing(6);
        verticalLayout_9->setContentsMargins(11, 11, 11, 11);
        verticalLayout_9->setObjectName(QStringLiteral("verticalLayout_9"));
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        label_5 = new QLabel(tab_taskPools);
        label_5->setObjectName(QStringLiteral("label_5"));

        gridLayout->addWidget(label_5, 0, 0, 1, 1);

        dial_plain_trans_threads = new QDial(tab_taskPools);
        dial_plain_trans_threads->setObjectName(QStringLiteral("dial_plain_trans_threads"));
        dial_plain_trans_threads->setMinimum(1);
        dial_plain_trans_threads->setMaximum(64);
        dial_plain_trans_threads->setPageStep(8);
        dial_plain_trans_threads->setNotchesVisible(true);

        gridLayout->addWidget(dial_plain_trans_threads, 0, 1, 1, 1);

        progressBar_plain_threads = new QProgressBar(tab_taskPools);
        progressBar_plain_threads->setObjectName(QStringLiteral("progressBar_plain_threads"));
        progressBar_plain_threads->setMinimum(1);
        progressBar_plain_threads->setMaximum(64);
        progressBar_plain_threads->setValue(1);
        progressBar_plain_threads->setInvertedAppearance(false);

        gridLayout->addWidget(progressBar_plain_threads, 0, 2, 1, 1);

        label_6 = new QLabel(tab_taskPools);
        label_6->setObjectName(QStringLiteral("label_6"));

        gridLayout->addWidget(label_6, 1, 0, 1, 1);

        dial_ssl_trans_threads = new QDial(tab_taskPools);
        dial_ssl_trans_threads->setObjectName(QStringLiteral("dial_ssl_trans_threads"));
        dial_ssl_trans_threads->setMinimum(1);
        dial_ssl_trans_threads->setMaximum(64);
        dial_ssl_trans_threads->setPageStep(8);
        dial_ssl_trans_threads->setWrapping(false);
        dial_ssl_trans_threads->setNotchTarget(4);
        dial_ssl_trans_threads->setNotchesVisible(true);

        gridLayout->addWidget(dial_ssl_trans_threads, 1, 1, 1, 1);

        progressBar_ssl_threads = new QProgressBar(tab_taskPools);
        progressBar_ssl_threads->setObjectName(QStringLiteral("progressBar_ssl_threads"));
        progressBar_ssl_threads->setMinimum(1);
        progressBar_ssl_threads->setMaximum(64);
        progressBar_ssl_threads->setValue(1);

        gridLayout->addWidget(progressBar_ssl_threads, 1, 2, 1, 1);

        label_7 = new QLabel(tab_taskPools);
        label_7->setObjectName(QStringLiteral("label_7"));

        gridLayout->addWidget(label_7, 2, 0, 1, 1);

        dial_task_working_threads = new QDial(tab_taskPools);
        dial_task_working_threads->setObjectName(QStringLiteral("dial_task_working_threads"));
        dial_task_working_threads->setMinimum(1);
        dial_task_working_threads->setMaximum(256);
        dial_task_working_threads->setPageStep(16);
        dial_task_working_threads->setNotchTarget(8);
        dial_task_working_threads->setNotchesVisible(true);

        gridLayout->addWidget(dial_task_working_threads, 2, 1, 1, 1);

        progressBar_working_threads = new QProgressBar(tab_taskPools);
        progressBar_working_threads->setObjectName(QStringLiteral("progressBar_working_threads"));
        progressBar_working_threads->setMinimum(1);
        progressBar_working_threads->setMaximum(256);
        progressBar_working_threads->setValue(1);

        gridLayout->addWidget(progressBar_working_threads, 2, 2, 1, 1);


        verticalLayout_9->addLayout(gridLayout);

        horizontalLayout_20 = new QHBoxLayout();
        horizontalLayout_20->setSpacing(6);
        horizontalLayout_20->setObjectName(QStringLiteral("horizontalLayout_20"));
        label_31 = new QLabel(tab_taskPools);
        label_31->setObjectName(QStringLiteral("label_31"));

        horizontalLayout_20->addWidget(label_31);

        horizontalSlider_rubbishCan = new QSlider(tab_taskPools);
        horizontalSlider_rubbishCan->setObjectName(QStringLiteral("horizontalSlider_rubbishCan"));
        horizontalSlider_rubbishCan->setMinimum(16);
        horizontalSlider_rubbishCan->setMaximum(65536);
        horizontalSlider_rubbishCan->setOrientation(Qt::Horizontal);
        horizontalSlider_rubbishCan->setTickPosition(QSlider::NoTicks);
        horizontalSlider_rubbishCan->setTickInterval(256);

        horizontalLayout_20->addWidget(horizontalSlider_rubbishCan);

        lcdNumber_rabbish = new QLCDNumber(tab_taskPools);
        lcdNumber_rabbish->setObjectName(QStringLiteral("lcdNumber_rabbish"));
        lcdNumber_rabbish->setSegmentStyle(QLCDNumber::Flat);

        horizontalLayout_20->addWidget(lcdNumber_rabbish);

        horizontalSpacer_13 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_20->addItem(horizontalSpacer_13);

        pushButton_threadsApply = new QPushButton(tab_taskPools);
        pushButton_threadsApply->setObjectName(QStringLiteral("pushButton_threadsApply"));
        QIcon icon7;
        icon7.addFile(QStringLiteral(":/icons/Resources/3floppy_mount-1.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_threadsApply->setIcon(icon7);

        horizontalLayout_20->addWidget(pushButton_threadsApply);


        verticalLayout_9->addLayout(horizontalLayout_20);

        QIcon icon8;
        icon8.addFile(QStringLiteral(":/icons/Resources/27_Martin.png"), QSize(), QIcon::Normal, QIcon::Off);
        tabWidget_Smartlink->addTab(tab_taskPools, icon8, QString());
        tab_set_db = new QWidget();
        tab_set_db->setObjectName(QStringLiteral("tab_set_db"));
        verticalLayout_4 = new QVBoxLayout(tab_set_db);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        tableView_dbconn = new QTableView(tab_set_db);
        tableView_dbconn->setObjectName(QStringLiteral("tableView_dbconn"));

        verticalLayout_4->addWidget(tableView_dbconn);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        label_9 = new QLabel(tab_set_db);
        label_9->setObjectName(QStringLiteral("label_9"));

        horizontalLayout_5->addWidget(label_9);

        lineEdit_db_name = new QLineEdit(tab_set_db);
        lineEdit_db_name->setObjectName(QStringLiteral("lineEdit_db_name"));

        horizontalLayout_5->addWidget(lineEdit_db_name);

        label_10 = new QLabel(tab_set_db);
        label_10->setObjectName(QStringLiteral("label_10"));

        horizontalLayout_5->addWidget(label_10);

        comboBox_db_type = new QComboBox(tab_set_db);
        comboBox_db_type->setObjectName(QStringLiteral("comboBox_db_type"));

        horizontalLayout_5->addWidget(comboBox_db_type);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_2);


        verticalLayout_4->addLayout(horizontalLayout_5);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        label_11 = new QLabel(tab_set_db);
        label_11->setObjectName(QStringLiteral("label_11"));

        horizontalLayout_6->addWidget(label_11);

        lineEdit_db_addr = new QLineEdit(tab_set_db);
        lineEdit_db_addr->setObjectName(QStringLiteral("lineEdit_db_addr"));

        horizontalLayout_6->addWidget(lineEdit_db_addr);

        label_12 = new QLabel(tab_set_db);
        label_12->setObjectName(QStringLiteral("label_12"));

        horizontalLayout_6->addWidget(label_12);

        lineEdit_db_port = new QLineEdit(tab_set_db);
        lineEdit_db_port->setObjectName(QStringLiteral("lineEdit_db_port"));

        horizontalLayout_6->addWidget(lineEdit_db_port);

        label_13 = new QLabel(tab_set_db);
        label_13->setObjectName(QStringLiteral("label_13"));

        horizontalLayout_6->addWidget(label_13);

        lineEdit_db_schema = new QLineEdit(tab_set_db);
        lineEdit_db_schema->setObjectName(QStringLiteral("lineEdit_db_schema"));

        horizontalLayout_6->addWidget(lineEdit_db_schema);

        horizontalSpacer_3 = new QSpacerItem(37, 17, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_3);


        verticalLayout_4->addLayout(horizontalLayout_6);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        label_14 = new QLabel(tab_set_db);
        label_14->setObjectName(QStringLiteral("label_14"));

        horizontalLayout_7->addWidget(label_14);

        lineEdit_db_user = new QLineEdit(tab_set_db);
        lineEdit_db_user->setObjectName(QStringLiteral("lineEdit_db_user"));

        horizontalLayout_7->addWidget(lineEdit_db_user);

        label_15 = new QLabel(tab_set_db);
        label_15->setObjectName(QStringLiteral("label_15"));

        horizontalLayout_7->addWidget(label_15);

        lineEdit_db_pass = new QLineEdit(tab_set_db);
        lineEdit_db_pass->setObjectName(QStringLiteral("lineEdit_db_pass"));
        lineEdit_db_pass->setEchoMode(QLineEdit::PasswordEchoOnEdit);

        horizontalLayout_7->addWidget(lineEdit_db_pass);

        horizontalSpacer_4 = new QSpacerItem(37, 17, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_4);


        verticalLayout_4->addLayout(horizontalLayout_7);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setSpacing(6);
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        horizontalSpacer_5 = new QSpacerItem(34, 14, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_5);

        pushButton_db_add = new QPushButton(tab_set_db);
        pushButton_db_add->setObjectName(QStringLiteral("pushButton_db_add"));

        horizontalLayout_8->addWidget(pushButton_db_add);

        pushButton_db_del = new QPushButton(tab_set_db);
        pushButton_db_del->setObjectName(QStringLiteral("pushButton_db_del"));

        horizontalLayout_8->addWidget(pushButton_db_del);

        pushButton_db_apply = new QPushButton(tab_set_db);
        pushButton_db_apply->setObjectName(QStringLiteral("pushButton_db_apply"));
        QIcon icon9;
        icon9.addFile(QStringLiteral(":/icons/Resources/Color, Sage.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_db_apply->setIcon(icon9);

        horizontalLayout_8->addWidget(pushButton_db_apply);


        verticalLayout_4->addLayout(horizontalLayout_8);

        QIcon icon10;
        icon10.addFile(QStringLiteral(":/icons/Resources/Color Classic.png"), QSize(), QIcon::Normal, QIcon::Off);
        tabWidget_Smartlink->addTab(tab_set_db, icon10, QString());
        tab_cluster = new QWidget();
        tab_cluster->setObjectName(QStringLiteral("tab_cluster"));
        verticalLayout_6 = new QVBoxLayout(tab_cluster);
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setContentsMargins(11, 11, 11, 11);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setSpacing(6);
        horizontalLayout_12->setObjectName(QStringLiteral("horizontalLayout_12"));
        label_23 = new QLabel(tab_cluster);
        label_23->setObjectName(QStringLiteral("label_23"));

        horizontalLayout_12->addWidget(label_23);

        lineEdit_cluster_pub_name = new QLineEdit(tab_cluster);
        lineEdit_cluster_pub_name->setObjectName(QStringLiteral("lineEdit_cluster_pub_name"));

        horizontalLayout_12->addWidget(lineEdit_cluster_pub_name);

        label_20 = new QLabel(tab_cluster);
        label_20->setObjectName(QStringLiteral("label_20"));

        horizontalLayout_12->addWidget(label_20);

        lineEdit_cluster_term_addr = new QLineEdit(tab_cluster);
        lineEdit_cluster_term_addr->setObjectName(QStringLiteral("lineEdit_cluster_term_addr"));

        horizontalLayout_12->addWidget(lineEdit_cluster_term_addr);

        label_19 = new QLabel(tab_cluster);
        label_19->setObjectName(QStringLiteral("label_19"));

        horizontalLayout_12->addWidget(label_19);

        lineEdit_cluster_term_port = new QLineEdit(tab_cluster);
        lineEdit_cluster_term_port->setObjectName(QStringLiteral("lineEdit_cluster_term_port"));

        horizontalLayout_12->addWidget(lineEdit_cluster_term_port);

        horizontalSpacer_10 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_12->addItem(horizontalSpacer_10);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_12->addItem(horizontalSpacer_6);


        verticalLayout_6->addLayout(horizontalLayout_12);

        horizontalLayout_16 = new QHBoxLayout();
        horizontalLayout_16->setSpacing(6);
        horizontalLayout_16->setObjectName(QStringLiteral("horizontalLayout_16"));
        label_24 = new QLabel(tab_cluster);
        label_24->setObjectName(QStringLiteral("label_24"));

        horizontalLayout_16->addWidget(label_24);

        horizontalSlider_cluster_transThreads = new QSlider(tab_cluster);
        horizontalSlider_cluster_transThreads->setObjectName(QStringLiteral("horizontalSlider_cluster_transThreads"));
        horizontalSlider_cluster_transThreads->setMinimum(1);
        horizontalSlider_cluster_transThreads->setMaximum(16);
        horizontalSlider_cluster_transThreads->setPageStep(1);
        horizontalSlider_cluster_transThreads->setOrientation(Qt::Horizontal);
        horizontalSlider_cluster_transThreads->setTickPosition(QSlider::TicksAbove);

        horizontalLayout_16->addWidget(horizontalSlider_cluster_transThreads);

        label_25 = new QLabel(tab_cluster);
        label_25->setObjectName(QStringLiteral("label_25"));

        horizontalLayout_16->addWidget(label_25);

        horizontalSlider_cluster_workingThread = new QSlider(tab_cluster);
        horizontalSlider_cluster_workingThread->setObjectName(QStringLiteral("horizontalSlider_cluster_workingThread"));
        horizontalSlider_cluster_workingThread->setMinimum(1);
        horizontalSlider_cluster_workingThread->setMaximum(16);
        horizontalSlider_cluster_workingThread->setPageStep(1);
        horizontalSlider_cluster_workingThread->setOrientation(Qt::Horizontal);
        horizontalSlider_cluster_workingThread->setTickPosition(QSlider::TicksAbove);

        horizontalLayout_16->addWidget(horizontalSlider_cluster_workingThread);


        verticalLayout_6->addLayout(horizontalLayout_16);

        groupBox = new QGroupBox(tab_cluster);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        horizontalLayout_14 = new QHBoxLayout(groupBox);
        horizontalLayout_14->setSpacing(6);
        horizontalLayout_14->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_14->setObjectName(QStringLiteral("horizontalLayout_14"));
        tableView_activeTerms = new QTableView(groupBox);
        tableView_activeTerms->setObjectName(QStringLiteral("tableView_activeTerms"));
        tableView_activeTerms->setAlternatingRowColors(true);

        horizontalLayout_14->addWidget(tableView_activeTerms);


        verticalLayout_6->addWidget(groupBox);

        horizontalLayout_13 = new QHBoxLayout();
        horizontalLayout_13->setSpacing(6);
        horizontalLayout_13->setObjectName(QStringLiteral("horizontalLayout_13"));
        label_21 = new QLabel(tab_cluster);
        label_21->setObjectName(QStringLiteral("label_21"));

        horizontalLayout_13->addWidget(label_21);

        lineEdit_cluster_LAN_Addr = new QLineEdit(tab_cluster);
        lineEdit_cluster_LAN_Addr->setObjectName(QStringLiteral("lineEdit_cluster_LAN_Addr"));

        horizontalLayout_13->addWidget(lineEdit_cluster_LAN_Addr);

        label_22 = new QLabel(tab_cluster);
        label_22->setObjectName(QStringLiteral("label_22"));

        horizontalLayout_13->addWidget(label_22);

        lineEdit_cluster_LAN_Port = new QLineEdit(tab_cluster);
        lineEdit_cluster_LAN_Port->setObjectName(QStringLiteral("lineEdit_cluster_LAN_Port"));

        horizontalLayout_13->addWidget(lineEdit_cluster_LAN_Port);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_13->addItem(horizontalSpacer_7);

        pushButton_cluster_apply = new QPushButton(tab_cluster);
        pushButton_cluster_apply->setObjectName(QStringLiteral("pushButton_cluster_apply"));

        horizontalLayout_13->addWidget(pushButton_cluster_apply);


        verticalLayout_6->addLayout(horizontalLayout_13);

        horizontalLayout_17 = new QHBoxLayout();
        horizontalLayout_17->setSpacing(6);
        horizontalLayout_17->setObjectName(QStringLiteral("horizontalLayout_17"));
        label_27 = new QLabel(tab_cluster);
        label_27->setObjectName(QStringLiteral("label_27"));

        horizontalLayout_17->addWidget(label_27);

        lineEdit_cluster_pub_Addr = new QLineEdit(tab_cluster);
        lineEdit_cluster_pub_Addr->setObjectName(QStringLiteral("lineEdit_cluster_pub_Addr"));

        horizontalLayout_17->addWidget(lineEdit_cluster_pub_Addr);

        label_26 = new QLabel(tab_cluster);
        label_26->setObjectName(QStringLiteral("label_26"));

        horizontalLayout_17->addWidget(label_26);

        lineEdit_cluster_pub_Port = new QLineEdit(tab_cluster);
        lineEdit_cluster_pub_Port->setObjectName(QStringLiteral("lineEdit_cluster_pub_Port"));

        horizontalLayout_17->addWidget(lineEdit_cluster_pub_Port);

        horizontalSpacer_9 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_17->addItem(horizontalSpacer_9);

        pushButton_join = new QPushButton(tab_cluster);
        pushButton_join->setObjectName(QStringLiteral("pushButton_join"));

        horizontalLayout_17->addWidget(pushButton_join);


        verticalLayout_6->addLayout(horizontalLayout_17);

        QIcon icon11;
        icon11.addFile(QStringLiteral(":/icons/Resources/hanukkah_03.png"), QSize(), QIcon::Normal, QIcon::Off);
        tabWidget_Smartlink->addTab(tab_cluster, icon11, QString());
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        verticalLayout_5 = new QVBoxLayout(tab);
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setContentsMargins(11, 11, 11, 11);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setSpacing(6);
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        label = new QLabel(tab);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout_9->addWidget(label);

        horizontalSlider_heartbeating = new QSlider(tab);
        horizontalSlider_heartbeating->setObjectName(QStringLiteral("horizontalSlider_heartbeating"));
        horizontalSlider_heartbeating->setMinimum(60);
        horizontalSlider_heartbeating->setMaximum(300);
        horizontalSlider_heartbeating->setValue(180);
        horizontalSlider_heartbeating->setOrientation(Qt::Horizontal);
        horizontalSlider_heartbeating->setTickPosition(QSlider::TicksBelow);

        horizontalLayout_9->addWidget(horizontalSlider_heartbeating);

        lcdNumber_heartbeating = new QLCDNumber(tab);
        lcdNumber_heartbeating->setObjectName(QStringLiteral("lcdNumber_heartbeating"));
        lcdNumber_heartbeating->setFrameShape(QFrame::Panel);
        lcdNumber_heartbeating->setFrameShadow(QFrame::Plain);
        lcdNumber_heartbeating->setSegmentStyle(QLCDNumber::Flat);

        horizontalLayout_9->addWidget(lcdNumber_heartbeating);

        label_8 = new QLabel(tab);
        label_8->setObjectName(QStringLiteral("label_8"));

        horizontalLayout_9->addWidget(label_8);


        verticalLayout_5->addLayout(horizontalLayout_9);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setSpacing(6);
        horizontalLayout_10->setObjectName(QStringLiteral("horizontalLayout_10"));
        label_16 = new QLabel(tab);
        label_16->setObjectName(QStringLiteral("label_16"));

        horizontalLayout_10->addWidget(label_16);

        lineEdit_SL_DB_Account = new QLineEdit(tab);
        lineEdit_SL_DB_Account->setObjectName(QStringLiteral("lineEdit_SL_DB_Account"));

        horizontalLayout_10->addWidget(lineEdit_SL_DB_Account);

        label_17 = new QLabel(tab);
        label_17->setObjectName(QStringLiteral("label_17"));

        horizontalLayout_10->addWidget(label_17);

        lineEdit_SL_DB_ME = new QLineEdit(tab);
        lineEdit_SL_DB_ME->setObjectName(QStringLiteral("lineEdit_SL_DB_ME"));

        horizontalLayout_10->addWidget(lineEdit_SL_DB_ME);


        verticalLayout_5->addLayout(horizontalLayout_10);

        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setSpacing(6);
        horizontalLayout_11->setObjectName(QStringLiteral("horizontalLayout_11"));
        label_18 = new QLabel(tab);
        label_18->setObjectName(QStringLiteral("label_18"));

        horizontalLayout_11->addWidget(label_18);

        spinBox_cluster_max_payload = new QSpinBox(tab);
        spinBox_cluster_max_payload->setObjectName(QStringLiteral("spinBox_cluster_max_payload"));
        spinBox_cluster_max_payload->setMinimum(1);
        spinBox_cluster_max_payload->setMaximum(16384);

        horizontalLayout_11->addWidget(spinBox_cluster_max_payload);

        horizontalSpacer_11 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_11->addItem(horizontalSpacer_11);


        verticalLayout_5->addLayout(horizontalLayout_11);

        verticalSpacer_2 = new QSpacerItem(20, 121, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_5->addItem(verticalSpacer_2);

        horizontalLayout_15 = new QHBoxLayout();
        horizontalLayout_15->setSpacing(6);
        horizontalLayout_15->setObjectName(QStringLiteral("horizontalLayout_15"));
        horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_15->addItem(horizontalSpacer_8);

        pushButton_smartlink_save = new QPushButton(tab);
        pushButton_smartlink_save->setObjectName(QStringLiteral("pushButton_smartlink_save"));

        horizontalLayout_15->addWidget(pushButton_smartlink_save);


        verticalLayout_5->addLayout(horizontalLayout_15);

        tabWidget_Smartlink->addTab(tab, icon4, QString());

        verticalLayout_2->addWidget(tabWidget_Smartlink);

        QIcon icon12;
        icon12.addFile(QStringLiteral(":/icons/Resources/19-1.png"), QSize(), QIcon::Normal, QIcon::Off);
        tabWidget->addTab(tab_settings, icon12, QString());
        tab_task = new QWidget();
        tab_task->setObjectName(QStringLiteral("tab_task"));
        horizontalLayout_18 = new QHBoxLayout(tab_task);
        horizontalLayout_18->setSpacing(6);
        horizontalLayout_18->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_18->setObjectName(QStringLiteral("horizontalLayout_18"));
        tabWidget_msgs = new QTabWidget(tab_task);
        tabWidget_msgs->setObjectName(QStringLiteral("tabWidget_msgs"));
        tabWidget_msgs->setTabPosition(QTabWidget::South);
        tabWidget_msgs->setTabShape(QTabWidget::Triangular);
        tab_msg_nodes = new QWidget();
        tab_msg_nodes->setObjectName(QStringLiteral("tab_msg_nodes"));
        horizontalLayout_2 = new QHBoxLayout(tab_msg_nodes);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        listView_msg = new QListView(tab_msg_nodes);
        listView_msg->setObjectName(QStringLiteral("listView_msg"));
        listView_msg->setContextMenuPolicy(Qt::DefaultContextMenu);

        horizontalLayout_2->addWidget(listView_msg);

        QIcon icon13;
        icon13.addFile(QStringLiteral(":/icons/Resources/019-1.png"), QSize(), QIcon::Normal, QIcon::Off);
        tabWidget_msgs->addTab(tab_msg_nodes, icon13, QString());
        tab_msg_db = new QWidget();
        tab_msg_db->setObjectName(QStringLiteral("tab_msg_db"));
        verticalLayout_7 = new QVBoxLayout(tab_msg_db);
        verticalLayout_7->setSpacing(6);
        verticalLayout_7->setContentsMargins(11, 11, 11, 11);
        verticalLayout_7->setObjectName(QStringLiteral("verticalLayout_7"));
        listView_msg_database = new QListView(tab_msg_db);
        listView_msg_database->setObjectName(QStringLiteral("listView_msg_database"));

        verticalLayout_7->addWidget(listView_msg_database);

        QIcon icon14;
        icon14.addFile(QStringLiteral(":/icons/Resources/Backup drive.png"), QSize(), QIcon::Normal, QIcon::Off);
        tabWidget_msgs->addTab(tab_msg_db, icon14, QString());
        tab_msg_cluster = new QWidget();
        tab_msg_cluster->setObjectName(QStringLiteral("tab_msg_cluster"));
        horizontalLayout_19 = new QHBoxLayout(tab_msg_cluster);
        horizontalLayout_19->setSpacing(6);
        horizontalLayout_19->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_19->setObjectName(QStringLiteral("horizontalLayout_19"));
        listView_msg_cluster = new QListView(tab_msg_cluster);
        listView_msg_cluster->setObjectName(QStringLiteral("listView_msg_cluster"));

        horizontalLayout_19->addWidget(listView_msg_cluster);

        tabWidget_msgs->addTab(tab_msg_cluster, icon11, QString());
        tab_msg_smartlink = new QWidget();
        tab_msg_smartlink->setObjectName(QStringLiteral("tab_msg_smartlink"));
        verticalLayout_8 = new QVBoxLayout(tab_msg_smartlink);
        verticalLayout_8->setSpacing(6);
        verticalLayout_8->setContentsMargins(11, 11, 11, 11);
        verticalLayout_8->setObjectName(QStringLiteral("verticalLayout_8"));
        listView_msg_smartlink = new QListView(tab_msg_smartlink);
        listView_msg_smartlink->setObjectName(QStringLiteral("listView_msg_smartlink"));

        verticalLayout_8->addWidget(listView_msg_smartlink);

        QIcon icon15;
        icon15.addFile(QStringLiteral(":/icons/Resources/+_Sign_Alt.png"), QSize(), QIcon::Normal, QIcon::Off);
        tabWidget_msgs->addTab(tab_msg_smartlink, icon15, QString());

        horizontalLayout_18->addWidget(tabWidget_msgs);

        tabWidget->addTab(tab_task, icon14, QString());

        horizontalLayout->addWidget(tabWidget);

        ZPMainFrame->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(ZPMainFrame);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 640, 23));
        menu_Control = new QMenu(menuBar);
        menu_Control->setObjectName(QStringLiteral("menu_Control"));
        menu_Help = new QMenu(menuBar);
        menu_Help->setObjectName(QStringLiteral("menu_Help"));
        ZPMainFrame->setMenuBar(menuBar);
        mainToolBar = new QToolBar(ZPMainFrame);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        mainToolBar->setMovable(true);
        mainToolBar->setAllowedAreas(Qt::AllToolBarAreas);
        mainToolBar->setFloatable(false);
        ZPMainFrame->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(ZPMainFrame);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        ZPMainFrame->setStatusBar(statusBar);

        menuBar->addAction(menu_Control->menuAction());
        menuBar->addAction(menu_Help->menuAction());
        menu_Control->addAction(action_Start_Stop);
        menu_Control->addAction(actionReload_config_file);
        menu_Control->addSeparator();
        menu_Control->addAction(actionExit);
        menu_Help->addAction(action_About);
        mainToolBar->addAction(action_Start_Stop);
        mainToolBar->addAction(actionReload_config_file);
        mainToolBar->addSeparator();
        mainToolBar->addAction(action_About);

        retranslateUi(ZPMainFrame);
        QObject::connect(dial_ssl_trans_threads, SIGNAL(valueChanged(int)), progressBar_ssl_threads, SLOT(setValue(int)));
        QObject::connect(dial_task_working_threads, SIGNAL(valueChanged(int)), progressBar_working_threads, SLOT(setValue(int)));
        QObject::connect(horizontalSlider_heartbeating, SIGNAL(valueChanged(int)), lcdNumber_heartbeating, SLOT(display(int)));
        QObject::connect(horizontalSlider_rubbishCan, SIGNAL(valueChanged(int)), lcdNumber_rabbish, SLOT(display(int)));
        QObject::connect(dial_plain_trans_threads, SIGNAL(valueChanged(int)), progressBar_plain_threads, SLOT(setValue(int)));

        tabWidget->setCurrentIndex(0);
        tabWidget_Smartlink->setCurrentIndex(2);
        tabWidget_msgs->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(ZPMainFrame);
    } // setupUi

    void retranslateUi(QMainWindow *ZPMainFrame)
    {
        ZPMainFrame->setWindowTitle(QApplication::translate("ZPMainFrame", "ZPMainFrame", Q_NULLPTR));
        actionExit->setText(QApplication::translate("ZPMainFrame", "E&xit", Q_NULLPTR));
        action_Start_Stop->setText(QApplication::translate("ZPMainFrame", "&Start/Stop", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        action_Start_Stop->setToolTip(QApplication::translate("ZPMainFrame", "Start or stop the server", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        action_About->setText(QApplication::translate("ZPMainFrame", "&About", Q_NULLPTR));
        actionReload_config_file->setText(QApplication::translate("ZPMainFrame", "&Reload config file", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        actionReload_config_file->setToolTip(QApplication::translate("ZPMainFrame", "Reload Config file", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        actionShow_Window->setText(QApplication::translate("ZPMainFrame", "Show&Window", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        actionShow_Window->setToolTip(QApplication::translate("ZPMainFrame", "Show main window", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        tabWidget->setTabText(tabWidget->indexOf(tab_network), QApplication::translate("ZPMainFrame", "Summary", Q_NULLPTR));
        label_2->setText(QApplication::translate("ZPMainFrame", "Name", Q_NULLPTR));
        label_3->setText(QApplication::translate("ZPMainFrame", "Addr", Q_NULLPTR));
        label_4->setText(QApplication::translate("ZPMainFrame", "Port", Q_NULLPTR));
        checkBox_listener_ssl->setText(QApplication::translate("ZPMainFrame", "SSL Connection", Q_NULLPTR));
        pushButton_addListener->setText(QApplication::translate("ZPMainFrame", "Add", Q_NULLPTR));
        pushButton_delListener->setText(QApplication::translate("ZPMainFrame", "Del", Q_NULLPTR));
        pushButton_listerner_apply->setText(QApplication::translate("ZPMainFrame", "Save", Q_NULLPTR));
        tabWidget_Smartlink->setTabText(tabWidget_Smartlink->indexOf(tab_set_network), QApplication::translate("ZPMainFrame", "Listeners", Q_NULLPTR));
        label_5->setText(QApplication::translate("ZPMainFrame", "Plain connection Trans Threads", Q_NULLPTR));
        progressBar_plain_threads->setFormat(QApplication::translate("ZPMainFrame", "%v", Q_NULLPTR));
        label_6->setText(QApplication::translate("ZPMainFrame", "SSL connection Trans Threads", Q_NULLPTR));
        progressBar_ssl_threads->setFormat(QApplication::translate("ZPMainFrame", "%v", Q_NULLPTR));
        label_7->setText(QApplication::translate("ZPMainFrame", "Working Task Threads", Q_NULLPTR));
        progressBar_working_threads->setFormat(QApplication::translate("ZPMainFrame", "%v", Q_NULLPTR));
        label_31->setText(QApplication::translate("ZPMainFrame", "Rabbish Can Items Size", Q_NULLPTR));
        pushButton_threadsApply->setText(QApplication::translate("ZPMainFrame", "Save", Q_NULLPTR));
        tabWidget_Smartlink->setTabText(tabWidget_Smartlink->indexOf(tab_taskPools), QApplication::translate("ZPMainFrame", "Task Pools", Q_NULLPTR));
        label_9->setText(QApplication::translate("ZPMainFrame", "Name", Q_NULLPTR));
        label_10->setText(QApplication::translate("ZPMainFrame", "Type", Q_NULLPTR));
        label_11->setText(QApplication::translate("ZPMainFrame", "Host Address", Q_NULLPTR));
        label_12->setText(QApplication::translate("ZPMainFrame", "Host Port", Q_NULLPTR));
        label_13->setText(QApplication::translate("ZPMainFrame", "Database Name", Q_NULLPTR));
        label_14->setText(QApplication::translate("ZPMainFrame", "User", Q_NULLPTR));
        label_15->setText(QApplication::translate("ZPMainFrame", "Password", Q_NULLPTR));
        pushButton_db_add->setText(QApplication::translate("ZPMainFrame", "Add", Q_NULLPTR));
        pushButton_db_del->setText(QApplication::translate("ZPMainFrame", "Remove", Q_NULLPTR));
        pushButton_db_apply->setText(QApplication::translate("ZPMainFrame", "Save", Q_NULLPTR));
        tabWidget_Smartlink->setTabText(tabWidget_Smartlink->indexOf(tab_set_db), QApplication::translate("ZPMainFrame", "Database", Q_NULLPTR));
        label_23->setText(QApplication::translate("ZPMainFrame", "Publish Name", Q_NULLPTR));
        label_20->setText(QApplication::translate("ZPMainFrame", "Terminal Listen Address", Q_NULLPTR));
        label_19->setText(QApplication::translate("ZPMainFrame", "Listen Port", Q_NULLPTR));
        label_24->setText(QApplication::translate("ZPMainFrame", "TransThreads", Q_NULLPTR));
        label_25->setText(QApplication::translate("ZPMainFrame", "Working Threads", Q_NULLPTR));
        groupBox->setTitle(QApplication::translate("ZPMainFrame", "Active Terminals", Q_NULLPTR));
        label_21->setText(QApplication::translate("ZPMainFrame", "LAN Address", Q_NULLPTR));
        label_22->setText(QApplication::translate("ZPMainFrame", "LAN Port", Q_NULLPTR));
        pushButton_cluster_apply->setText(QApplication::translate("ZPMainFrame", "&Save", Q_NULLPTR));
        label_27->setText(QApplication::translate("ZPMainFrame", "Publish Address", Q_NULLPTR));
        label_26->setText(QApplication::translate("ZPMainFrame", "Publish Port", Q_NULLPTR));
        pushButton_join->setText(QApplication::translate("ZPMainFrame", "Participate", Q_NULLPTR));
        tabWidget_Smartlink->setTabText(tabWidget_Smartlink->indexOf(tab_cluster), QApplication::translate("ZPMainFrame", "Cluster", Q_NULLPTR));
        label->setText(QApplication::translate("ZPMainFrame", "Heart-beating", Q_NULLPTR));
        label_8->setText(QApplication::translate("ZPMainFrame", "seconds", Q_NULLPTR));
        label_16->setText(QApplication::translate("ZPMainFrame", "User Account Database Name", Q_NULLPTR));
        label_17->setText(QApplication::translate("ZPMainFrame", "Main event db name", Q_NULLPTR));
        label_18->setText(QApplication::translate("ZPMainFrame", "Cluster balance max payload", Q_NULLPTR));
        pushButton_smartlink_save->setText(QApplication::translate("ZPMainFrame", "&Save", Q_NULLPTR));
        tabWidget_Smartlink->setTabText(tabWidget_Smartlink->indexOf(tab), QApplication::translate("ZPMainFrame", "Example Server", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab_settings), QApplication::translate("ZPMainFrame", "Settings", Q_NULLPTR));
        tabWidget_msgs->setTabText(tabWidget_msgs->indexOf(tab_msg_nodes), QApplication::translate("ZPMainFrame", "Nodes", Q_NULLPTR));
        tabWidget_msgs->setTabText(tabWidget_msgs->indexOf(tab_msg_db), QApplication::translate("ZPMainFrame", "Database", Q_NULLPTR));
        tabWidget_msgs->setTabText(tabWidget_msgs->indexOf(tab_msg_cluster), QApplication::translate("ZPMainFrame", "Cluster", Q_NULLPTR));
        tabWidget_msgs->setTabText(tabWidget_msgs->indexOf(tab_msg_smartlink), QApplication::translate("ZPMainFrame", "Smartlink", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab_task), QApplication::translate("ZPMainFrame", "Messages", Q_NULLPTR));
        menu_Control->setTitle(QApplication::translate("ZPMainFrame", "&Control", Q_NULLPTR));
        menu_Help->setTitle(QApplication::translate("ZPMainFrame", "&Help", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class ZPMainFrame: public Ui_ZPMainFrame {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ZPMAINFRAME_H

﻿#include "st_logger.h"
#include <QTextStream>

namespace STMsgLogger
{
    st_logger::st_logger(QObject* parent)
        : QObject(parent)
	{
		m_pLogFile = 0;
		m_bUseLogFile = true;
		m_nLogLevel = 2;
        m_nMaxFileSize = 268435456;
	}

	void st_logger::setMaxFileSize(int nSize)
	{
        if (nSize >= 1024 * 1024 && nSize <= 1024 * 1024 * 1024)
        {
            m_nMaxFileSize = nSize;
        }
	}

	int st_logger::maxFileSize() const
	{
		return m_nMaxFileSize;
	}

	int st_logger::logLevel() const
	{
		return m_nLogLevel;
	}

	void st_logger::setLogLevel(int lv)
	{
        if (lv >= 0 && lv <= 3)
        {
            m_nLogLevel = lv;
        }
	}

    bool st_logger::createNewLogFile(QCoreApplication* app)
	{
		bool res = false;
		QDateTime dtmCur = QDateTime::currentDateTime();
		m_currLogFileName = app->applicationDirPath() + "/Log/" + dtmCur.toString("yyyy_MM") + "/";
		QDir dir;
		dir.mkpath(m_currLogFileName);
		m_currLogFileName += dtmCur.toString("yyyy_MM_dd_HH_mm_ss_");
		m_currLogFileName += app->applicationName() + QString("(%1).txt").arg(app->applicationPid());
		if (m_pLogFile)
		{
            if (m_pLogFile->isOpen())
            {
                m_pLogFile->close();
            }
			m_pLogFile->deleteLater();
			m_pLogFile = 0;
		}
        m_pLogFile = new QFile(m_currLogFileName, this);
		if (m_pLogFile)
		{
            if (m_pLogFile->open(QIODevice::WriteOnly) == false)
			{
				m_pLogFile->deleteLater();
				m_pLogFile = 0;
			}
			else
            {
                res = true;
            }
		}

		return res;
	}

    void st_logger::messageOutput(QtMsgType type, const QMessageLogContext& context, const QString& msg)
	{
		m_mutextLogger.lock();
        switch (type)
        {
            case QtDebugMsg:
                if (m_nLogLevel < 3)
                {
                    m_mutextLogger.unlock();
                    return;
                }
                break;
            case QtWarningMsg:
                if (m_nLogLevel < 2)
                {
                    m_mutextLogger.unlock();
                    return;
                }
                break;
            case QtCriticalMsg:
                if (m_nLogLevel < 1)
                {
                    m_mutextLogger.unlock();
                    return;
                }
                break;
            case QtFatalMsg:
                if (m_nLogLevel < 0)
                {
                    m_mutextLogger.unlock();
                    return;
                }
                break;
            default:
                if (m_nLogLevel < 3)
                {
                    m_mutextLogger.unlock();
                    return;
                }
                break;
		}
        if (m_pLogFile == 0)
		{
            if (m_bUseLogFile)
            {
                m_bUseLogFile = createNewLogFile(QCoreApplication::instance());
            }
            if (m_pLogFile == 0)
			{
				m_mutextLogger.unlock();
				return;
			}
		}

		QDateTime dtmCur = QDateTime::currentDateTime().toUTC();

		QString strMsg ;
		strMsg += dtmCur.toString("yyyy-MM-dd HH:mm:ss.zzz");
		QString strMsgHeader = dtmCur.toString("\n                       ");
		strMsg += "(UTC)>";
		strMsgHeader += "      ";
        switch (type)
        {
            case QtDebugMsg:
                strMsg += QString("Debug   :");
                break;
            case QtWarningMsg:
                strMsg += QString("Warning :");
                break;
            case QtCriticalMsg:
                strMsg += QString("Critical:");
                break;
            case QtFatalMsg:
                strMsg += QString("Fatal   :");
                break;
            default:
                strMsg += QString("Unknown :");
                break;
		}
		strMsg.append(msg);
		strMsgHeader += QString("         From {%1:%2,%3}\n").arg(QString(context.file)).arg(context.line).arg(QString(context.function));
		strMsg.append(strMsgHeader);

		QTextStream stream(m_pLogFile);
		stream << strMsg;
		stream.flush();
        if (m_pLogFile->pos() >= m_nMaxFileSize)
        {
            m_bUseLogFile = createNewLogFile(QCoreApplication::instance());
        }
		m_mutextLogger.unlock();
	}
}

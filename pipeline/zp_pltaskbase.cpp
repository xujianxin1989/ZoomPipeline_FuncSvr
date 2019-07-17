#include "zp_pltaskbase.h"

namespace ZPTaskEngine
{
    zp_plTaskBase::zp_plTaskBase(QObject *parent) : QObject(parent)
	{
        m_refCount = 0;
	}

	int zp_plTaskBase::addRef()
	{
		QMutexLocker locker(&m_mutex_ref);
        m_refCount++;
        return m_refCount;
	}

	int zp_plTaskBase::delRef()
	{
		QMutexLocker locker(&m_mutex_ref);
        m_refCount--;
        return m_refCount;
	}

	int zp_plTaskBase::ref()
	{
		QMutexLocker locker(&m_mutex_ref);
        return m_refCount;
	}

    bool zp_plTaskBase::lockRun()
	{
		return m_mutex_run.tryLock();
	}

    void zp_plTaskBase::unlockRun()
	{
		return m_mutex_run.unlock();
	}
}

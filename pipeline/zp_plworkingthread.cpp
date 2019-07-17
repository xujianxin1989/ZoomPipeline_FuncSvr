#include "zp_plworkingthread.h"
#include <assert.h>
#include "zp_pipeline.h"

namespace ZPTaskEngine
{
    zp_plWorkingThread::zp_plWorkingThread(zp_pipeline* pipl, QObject* parent) : QObject(parent)
	{
		m_bRuning = true;
		m_pipeline = pipl;
		assert(m_pipeline != NULL);
		m_bBusy = false;
	}

	/**
     * @brief This slot mark this zp_plWorkingThread object as stopped.
     * when tasks belong to this zp_plWorkingThread has been finished, it will quit.
	 *
	 * @fn zp_plWorkingThread::setStopMark
     * @param pThread the zp_plWorkingThread object recieved by signal-slot system.
	 * this method will omit zp_plWorkingThread objs except for it self.
	 */
    void zp_plWorkingThread::setStopMark(zp_plWorkingThread* pThread)
	{
        if (pThread != this)
        {
            return;
        }
		m_bRuning = false;
		m_pipeline->m_mutex_protect.lock();
		m_pipeline->m_nExistingThreads--;
		m_pipeline->m_mutex_protect.unlock();
		this->deleteLater();
		QThread::currentThread()->quit();
	}

	/**
	 * @brief Call zp_plTaskBase::popTask to fetch new tasks.
	 *
     * @fn zp_plWorkingThread::fetchNewTask
     * @param pThread the zp_plWorkingThread object recieved by signal-slot system.
     * this method will omit zp_plWorkingThread objects except for it self.
	 */
    void zp_plWorkingThread::fetchNewTask(zp_plWorkingThread* pThread)
	{
        if (pThread != this)
        {
            return;
        }
        if (!m_bRuning)
        {
            return;
        }
        bool bValid = false;
        zp_plTaskBase *task = m_pipeline->popTask(&bValid);
        if (bValid && task)
        {
            m_bBusy = true;
            if (task->lockRun())
            {
                int res = task->run();
                task->delRef();
                if (res != 0)
                {
                    m_pipeline->pushTask(task, false);
                }
                task->unlockRun();
            }
            else
            {
                task->delRef();
                m_pipeline->pushTask(task, false);
            }
            m_bBusy = false;
        }

        emit evt_TaskFinished(this);
	}
}

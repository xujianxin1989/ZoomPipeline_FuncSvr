#include "zp_pipeline.h"

namespace ZPTaskEngine
{
    zp_pipeline::zp_pipeline(QObject* parent) : QObject(parent)
	{
		m_nExistingThreads = 0;
	}

	bool zp_pipeline::canClose() const
	{
        return m_nExistingThreads == 0;
	}

	/**
	 * @brief Add nThreads to the thread pool
	 *
	 * @fn zp_pipeline::addThreads
	 * @param nThreads how many threads you want to add.
	 * @return int current threads count after add.
	 */
	int zp_pipeline::addThreads(int nThreads)
	{
        if (nThreads < 1 || nThreads > 128)
        {
            return m_vec_workingThreads.size();
        }

        for (int i = 0; i < nThreads; i++)
        {
            zp_plWorkingThread* workThread = new zp_plWorkingThread(this);
            m_vec_workingThreads.push_back(workThread);

            QThread* pThread = new QThread(this);
            m_vec_InternalworkingThreads.push_back(pThread);
            workThread->moveToThread(pThread);
            connect(this, &zp_pipeline::evt_Start_work, workThread, &zp_plWorkingThread::fetchNewTask, Qt::QueuedConnection);
            connect(this, &zp_pipeline::evt_Stop_work, workThread, &zp_plWorkingThread::setStopMark, Qt::QueuedConnection);
            connect(workThread, &zp_plWorkingThread::evt_TaskFinished, this, &zp_pipeline::on_Finished_task, Qt::QueuedConnection);
            pThread->start();
            m_mutex_protect.lock();
            m_nExistingThreads++;
            m_mutex_protect.unlock();
        }

		return m_vec_workingThreads.size();
	}

	/**
     * @brief remove n threads and kill them. nthreads = -1 means kill all.
	 *
	 * @fn zp_pipeline::removeThreads
	 * @param nThreads how many threads to kill
	 * @return int the threads count after del
	 */
	int zp_pipeline::removeThreads(int nThreads)
	{
        int size = m_vec_workingThreads.size();
        if (nThreads < 0 || nThreads > size)
        {
            nThreads = size;
        }
        for (int i = 0; i < nThreads; i++)
		{
            emit evt_Stop_work(m_vec_workingThreads.last());
			m_vec_workingThreads.pop_back();
			m_vec_InternalworkingThreads.pop_back();
		}
		return m_vec_workingThreads.size();
	}

	/**
	 * @brief  Threads call this function to get next task, task will be popped from list.
	 *
	 * @fn zp_pipeline::popTask
	 * @param bValid return whether this task is valid
	 * @return zp_plTaskBase the top-task in queue
	 */
    zp_plTaskBase* zp_pipeline::popTask(bool* bValid)
	{
		*bValid = false;
        zp_plTaskBase* funcres = nullptr;
		m_mutex_protect.lock();
        if (!m_list_tasks.empty())
		{
            funcres = *m_list_tasks.begin();
			m_list_tasks.pop_front();
			*bValid = true;
		}
		m_mutex_protect.unlock();
		return funcres;
	}

	/**
	 * @brief Call this function to insert an task into queue.
	 * task will be put into the tail of the fifo
	 * @fn zp_pipeline::pushTask
	 * @param task the pointer to this task object
	 * @param bFire if true, will fire an deal-event immediately
	 */
    void zp_pipeline::pushTask(zp_plTaskBase* task, bool bFire)
	{
		m_mutex_protect.lock();
		m_list_tasks.push_back(task);
		task->addRef();
		m_mutex_protect.unlock();

        if (!bFire)
        {
            return;
        }
        int size = m_vec_workingThreads.size();
        for (int i = 0; i < size; i++)
        {
            zp_plWorkingThread* workThread = m_vec_workingThreads.at(i);
            if (workThread->m_bBusy)
            {
                continue;
            }
            on_Finished_task(workThread);
            break;
        }
	}

	int zp_pipeline::threadsCount() const
	{
		return m_vec_workingThreads.size();
	}

    int zp_pipeline::payLoad()
	{
		int res;
		m_mutex_protect.lock();
		res = m_list_tasks.size();
		m_mutex_protect.unlock();

		return res;
	}

	int  zp_pipeline::idleThreads() const
	{
		int idle = 0;
        int size = m_vec_workingThreads.size();
        for (int i = 0; i < size; i++)
		{
            if (!m_vec_workingThreads.at(i)->m_bBusy)
            {
                idle++;
            }
		}
		return idle;
	}

	/**
	 * @brief this slot will be called when a task is finished,
     * in this method, it will check whether the queue is still
     * not empty and fire the event to deal with next job.
     * on_Finished_task 槽函数既是任务的起始，也是任务的结束。
     * 当某个 zp_plWorkingThread 对象执行完了一次任务，便会触发本方法。
     * 在本方法中，zp_pipeline 对象检查自己的队列，看看是否还有任务需要执行。
     * 如果有，则读入一个任务继续执行。
     * @fn zp_pipeline::on_Finished_task
	 * @param task
	 */
    void zp_pipeline::on_Finished_task(zp_plWorkingThread* task)
	{
		int res;
		m_mutex_protect.lock();
		res = m_list_tasks.size();
		m_mutex_protect.unlock();
		if (res)
        {
            emit evt_Start_work(task);
        }
	}
}

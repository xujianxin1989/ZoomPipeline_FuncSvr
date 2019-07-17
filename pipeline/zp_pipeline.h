#ifndef ZP_PIPELINE_H
#define ZP_PIPELINE_H

#include <QObject>
#include <QVector>
#include <list>
#include <QThread>
#include "zp_plworkingthread.h"
#include "zp_pltaskbase.h"

/**!
  The pipe line will process each task item group by group.
  quit your task as soon as possible, so that other tasks can get more cpu time.
*/
namespace ZPTaskEngine
{
	class zp_plWorkingThread;

	/**
	 * @brief the task engine class. hold some threads to do fifo-like tasks.
     * 该类是流水线线程池的接口类。其管理了各个执行者线程，以及任务队列。
     * 执行者线程存储在本类的成员变量中。
	 * @class zp_pipeline zp_pipeline.h "ZoomPipeline_FuncSvr/pipeline/zp_pipeline.h"
	 */
	class zp_pipeline : public QObject
	{
		Q_OBJECT
        friend class zp_plWorkingThread;

	public:
        explicit zp_pipeline(QObject *parent = 0);
		int addThreads(int nThreads);

        //remove n threads and kill them. nThreads = -1 means kill all.
		int removeThreads(int nThreads);
		int threadsCount() const;
        int payLoad();
		int idleThreads() const;
        bool canClose() const;

	protected:
		//Mutex
		QMutex m_mutex_protect;
        //working thread objects
        QVector<zp_plWorkingThread *> m_vec_workingThreads;
        //working threads
        QVector<QThread *> m_vec_InternalworkingThreads;

		//This is a C++11 function pool.
        //return -1, the function will be kept in list,
        //return  0, will be removed.
        std::list<zp_plTaskBase *> m_list_tasks;
		int m_nExistingThreads;

		//Threads call this function to get next task, task will be popped from list.
        zp_plTaskBase* popTask(bool *bValid);

	signals:
        void evt_Start_work(zp_plWorkingThread *task);
        void evt_Stop_work(zp_plWorkingThread *task);

	public slots:
        void on_Finished_task(zp_plWorkingThread *task);
		//Call this function to insert func
        void pushTask(zp_plTaskBase *task, bool bFire = true);
	};
}
#endif // ZP_PIPELINE_H

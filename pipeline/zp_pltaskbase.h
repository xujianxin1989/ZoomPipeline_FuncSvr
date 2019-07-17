#ifndef ZP_PLTASKBASE_H
#define ZP_PLTASKBASE_H

#include <QObject>
#include <QMutexLocker>

namespace ZPTaskEngine
{
	/**
	 * @brief the pure-base class for tasks , will be called by zp_plWorkingThread
	 * Users will imp your own int zp_plTaskBase::run() function.
     * zp_plTaskbase 类一个纯虚基类，用于给应用者重载具体的执行任务。
     * 该类的核心方法是 run()，用于在线程池的某个线程中运行。
	 */
	class zp_plTaskBase : public QObject
	{
		Q_OBJECT

	public:
        explicit zp_plTaskBase(QObject *parent = nullptr);

		/**
		 * @brief the virtual function called by zp_plWorkingThread
		 * this function will be called in thread pool
		 * @return int 0 means this object will be removed from pipeline-fifo. otherwise,
		 * when run() finished, the object will be re-injected into the tail of task-fifo.
		 */
		virtual int run() = 0;
		int addRef();
		int delRef();
		int ref();
        bool lockRun();
        void unlockRun();

	private:
        int m_refCount;
		QMutex m_mutex_ref;
		QMutex m_mutex_run;
	};
}
#endif // ZP_PLTASKBASE_H

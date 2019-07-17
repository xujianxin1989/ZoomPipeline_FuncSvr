#ifndef ZP_PLWORKINGTHREAD_H
#define ZP_PLWORKINGTHREAD_H

#include <QObject>
#include "zp_pltaskbase.h"

namespace ZPTaskEngine
{
	class zp_pipeline;
	/**
	 * @brief /Working thread, reading functions from queue,
     * call the tasks "run" method, doing actual works.
     * 这个类是用于执行任务的线程对象。在其被创建时，
     * 绑定在一个 QThread 的线程事件循环中运行。
     * 创建的方法位于 zp_pipeline::addThreads 中。
	 * @class zp_plWorkingThread zp_plworkingthread.h "ZoomPipeline_FuncSvr/pipeline/zp_plworkingthread.h"
	 */
	class zp_plWorkingThread : public QObject
	{
		Q_OBJECT

	public:
        explicit zp_plWorkingThread(zp_pipeline *pipl, QObject *parent = nullptr);
		bool m_bBusy;

	protected:
        zp_pipeline *m_pipeline;
		bool m_bRuning;

	public slots:
        void setStopMark(zp_plWorkingThread *pThread);
        void fetchNewTask(zp_plWorkingThread *pThread);

	signals:
        void evt_TaskFinished(zp_plWorkingThread *);
	};
}
#endif // ZP_PLWORKINGTHREAD_H

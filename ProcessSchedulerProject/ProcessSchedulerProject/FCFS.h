#ifndef FCFS_H
#define FCFS_H

#include<queue>
#include"Process.h"
#include"Scheduler.h"

namespace ProcessScheduling
{
	class Process;

	class FCFS: public Scheduler
	{
	private:
		queue<Process*> ready;
		queue<Process*> wait;
	public:	
		FCFS(unsigned aIODelay, unsigned aContextSwitchDelay, Random aRandom);

		void moveProcessToReady(Process* p);

		void moveProcessFromReadyToRun();

	    void updateRunningProcess();

		void updateBlockingProcessInWait();

		void displaySchedulerStatus();

	    // process which finished waiting for I/O to complete
		// is moved to Ready queue
		void moveProcessFromWaitToReady();

		friend ostream& operator<<(ostream& os, FCFS& fcfs);
	};
}

#endif
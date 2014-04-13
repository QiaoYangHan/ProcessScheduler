#ifndef CTSS_H
#define CTSS_H

#include<vector>
#include<queue>

#include"Process.h"
#include"Scheduler.h"

namespace ProcessScheduling
{
	class Process;

	class CTSS: public Scheduler
	{
	private:
		vector<queue<Process*>> ready;
		vector<queue<Process*>> wait;

		int CTSSQueues;
	public:
		CTSS(unsigned aIODelay, unsigned aContextSwitchDelay, Random aRandom, int aCTSSQueues);

		void moveProcessToReady(Process* p);

		void moveProcessFromWaitToReady();

		void moveProcessFromReadyToRun();

	    void updateRunningProcess();

		void updateBlockingProcessInWait();

		void displaySchedulerStatus();

		friend ostream& operator<<(ostream& os, CTSS& ctss);
	};
}

#endif
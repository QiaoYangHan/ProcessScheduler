#ifndef SCHEDULER_H
#define SCHEDULER_H

#include<windows.h>
#include"Process.h"
#include<queue>
#include<vector>
#include<iostream>

using namespace std;

namespace ProcessScheduling
{
	class Process;

	class Random;

	class Scheduler
	{
	private:
		vector<Process*> processSet;

	protected:
		queue<Process*> arrived;
		
		const unsigned IODelay;
		const unsigned contextSwitchDelay;

		unsigned currentCPUCycle;
		unsigned remainingContextSwitchDelay;

		Process* runningProcess;
		Random random;

	public:
		Scheduler(unsigned aIODelay, unsigned aContextSwitchDelay, Random aRadnom);

		virtual void moveProcessToReady(Process* p) = 0;

		virtual void moveProcessFromReadyToRun() = 0;

		virtual void updateBlockingProcessInWait() = 0;

		virtual void moveProcessFromWaitToReady() = 0;

		virtual void updateRunningProcess() = 0;

		virtual void displaySchedulerStatus() = 0;


		void run();

		//Advance the clock cycle 
		void update();

		void addProcessToArrived(Process* p);

		void runProcess(Process* p);

		void contextSwitch();

		bool isInContextSwitch();

		bool isSchedulerDone();

		unsigned getCurrentCPUCycle();
	};
}
#endif 
#include"Scheduler.h"
#include"Process.h"
#include <vector>
#include <iostream>

using namespace std;

namespace ProcessScheduling
{
	Scheduler::Scheduler(unsigned aIODelay, unsigned aContextSwitchDelay, Random aRandom):
		IODelay(aIODelay), contextSwitchDelay(aContextSwitchDelay), random(aRandom)
	{
		remainingContextSwitchDelay = 0;
		currentCPUCycle = 0;
		runningProcess = NULL;
	}

	void Scheduler::run()
	{
		while (!isSchedulerDone())
		{
			while (!arrived.empty() && arrived.front()->getArrivalTime() == currentCPUCycle)
			{
				this->moveProcessToReady(arrived.front());
				arrived.pop();
			}
			update();
			Sleep(1000);
		}
	}

	void Scheduler::update()
	{
		this->displaySchedulerStatus();
		// IODelay doesn't conflict with ContextSwitchDelay
		this->updateBlockingProcessInWait();

		if(!isInContextSwitch())
		{
			this->moveProcessFromWaitToReady();
			if (runningProcess)
			{
				this->updateRunningProcess();
			}

			// updateRunningProcess may change runningProcess to NULL
			if (!runningProcess && !isInContextSwitch())
				this->moveProcessFromReadyToRun();
		}

		currentCPUCycle++;
		if (remainingContextSwitchDelay > 0)
			remainingContextSwitchDelay--;
	}
    
	void Scheduler::addProcessToArrived(Process* p)
	{
		arrived.push(p);
		p->setProcessStateArrived();
		processSet.push_back(p);
	}

	void Scheduler::runProcess(Process* p)
	{
		runningProcess = p;
		runningProcess->startBurst(currentCPUCycle);
	}

	void Scheduler::contextSwitch()
	{
		remainingContextSwitchDelay = contextSwitchDelay;
	}

	bool Scheduler::isInContextSwitch()
	{
		return (remainingContextSwitchDelay > 0) ? true : false;
	}

	bool Scheduler::isSchedulerDone()
	{
		bool isDone = true;
		for (size_t i = 0; i < processSet.size(); i++)
		{
			if (!processSet[i]->isProcessDone())
				isDone = false;
		}
		return isDone;
	}

	unsigned Scheduler::getCurrentCPUCycle()
	{
		return currentCPUCycle;
	}
}
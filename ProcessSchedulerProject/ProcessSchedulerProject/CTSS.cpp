#include"CTSS.h"
#include"Process.h"
#include"Scheduler.h"

using namespace std;

namespace ProcessScheduling
{
	CTSS::CTSS(unsigned aIODelay, unsigned aContextSwitchDelay, Random aRandom, int aCTSSQueues):
	Scheduler(aIODelay, aContextSwitchDelay, aRandom),
	CTSSQueues(aCTSSQueues)
	{
		for (int i = 0; i < CTSSQueues; i++)
		{
			queue<Process*> r;
			queue<Process*> w;
			ready.push_back(r);
			wait.push_back(w);
		}
	}

	void CTSS::moveProcessToReady(Process* p)
	{
		ready[0].push(p);
		p->setProcessStateReady();
	}

	void CTSS::updateRunningProcess()
	{
		if (runningProcess)
		{
			runningProcess->elapsedBurstTime++;
			runningProcess->elapsedTotalTime++;
			runningProcess->usedQuantum++;

			if (!runningProcess->isBurstFinished(random))
			{
				if (runningProcess->fullQuantum <= runningProcess->usedQuantum)
				{
					runningProcess->priority =
					(runningProcess->priority < CTSSQueues) ? runningProcess->priority + 1 : runningProcess->priority;
					runningProcess->setProcessStateReady();
					runningProcess->fullQuantum *= 2;
					runningProcess->usedQuantum = 0;
					ready[runningProcess->priority - 1].push(runningProcess);
					contextSwitch();
					runningProcess = NULL;
				}
				// runningProcess is prempted by a higher priority process
				else
				{
					queue<Process*> temp;
					runningProcess->fullQuantum = 
						runningProcess->fullQuantum - runningProcess->usedQuantum;
					runningProcess->usedQuantum = 0;
					temp.push(runningProcess);
					while (!ready[runningProcess->priority - 1].empty())
					{
						temp.push(ready[runningProcess->priority - 1].front());
						ready[runningProcess->priority - 1].pop();
					}
					ready[runningProcess->priority - 1] = temp;
					contextSwitch();
					runningProcess = NULL;
				}
			}
			else
			{
				runningProcess->endBurst(currentCPUCycle, IODelay);
				runningProcess->usedQuantum = 0;
				if (!runningProcess->isProcessDone())
				{
					if (((double)runningProcess->elapsedBurstTime / (double)runningProcess->fullQuantum)
						<= 0.5)
					{
						runningProcess->priority = (runningProcess->priority > 1) ? (runningProcess->priority - 1) : 1;
						wait[runningProcess->priority - 1].push(runningProcess);	
					}
					else
						wait[runningProcess->priority - 1].push(runningProcess);
				}
				contextSwitch();
				runningProcess = NULL;
			}
		}
	}

	void CTSS::moveProcessFromReadyToRun()
	{
		for (size_t priority = 0; priority < ready.size(); priority++)
		{
			if (!ready[priority].empty())
			{
				runProcess(ready[priority].front());
				runningProcess->priority = priority + 1;
				runningProcess->fullQuantum = ((priority == 0) ? 1 : 1 << priority);
				ready[priority].pop();
				break;
			}
		}
	}

	void CTSS::updateBlockingProcessInWait()
	{
		for (size_t priority = 0; priority < wait.size(); priority++)
		{
			queue<Process*> temp = wait[priority];
			while (!temp.empty())
			{
				temp.front()->updateBlockingTime();
				temp.pop();
			}
		}
	}

	void CTSS::moveProcessFromWaitToReady()
	{
		for (size_t priority = 0; priority < wait.size(); priority++)
		{
			queue<Process*> waitQueue = wait[priority];
			queue<Process*> waitNextCycle;
			while (!waitQueue.empty())
			{
				if (waitQueue.front()->isProcessBlocked())
				{
					waitNextCycle.push(waitQueue.front());
					cout << "Process " << waitQueue.front()->getID() << " is still waiting" << endl;
				}
				else
				{
					ready[priority].push(waitQueue.front());
					waitQueue.front()->setProcessStateReady();
					cout << "Process " << waitQueue.front()->getID() << " is still waiting" << endl;
				}
				waitQueue.pop();
			}
			wait[priority] = waitNextCycle; 
		}
	}

	void CTSS::displaySchedulerStatus()
	{
		cout << *this << endl;
	}

	ostream& operator<<(ostream& os, CTSS& ctss)
	{
		os << "==========" << endl
		   << "Time: " << ctss.currentCPUCycle << endl;

		if(!ctss.isInContextSwitch())
		{
			os << "Running: ";

			if(!ctss.runningProcess)
			{
				os << "none";
			}
			else
			{
				os << ctss.runningProcess->getID();
			}
			os << endl;

			os << "Arrival: ";
			queue<Process*> arrived = ctss.arrived;
			if(arrived.empty())
			{
				os << "none";
			}
			while(!arrived.empty())
			{
				os << arrived.front()->getID() << " ";
				arrived.pop();
			}
			os << endl;

			for (size_t i = 0; i < ctss.ready.size(); i++)
			{
				os << "Ready[" << i << "]: ";
				queue<Process*> ready = ctss.ready[i];
				if(ready.empty())
				{
					os << "none";
				}
				while(!ready.empty())
				{
					os << ready.front()->getID() << " ";
					ready.pop();
				}
				os << endl;
			}

			for (size_t i = 0; i < ctss.wait.size(); i++)
			{
				os << "Waiting[" << i << "]: ";
				queue<Process*> wait = ctss.wait[i];
				if(wait.empty())
				{
					os << "none";
				}
				while(!wait.empty())
				{
					os << wait.front()->getID() << " ";
					wait.pop();
				}
				os << endl;
			}
		}
		else
			os << "Remaining Context Switch Delay: " << ctss.remainingContextSwitchDelay << endl;
		os << "==========";
		return os;
	}
}
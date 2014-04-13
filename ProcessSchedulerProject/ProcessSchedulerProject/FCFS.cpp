#include"FCFS.h"
#include"Process.h"
#include<math.h>

using namespace std;

namespace ProcessScheduling
{
	FCFS::FCFS(unsigned aIODelay, unsigned aContextSwitchDelay, Random aRandom)
		:Scheduler(aIODelay, aContextSwitchDelay, aRandom)
	{
	}

	void FCFS::moveProcessToReady(Process* p)
	{
		ready.push(p);
		p->setProcessStateReady();
	}

	void FCFS::moveProcessFromReadyToRun()
	{
		if (ready.size() > 0)
		{
			runProcess(ready.front());
			ready.pop();
		}
	}

	void FCFS::updateRunningProcess()
	{
		// if there is one process in Running state
		if (runningProcess)
		{
			runningProcess->elapsedBurstTime++;
			runningProcess->elapsedTotalTime++;

			if (!runningProcess->isBurstFinished(random))
				return;

			else
			{
				runningProcess->endBurst(currentCPUCycle, IODelay); 
				if (!runningProcess->isProcessDone())
					wait.push(runningProcess);
				contextSwitch();
				runningProcess = NULL;
			}
		}
	}

	void FCFS::updateBlockingProcessInWait()
	{
		queue<Process*> temp = wait;
		while(!temp.empty())
		{
			temp.front()->updateBlockingTime();
			temp.pop();
		}
	}

	void FCFS::moveProcessFromWaitToReady()
	{
		queue<Process*> waitNextCycle;
		while (!wait.empty())
		{
			if (!wait.front()->isProcessBlocked())
			{
				ready.push(wait.front());
				wait.front()->setProcessStateReady();
				cout << "Process " << wait.front()->getID() 
					 << " is moved from WAITING to READY" << endl;
			}
			else
			{
				waitNextCycle.push(wait.front());
				cout << "Process " << wait.front()->getID() 
					 << " is still in WAITING" << endl;
			}
			wait.pop();
		}
		wait = waitNextCycle;
	}

	void FCFS::displaySchedulerStatus()
	{
		cout << *this << endl;
	}

	ostream& operator<<(ostream& os, FCFS& fcfs)
	{
		os << "==========" << endl
		   << "Time: " << fcfs.currentCPUCycle << endl;

		if(!fcfs.isInContextSwitch())
		{
			os << "Running: ";

			if(!fcfs.runningProcess)
			{
				os << "none";
			}
			else
			{
				os << fcfs.runningProcess->getID();
			}
			os << endl;

			os << "Arrival: ";
			queue<Process*> arrived = fcfs.arrived;
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

			os << "Ready: ";
			queue<Process*> ready = fcfs.ready;
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

			os << "Waiting: ";
			queue<Process*> wait = fcfs.wait;
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
		else
			os << "Remaining Context Switch Delay: " << fcfs.remainingContextSwitchDelay << endl;
		os << "==========";
		return os;
	}

}
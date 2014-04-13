// Task
// get the resource file "scheduling.txt"
// the processes need to be scheduled are on the this file
// run two algorithm on these processes and produce output

#include"Process.h"

#include<iostream>

namespace ProcessScheduling
{
	Process::Process(unsigned aId, unsigned anArrivalTime,
			unsigned aTotalCPU, unsigned anAverageCPUBurst):
		id(aId), arrivalTime(anArrivalTime), totalCPU(aTotalCPU),
		averageCPUBurst(anAverageCPUBurst)
	{
		hasStarted = false;
		blockingTime = 0;
		elapsedBurstTime = 0;
		elapsedTotalTime = 0;
		startTime = -1;
		endTime = -1;
		burstCount = 0;
	}

	void Process::startBurst(unsigned currentCPUCycle)
	{
		burstCount++;
		if (!hasStarted)
		{
			hasStarted = true;
			startTime = currentCPUCycle;
		}
		elapsedBurstTime = 0;
		state = RUNNING;
		cout << "Process " << id << endl
			 << "Starting burst (" << burstCount
			 << ") is moved from READY to RUNNING" << endl
			 << "Remaining time: " 
			 << (totalCPU - elapsedTotalTime) << endl;
	}

	void Process::endBurst(unsigned currentCPUCycle, unsigned IODelay)
	{
		if (elapsedTotalTime >= totalCPU)
		{
			endTime = currentCPUCycle;
			state = DONE;
			cout << "Process " << id << " is finished" << endl;
		}
		else
		{
			blockingTime = IODelay;
			state = WAITING;
			cout << "Process " << id << endl
				 << "Ending burst (" << burstCount
				 << ") is moved from RUNNING to WAITING" << endl
				 << "Remaining time: " 
				 << (totalCPU - elapsedTotalTime) << endl;
		}
	}

	bool Process::isBurstFinished(Random& random)
	{
		if (elapsedTotalTime == totalCPU)
			return true;
		else if (elapsedBurstTime < averageCPUBurst - 1)
			return false;
		else if (elapsedBurstTime == averageCPUBurst - 1)
		{
			if (random.getProbability() <= 1.0/3.0)
				return true;
			else 
				return false;
		}
		else if (elapsedBurstTime == averageCPUBurst)
		{
			if (random.getProbability() <= 1.0/2.0)
				return true;
			else 
				return false;
		}
		else if (elapsedBurstTime > averageCPUBurst)
			return true;
	}

	bool Process::isProcessBlocked()
	{
		return (blockingTime > 0) ? true : false;
	}

	bool Process::isProcessDone()
	{
		return (state == DONE) ? true : false;
	}

	void Process::updateBlockingTime()
	{
		if (blockingTime > 0)
			blockingTime--;
	}

	void Process::setProcessStateArrived()
	{
		state = ARRIVED;
	}

	void Process::setProcessStateReady()
	{
		state = READY;
	}

	unsigned Process::getID()
	{
		return id;
	}

	unsigned Process::getArrivalTime()
	{
		return arrivalTime;
	}

	unsigned Process::getTotalCPU()
	{
		return totalCPU;
	}
	
	unsigned Process::getTurnaroundTime()
	{
		return endTime - arrivalTime;
	}

	ostream& operator<<(ostream& os, Process& p)
	{
		os << "PID: " << p.id << "\t";
		os << "Arrival: " << p.arrivalTime << "\t";
		os << "Total CPU: " << p.totalCPU << "\t";
		os << "Average Burst: " << p.averageCPUBurst << endl;
		
		if (p.state == ARRIVED)
			return os << "The Process is in ARRIVAL, Not Run Yet" << endl;

		os << "Start Time: " << p.startTime << endl;
		os << "End Time: " << p.endTime << endl;
		os << "Turnaround Time: " << p.getTurnaroundTime();
		return os;
	}
}


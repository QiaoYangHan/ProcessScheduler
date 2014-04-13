#ifndef PROCESS_H
#define PROCESS_H

#include"Random.h"
namespace ProcessScheduling
{
	enum ProcessState
	{
		ARRIVED,
		READY,
		RUNNING,
		WAITING,
		DONE
	};

	class Process 
	{
	private:
		unsigned id;
		unsigned arrivalTime;
		unsigned totalCPU;
		unsigned averageCPUBurst;

		unsigned blockingTime;

		unsigned startTime;
		unsigned endTime;

		int burstCount;

		bool hasStarted;
		ProcessState state;
	public:
		// Constructor
		Process(unsigned aId, unsigned anArrivalTime,
			unsigned aTotalCPU, unsigned anAverageCPUBurst);

		void startBurst(unsigned currentCPUCycle);

		void endBurst(unsigned currentCPUCycle, unsigned IODelay);

		// At each clock tick, is the process finished 
		// with its burst? 
		bool isBurstFinished(Random& random);

		bool isProcessBlocked();

		bool isProcessDone();

		void updateBlockingTime();

		void setProcessStateArrived();

		void setProcessStateReady();

		unsigned getID();

		unsigned getArrivalTime();

		unsigned getTotalCPU();

		unsigned getTurnaroundTime();

		friend ostream& operator<<(ostream& os, Process& p);

		unsigned elapsedBurstTime;
	    unsigned elapsedTotalTime;

		//CTSS
		int fullQuantum;
		int usedQuantum;
		int priority;
	};

}
#endif
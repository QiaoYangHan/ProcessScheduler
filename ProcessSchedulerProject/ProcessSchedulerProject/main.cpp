#include<stdlib.h>
#include<iostream>
#include<fstream>
#include<vector>
using namespace std;

#include"Configuration.h"
#include"Process.h"
#include"Random.h"
#include"Scheduler.h"
#include"FCFS.h"
#include"CTSS.h"
using namespace ProcessScheduling;

const string RANDOM_INPUT_FILENAME = "random.txt";

const string CONFIGURATION_FILENAME = "scheduling.txt";

void createProcesses(istream& ifs, vector<Process*>& processes)
{	
	while(!ifs.eof())
	{
		unsigned id;
		unsigned arrivalTime;
		unsigned totalCPU;
		unsigned averageBurst;
		ifs >> id >> arrivalTime >> totalCPU >> averageBurst;
		Process* p = new Process(id, arrivalTime, totalCPU, averageBurst);
		processes.push_back(p);
	}
}

int main()
{
	ifstream randomFile(RANDOM_INPUT_FILENAME);
	ifstream configurationFile(CONFIGURATION_FILENAME);
	
	Random random(randomFile);
	Configuration config(configurationFile);
	
	randomFile.close();
	configurationFile.close();
    
	ifstream processFile(config.processFile);
	vector<Process*> processes;
	createProcesses(processFile, processes);
	processFile.close();
	
	cout << "Qiao Yang Han" << endl
		 << "Process Scheduling" << endl
		 << "CS3224" << endl
		 << "Fall 2013" << endl
		 << "=========" << endl;

	cout << endl << config << endl;
	
	//FCFS fcfs(config.IODelay, config.contextSwitchDelay, random);

	//for (size_t i = 0; i < processes.size(); i++)
	//{
	//	fcfs.addProcessToArrived(processes[i]);
	//	cout << *processes[i] << endl;
	//}

	//cout << endl;

	//cout << "==============" << endl
	//	 << "     FCFS     " << endl
	//	 << "==============" << endl;

	//fcfs.run();
	CTSS ctss(config.IODelay, config.contextSwitchDelay, random, config.ctssQueues);
	for (size_t i = 0; i < processes.size(); i++)
	{
		ctss.addProcessToArrived(processes[i]);
		cout << *processes[i] << endl;
	}

	cout << endl;

	cout << "==============" << endl
		 << "     CTSS     " << endl
		 << "==============" << endl;
	ctss.run();
	system("pause");
}